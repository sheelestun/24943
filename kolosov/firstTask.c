#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <limits.h>
#include <fcntl.h>

extern char **environ;

typedef struct { char opt; char *arg; } Action;

static void print_ids(void){
    printf("UID: real=%ld effective=%ld\n", (long)getuid(), (long)geteuid());
    printf("GID: real=%ld effective=%ld\n", (long)getgid(), (long)getegid());
}
static void print_pids(void){
    printf("PID=%ld PPID=%ld PGID=%ld\n",(long)getpid(),(long)getppid(),(long)getpgid(0));
}
static void print_rlim(int res){
    struct rlimit rl;
    if (getrlimit(res, &rl) != 0) { perror("getrlimit"); return; }

    printf("soft=");
    if (rl.rlim_cur == RLIM_INFINITY) printf("unlimited");
    else printf("%llu", (unsigned long long)rl.rlim_cur);

    printf("  hard=");
    if (rl.rlim_max == RLIM_INFINITY) printf("unlimited\n");
    else printf("%llu\n", (unsigned long long)rl.rlim_max);
}

static int set_rlim_val(int res, const char *s){
    if (!s) return -1;
    struct rlimit rl; if (getrlimit(res,&rl)!=0){perror("getrlimit");return -1;}
    if (strcmp(s,"unlimited")==0) rl.rlim_cur = RLIM_INFINITY;
    else {
        errno=0; char *end=0; unsigned long long v=strtoull(s,&end,10);
        if (errno||!end||*end){fprintf(stderr,"bad limit: %s\n",s);return -1;}
        rl.rlim_cur=(rlim_t)v;
    }
    if (setrlimit(res,&rl)!=0){perror("setrlimit");return -1;}
    return 0;
}
static void print_cwd(void){
    char buf[PATH_MAX]; if (!getcwd(buf,sizeof buf)){perror("getcwd");return;} puts(buf);
}
static void print_env(void){ for (char **e=environ; e && *e; ++e) puts(*e); }

static void set_env_pair(const char *nv){
    const char *eq=strchr(nv,'=');
    if (!eq||eq==nv){fprintf(stderr,"-V expects name=value\n");return;}
    size_t nlen=(size_t)(eq-nv); char *name=malloc(nlen+1); if(!name){perror("malloc");return;}
    memcpy(name,nv,nlen); name[nlen]='\0';
    if (setenv(name, eq+1, 1)!=0) perror("setenv");
    free(name);
}
static void usage(const char* p){
    fprintf(stderr,"usage: %s [-i] [-s] [-p] [-u] [-U n|unlimited] [-c] [-C n|unlimited] [-d] [-v] [-V name=value]\n",p);
}

int main(int argc, char **argv){
    int opt; 
    Action *acts=NULL; 
    size_t n=0,cap=0;

    while ((opt=getopt(argc,argv,"ispuU:cC:dvV:"))!=-1){
        if (opt=='?'){ 
            usage(argv[0]); 
            free(acts); 
            return 1; }
        if (n==cap){ 
            cap=cap?cap*2:16; 
            Action *t=realloc(acts,cap*sizeof*acts); 
            if(!t){perror("realloc"); 
                return 1;} 
                acts=t; 
            }
        acts[n].opt=(char)opt;
        acts[n].arg=(opt=='U'||opt=='C'||opt=='V')?optarg:NULL;
        n++;
    }
    if (n==0){ usage(argv[0]); return 0; }

    for (ssize_t i=(ssize_t)n-1;i>=0;--i){
        switch(acts[i].opt){
            case 'i': print_ids(); break;
            case 's': if (setpgid(0,0)!=0) perror("setpgid"); break;
            case 'p': print_pids(); break;
            case 'u': print_rlim(RLIMIT_FSIZE); break;
            case 'U': set_rlim_val(RLIMIT_FSIZE, acts[i].arg); break;
            case 'c': print_rlim(RLIMIT_CORE); break;
            case 'C': set_rlim_val(RLIMIT_CORE, acts[i].arg); break;
            case 'd': print_cwd(); break;
            case 'v': print_env(); break;
            case 'V': set_env_pair(acts[i].arg); break;
            default: fprintf(stderr,"unknown -%c\n",acts[i].opt); break;
        }
    }
    free(acts);
    return 0;
}
