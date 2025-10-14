#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char *s;
    struct Node *next;
} Node;

int main(void) {
    enum { MAX = 4096 };
    char buf[MAX];

    Node *head = NULL, *tail = NULL;

    puts("Введите строки. Точка в начале строки завершает ввод.");
    while (fgets(buf, sizeof(buf), stdin)) {
        if (buf[0] == '.') break;                // точка в начале — конец ввода

        size_t len = strlen(buf);
        if (len && buf[len-1] == '\n') buf[--len] = '\0';  // убираем '\n'

        // выделяем ровно len+1 байт и копируем строку
        char *copy = (char *)malloc(len + 1);
        if (!copy) { perror("malloc"); return 1; }
        memcpy(copy, buf, len + 1);

        // создаём узел
        Node *node = (Node *)malloc(sizeof(Node));
        if (!node) { perror("malloc"); free(copy); return 1; }
        node->s = copy;
        node->next = NULL;

        // вставка в конец списка
        if (!head) head = tail = node;
        else { tail->next = node; tail = node; }
    }

    // вывод всех строк
    puts("\nСодержимое списка:");
    for (Node *p = head; p; p = p->next) {
        puts(p->s);
    }

    // освобождение памяти
    while (head) {
        Node *n = head->next;
        free(head->s);
        free(head);
        head = n;
    }
    return 0;
}
