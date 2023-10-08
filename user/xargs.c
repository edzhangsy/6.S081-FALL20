#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

static char *args[MAXARG];
static char buf[512];

int readline(int fd)
{
    memset(buf, 0, sizeof(buf));
    char tmp = '0';
    char *p = buf;
    while (read(fd, &tmp, 1))
    {
        if (tmp == '\n')
        {
            goto exit;
        }
        *p = tmp;
        p++;
    }
    if (p == buf)
    {
        return 0;
    }
exit:
    p = '\0';
    return 1;
}

int main(int argc, char *argv[])
{
    if (argc <= 2)
    {
        fprintf(2, "arguments are too few\n");
        exit(1);
    }
    if (argc > MAXARG)
    {
        fprintf(2, "arguments are too much\n");
        exit(1);
    }
    while (readline(0))
    {
        memset(args, 0, sizeof(args));
        for (int i = 1; i < argc; i++)
        {
            args[i - 1] = argv[i];
        }
        args[argc - 1] = buf;
        if (fork() == 0)
        {
            exec(argv[1], args);
        }
        wait(0);
    }
    exit(0);
}