#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>

#define BUFSIZE 500

int main(){
    int fd = open("file.txt", O_RDONLY);
    
    int terminal = open("/dev/tty", O_RDONLY | O_NDELAY);

    off_t size = lseek(fd, 0, SEEK_END);
    
    // Отображаем файл в память
    char *p = (char*)mmap(0, size, PROT_READ, MAP_SHARED, fd, 0);
    
    // Создаем двумерный массив для хранения данных о строках
    long table[2][500];  // table[0][i] - указатель на позицию после конца строки, table[1][i] - длина строки
    int i = 0, j = 0;
    
    // Инициализация для начала работы
    table[0][0] = 0;  // Будет заполнено позже при обнаружении первого переноса строки
    
    // Сканируем файл и находим позиции концов строк
    for(int count = 0; count < size; count++) {
        if (p[count] == '\n') {
            j++;
            table[1][i] = j;  // Запоминаем длину текущей строки (включая \n)
            table[0][i] = count + 1;  // Позиция сразу после переноса строки
            i++;
            j = 0;
        }
        else j++;
    }
    
    int num_lines = i;  // Количество строк в файле
    
    char buf[BUFSIZE];
    
    for (;;) {
        printf("You have 5 seconds to enter a line number: ");
        fflush(stdout);
        sleep(5);
        
        int bytes_read = read(terminal, buf, BUFSIZE - 1);
        if (bytes_read <= 0) {
            // Нет ввода - печатаем весь файл
            printf("\nTime's up! Printing entire file:\n");
            write(STDOUT_FILENO, p, size);  // Используем mmap вместо read/write
            munmap(p, size);
            close(fd);
            close(terminal);
            exit(0);
        }
        else {
            // Есть ввод - обрабатываем только первое число
            buf[bytes_read] = '\0';
            
            int n = atoi(buf);
            
            if (n <= 0) {
                printf("Exiting...\n");
                munmap(p, size);
                close(fd);
                close(terminal);
                exit(0);
            }
            
            if (n > num_lines) {
                printf("There are only %d rows!\n", num_lines);
            }
            else {
                // Используем отображенный файл и данные о строках из table
                int length = table[1][n-1];        // Длина строки
                long end_offset = table[0][n-1];   // Позиция после конца строки
                long start_offset = end_offset - length; // Вычисляем начало строки
                
                // Выводим строку, используя p + смещение
                write(STDOUT_FILENO, p + start_offset, length);
            }
        }
    }
}