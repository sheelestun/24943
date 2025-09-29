#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

extern char *tzname[];
int main(void)
{
    time_t now;
    struct tm *sp;

    setenv("TZ", "PST8PST", 1);
    tzset();
    (void) time( &now );
    sp = localtime(&now);

    printf("%d/%d/%02d %d:%02d %s\n",
        sp->tm_mon + 1, sp->tm_mday,
        sp->tm_year + 1900, sp->tm_hour,
        sp->tm_min, tzname[sp->tm_isdst]);

    
    //Если учитывать подсказку, то, видимо, хотели так:
    sp = gmtime(&now);
    sp->tm_hour -= 8;
    mktime(sp);
    

    printf("%d/%d/%02d %d:%02d\n",
        sp->tm_mon + 1, sp->tm_mday,
        sp->tm_year + 1900, sp->tm_hour,
        sp->tm_min);
    exit(0);
}