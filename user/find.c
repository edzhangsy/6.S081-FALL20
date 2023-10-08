#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

#define PATH_LEN 512
char *extract_name(char *path)
{
    char *p;
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
    {
        continue;
    }
    if (p == path)
    {
        return path;
    }
    p++;
    return p;
}

void find(char *path, char *name)
{
    // fprintf(1, "%s\n", path);
    int fd;
    struct dirent de;
    struct stat st;
    char *p;

    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type)
    {
    case T_FILE:
        if (strcmp(name, extract_name(path)) == 0)
        {
            fprintf(1, "%s\n", path);
        }
        break;

    case T_DIR:
        if (strlen(path) + 1 + DIRSIZ + 1 > PATH_LEN)
        {
            printf("find: path too long\n");
            break;
        }
        p = path + strlen(path);
        *p = '/';
        p++;
        *p = '\0';
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0)
            {
                continue;
            }
            if (strcmp(".", de.name) == 0)
            {
                continue;
            }
            if (strcmp("..", de.name) == 0)
            {
                continue;
            }
            strcpy(p, de.name);
            if (stat(path, &st) < 0)
            {
                printf("find: cannot stat %s\n", path);
                continue;
            }
            find(path, name);
            *p = '\0';
        }
        break;
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc <= 2)
    {
        fprintf(1, "The usage is find path name:\n");
        exit(0);
    }
    if (argc > 3)
    {
        fprintf(1, "The usage is find path name:\n");
        exit(0);
    }
    char buf[512];
    if (strlen(argv[1]) > PATH_LEN)
    {
        fprintf(1, "The input path is too long\n");
        exit(0);
    }
    strcpy(buf, argv[1]);
    find(buf, argv[2]);
    exit(0);
}