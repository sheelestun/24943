#include <unistd.h>
#include <stdio.h>

void print_id(){
    printf("UID: %d\n", getuid());
    printf("EUID: %d\n", geteuid());
}

void open_file(char *filename){
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("Fail\n");
        return;
    }

    fclose(file);
}

int main(int argc, char ** argv){
    if (argc < 2) {
        printf("Filename is not specified\n");
        return 1;
    }

    print_id();
    open_file(argv[1]);

    setuid(getuid());

    print_id();
    open_file(argv[1]);

    return 0;
}
