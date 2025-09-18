#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>

int main(int argc, char *argv[])
{
    char options[ ] = "ispucdv";
    int c, invalid = 0, iflg = 0;
    char *i_ptr;

    printf("argc equals %d\n", argc);
    while ((c = getopt(argc, argv, options)) != EOF) {
        switch (c) {
            case 'i':
                
                break;
            
            case '?':
                printf("invalid option is %c\n", optopt);
                invalid++;
        }
    }   

    printf("invalid equals %d\n", invalid);
    printf("optind equals %d\n", optind);
    if(optind < argc) printf("next parameter = %s\n", argv[optind]);
}