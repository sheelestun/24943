#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/resource.h>
//#include <linux/limits.h>
#include <string.h>

extern char** environ;

int main(int argc, char *argv[])
{
    char options[] = "ispuU:cC:dvV:";
    int c;
    long param;
    struct rlimit rl;

    printf("argc equals %d\n", argc);
    while ((c = getopt(argc, argv, options)) != EOF) {
        switch (c) {
            case 'i':
                printf("Real uid: %d\n", getuid());
                printf("Effective uid: %d\n", geteuid());
                printf("Real gid: %d\n", getgid());
                printf("Effective gid: %d\n", getegid());
                break;
            
            case 's':
                setpgid(0, 0);
                break;

            case 'p':
                printf("Process ID: %d\n", getpid());
                printf("Parent Process ID: %d\n", getppid());
                printf("Process Group ID: %d\n", getpgid(0));
                break;

            case 'u':
                getrlimit(RLIMIT_FSIZE, &rl); //RLIMIT_FSIZE потому-что написано ulimit, а эта функция работает только с этим лимитом
                printf("ulimit: %ld\n", rl.rlim_cur);
                break;

            case 'U':
                getrlimit(RLIMIT_FSIZE, &rl);
                param = atol(optarg);
                if (param < -1) {
                    printf("(U): You should use positive number or -1 for unlimited\n");
                    break;
                }
                rl.rlim_cur = param;
                setrlimit(RLIMIT_FSIZE, &rl);
                break;

            case 'c':
                getrlimit(RLIMIT_CORE, &rl);
                printf("Core file size limit: %ld\n", rl.rlim_cur);
                break;

            case 'C':
                getrlimit(RLIMIT_CORE, &rl);
                param = atol(optarg);
                if (param < -1) {
                    printf("(C): You should use positive number or -1 for unlimited\n");
                    break;
                }
                rl.rlim_cur = param;
                setrlimit(RLIMIT_CORE, &rl);
                break;

            case 'd': {
                char cwd[500];
                getcwd(cwd, sizeof(cwd));
                printf("Current working directory: %s\n", cwd);
                break;
            }

            case 'v': {
                char **env;
                for (env = environ; *env != 0; env++) {
                    printf("%s\n", *env);
                }
                break;
            }

            case 'V': {
                char* checker = strchr(optarg, '=');
                if (checker != NULL && checker != optarg && checker[1] != '\0')
                    putenv(optarg);
                else
                    printf("You shold use \"NAME=valeue\" format for -V option\n");
                break;
            }
                
            case '?':
                break;
        }
    }

    if(optind < argc) printf("next parameter = %s\n", argv[optind]);
    
    return 0;
}