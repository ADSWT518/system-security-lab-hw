#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NOTICE "[R]ock-[P]aper-[S]cissors!"
#define DRW 0
#define LOS 1
#define WIN 2
#define LONGGEST_ITEM "Scissors"

char RPS_result[3][3] = {DRW, LOS, WIN, WIN, DRW, LOS, LOS, WIN, DRW};
char RPS_item[3][sizeof(LONGGEST_ITEM)]= {"Rock", "Paper", "Scissors"};

char round(char player, char npc)
{
    printf("Your %s vs. npc's %s\n", RPS_item[player], RPS_item[npc]);
    return RPS_result[player][npc];
}

char play(char X)
{
    unsigned char choice = -1;
    switch(X){
        case 'R':
        choice = 0;
        break;
        case 'P':
        choice = 1;
        break;
        case 'S':
        choice = 2;
        break;
        default:
        return -1;
    }

    return round(choice, rand()%3);
}

int main(int argc, char *argv[])
{
    char player_input = -1;
    int count = 0;

    srand(time(0));

    puts(NOTICE);
    while((player_input = getchar())!=EOF){
        if(player_input == '\n')
            continue;

        char res = play(player_input);
        
        if(res < 0){
            puts("[-]Bad input\n");
            printf("%x\n", player_input);
            return 0;
        }
        else if(res == 1){
            puts("GM:u lose\n");
            count = 0;
        }
        else if(res == 0){
            puts("GM:draw game\n");
            count = 0;
        }
        else if(res == 2){
            puts("GM:u win\n");
            count += 1;
        }

        if(0x100 <= count){
            puts("GM: WINNER WINNER CHICKEN DINNER!\n");
            return 0;
        }

        puts(NOTICE);
    }


    return 0;
}
