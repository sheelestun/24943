#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    char options[] = "ispucdv";
    int c;
    int invalid = 0;

    printf("argc equals %d\n", argc);
    while ((c = getopt(argc, argv, options)) != EOF) {
        switch (c) {
            case 'i':
                printf("Real uid: %d\n", getuid());
                printf("Effective uid: %d\n", geteuid());
                printf("Real gid: %d\n", getgid());
                printf("Effective gid: %d\n", getegid());
                break;
            
            case 'p':
                setpgid(0, 0);
                break;
            case '?':
                printf("invalid option is %c\n", (char)optopt);
                invalid++;
                break;
        }
    }

    if(optind < argc) printf("next parameter = %s\n", argv[optind]);
    
    return 0;
}