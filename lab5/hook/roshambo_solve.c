#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define NOTICE "roshambo solver (q for quit)"
#define EVILSEED 0xdeadbeef

char play()
{
    unsigned char choice = -1;
    switch (rand()%3)
    {
        case 0:
            choice = 'P';
            break;
        case 1:
            choice = 'S';
            break;
        case 2:
            choice = 'R';
            break;
    }

    return choice;
}

int main(int argc, char *argv[])
{
    char player_input = -1;
    int count = 0;

    srand(EVILSEED);
    puts(NOTICE);
    while((player_input = getchar())!='q'){
        char res = play();
        printf("%c", res);
    }
    return 0;
}
