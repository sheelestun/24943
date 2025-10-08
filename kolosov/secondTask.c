#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(void) {
    // Задаём таймзону Калифорнии.
    // Лучше IANA-имя (America/Los_Angeles), чем "PST8PDT".
    if (setenv("TZ", "America/Los_Angeles", 1) != 0) {
        // если setenv недоступен/неудачен, можно так:
        putenv("TZ=America/Los_Angeles");
    }
    tzset();

    time_t now = time(NULL);
    struct tm *pt = localtime(&now);
    if (!pt) return 1;

    char buf[64];
    pt->tm_hour -=1;
    // Пример формата: 2025-10-07 12:34:56 PDT
    if (strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S %Z", pt) == 0) return 1;

    printf("California time: %s\n", buf);
    return 0;
}
