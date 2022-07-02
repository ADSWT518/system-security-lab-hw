#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
int main(int argc,char **argv,char **env)
{
    setvbuf(stdout, 0, 2, 0);
    char *secret = malloc(200);
    char *name[10];
    printf("please input your secret:");
    read(0,secret,200);
    printf("your secret has been safely stored in %p",secret);
    printf("what's your name:");
    read(0,name,200);

    return 1;
}
