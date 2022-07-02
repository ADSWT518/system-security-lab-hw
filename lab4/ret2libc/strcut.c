#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 0x40

struct stupid_global
{
    char buffer[BUFFER_SIZE];
    void *system_addr;
}__attribute__ ((aligned (1))); 

struct stupid_global global;

void Suggestion()
{
    puts("Any suggestion...");
    char buf;
    gets(&buf);
}

int main()
{
    char local_buffer[0x40];
    int length = 0, start = 0, end = -1;
    puts("######string cutter######");
    puts("String length:");
    scanf("%d", &length);
    global.system_addr = *(unsigned int *)(*(unsigned int *)((char *)scanf + 2));

    if(length <= 0 || BUFFER_SIZE < length){
        puts("Invalid length!");
        length = BUFFER_SIZE;
    }

    puts("String:");
    for(int i = 0; i < length + 1; i++){
        char temp = getchar();
        global.buffer[i] = temp;
    }
    //getchar();//swallow 0xa
    puts("START and END of the string:");
    scanf("%d%d", &start, &end);
    getchar();

    if(start <= end){
        if(start <= 0){
            puts("Invalid START! WILL USE DEFAULT VALUE");
            start = start;
        }
        if(length < end){
            puts("Invalid END! WILL USE DEFAULT VALUE");
            end = length;
        }
        if(start == end || (end - start) == length){
            puts("Are you kidding me? I will do nothing for such range");
            return 0;
        }
    }
    else{
        int it = start;
        start = end;
        end = it;
    }

    for(int i = start; i < end; i++){
        write(1, &global.buffer[i], 1);
    }
    puts("");
    Suggestion();
    return 0;
    
}
