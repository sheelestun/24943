#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
    int fd = open("file.txt", O_RDONLY);
    

    char c;
    long table[2][500];
    int i = 0, j = 0;
    table[0][0] = 0L;
    while(read(fd, &c, 1)) {
        if (c == '\n') {
            j++;
            table[1][i] = j;
            table[0][i] = lseek(fd, 0L, SEEK_CUR);
            j = 0;
            i++;
        }
        else j++;
    }

    int n = 1;
    while(n){
        printf("Enter the row idx or 0 to stop: ");
        scanf("%d", &n);
        if (n < 0) printf("You should use positive number or 0 to stop!\n");
        else if (n > i) printf("There are only %d rows!\n", i);
        else {
            lseek(fd, table[0][n-1] - table[1][n - 1], SEEK_SET);
            char buf[table[1][n-1] + 1];
            read(fd, buf, table[1][n-1]);
            buf[table[1][n-1]] = '\0';
            printf("%s", buf);
        }
    }
    
    close(fd);
    return 0;
}