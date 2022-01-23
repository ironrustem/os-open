#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include <time.h>

void getTime(){
    struct timespec mt1;
    clock_gettime (CLOCK_REALTIME, &mt1);
    long int s_time;
    struct tm *m_time;
    char str_t[128]="";
    s_time = time (NULL);
    m_time = localtime (&s_time);
    strftime (str_t, 128, "Время: %T", m_time);
    printf ("%s",str_t);
    printf (":%.3ld\n", mt1.tv_nsec / 1000000);
}

int main(int argc, char const *argv[]) {
    pid_t pid; /* идентификатор процесса */
    pid_t pid1; /* идентификатор процесса */
    pid = fork (); /* создание нового процесса */

    if (pid == 0) {
        printf ("Это Дочерний процесс, его pid=%d\n", getpid());
        printf ("А pid его Родительского процесса=%d\n", getppid());
        getTime();

    }
    else if (pid > 0) {
        printf ("Это Родительский процесс pid=%d\n", getpid());
        system("ps -x");
        getTime();
        pid1 = fork();
        if (pid1 == 0) {
            printf ("Это Дочерний процесс, его pid=%d\n", getpid());
            printf ("А pid его Родительского процесса=%d\n", getppid());
            getTime();
        }
    }
    return 0;
}
