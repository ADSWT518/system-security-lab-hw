#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include <time.h>

typedef void(*SRAND)(unsigned int seed);

#define EVILSEED 0xdeadbeef

int srand(const char *s1, const char *s2)
{
    static void *handle = NULL;
    static SRAND old_srand = NULL;

    if( !handle )
    {
        handle = dlopen("libc.so.6", RTLD_LAZY);
        old_srand = (SRAND)dlsym(handle, "srand");
    }
    printf("hack function invoked. Seed going to be : 0x%X\n", EVILSEED);
    old_srand(EVILSEED);
}
