#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(2, "Usage: sleep number \n");
        exit(1);
    }
    if (sleep(atoi(argv[1])) < 0)
    {
        fprintf(2, "sleep %s failed\n", argv[1]);
    }
    exit(0);
}