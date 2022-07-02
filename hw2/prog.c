#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void (*func_ptr)(char*);

void evil_fuc(char command[]) { system(command); }

void echo(char content[]) { printf("%s", content); }

func_ptr* echoFunc;
func_ptr* evilFunc;

int main(int argc, char** argv)
{
    char line[128];
    
    while (1) {
        printf("[ echoFunc = %p, evilFunc = %p ]\n", echoFunc, evilFunc);
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }

        if (strncmp(line, "setEcho", 7) == 0) {
            echoFunc = (func_ptr*)malloc(sizeof(func_ptr));
            memset(echoFunc, 0, sizeof(func_ptr));
            *echoFunc = echo;
        }
        if (strncmp(line, "setEvil", 7) == 0) {
            evilFunc = (func_ptr*)malloc(sizeof(func_ptr));
            memset(evilFunc, 0, sizeof(func_ptr));
            *evilFunc = evil_fuc;
        }
        if (strncmp(line, "delEcho", 7) == 0) {
            free(echoFunc);
        }
        if (strncmp(line, "callEcho ", 9) == 0) {
            if (echoFunc) {
                (*echoFunc)(line + 9);
            } else {
                printf("echoFunc is NULL\n");
            }
        }
    }
}

/*
1. 预计会输出echoFunc is NULL，实际上segmentation fault
setEcho
callEcho hello world
delEcho
callEcho hello again

2. 预计会输出echoFunc is NULL，实际上调用了evil_fuc
setEcho
callEcho hello world
delEcho
setEvil
callEcho /bin/sh
*/