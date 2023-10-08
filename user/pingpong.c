#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int p1[2]; // parent -> child
    int p2[2]; // child -> parent
    if (pipe(p1) != 0)
    {
        fprintf(2, "open pipe failed");
        exit(1);
    }
    if (pipe(p2) != 0)
    {
        fprintf(2, "open pipe failed");
        exit(1);
    }
    if (fork() == 0)
    {
        char receive = '1';
        // inside the child
        int fd = dup(1);
        close(0);
        dup(p1[0]);
        close(p1[0]);
        close(p1[1]);
        // set up the first pipe
        close(1);
        dup(p2[1]);
        close(p2[1]);
        close(p2[1]);
        read(0, &receive, 1);
        if (receive == '0')
        {
            fprintf(fd, "%d: received ping\n", getpid());
        }
        write(1, &receive, 1);
        close(fd);
        exit(0);
    }
    else
    {
        char receive = '1';
        int fd = dup(1);
        // inside the parent
        close(0);
        dup(p2[0]);
        close(p2[0]);
        close(p2[1]);
        // set up the first pipe
        close(1);
        dup(p1[1]);
        close(p1[1]);
        close(p1[0]);
        write(1, "0", 1);
        read(0, &receive, 1);
        if (receive == '0')
        {
            fprintf(fd, "%d: received pong\n", getpid());
        }
        close(fd);
    }
    exit(0);
}