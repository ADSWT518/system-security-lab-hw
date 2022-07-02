#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
int main()
{
    int secret;
    int guess;
    char name[10];
    char welcome[20] = "welcome,";
    int fd;
    
    fd = open("/dev/urandom",0);
    read(fd,&secret,4);
    puts("what's your name?");
    scanf("%5s",name);
    strcat(welcome,name);
    printf(welcome);
    puts("guess what is the secret:");
    scanf("%d",&guess);
    if(secret == guess)
        puts("you win!");
    else
        puts("you lost!");
}