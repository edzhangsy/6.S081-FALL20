#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void operation(int fd_screen)
{
    int n;
    int ret = read(0, &n, sizeof(int));
    // assume the read is set by the parent
    // there are no more numbers, return of the recursion
    if (ret == 0)
    {
        close(0);
        close(1);
        exit(0);
    }
    fprintf(fd_screen, "the pid is %d\n", getpid());
    fprintf(fd_screen, "prime %d\n", n);
    // print the initial prime passed from right
    // set up the pipeliens
    int p[2];
    if (pipe(p) != 0)
    {
        fprintf(2, "pipe open failed");
        close(0);
        close(1);
        exit(1);
    }
    int tmp = 0;  // tmp stores value passed from right side
    int flag = 0; // flag for the fisrt prime encountered
    int f = 0;    // child pid
    while (read(0, &tmp, sizeof(tmp)))
    {
        // fprintf(fd_screen, "the pid is %d, tmp is %d\n", getpid(), tmp);
        if (tmp % n == 0)
        {
            continue;
        }
        if (flag == 0)
        {
            // create child
            f = fork();
            if (f == 0)
            {
                close(0);
                dup(p[0]);
                close(p[0]);
                close(p[1]);
                operation(fd_screen);
            }
            close(1);
            dup(p[1]);
            close(p[0]);
            close(p[1]);
            flag = 1;
        }
        if (write(1, &tmp, sizeof(int)) == 0)
        {
            break;
        }
    }
    close(1);
    close(0);
    wait(0);
    exit(0);
}
int main(int argc, char *argv[])
{
    int p[2];
    int fd_screen = dup(1);
    pipe(p);
    if (fork() == 0)
    {
        close(0);
        dup(p[0]);
        close(p[0]);
        close(p[1]);
        operation(fd_screen);
    }
    close(1);
    dup(p[1]);
    close(p[0]);
    close(p[1]);
    int i = 2;
    for (; i <= 35; i++)
    {
        write(1, &i, sizeof(int));
    }
    close(1);
    close(0);
    wait(0);
    exit(0);
}