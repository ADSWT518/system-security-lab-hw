#include <stdio.h>
#include <string.h>

int main(int argc, char** argv, char** env)
{
    char encrypted_text[] = "\x04\x0d\x16\x1e\x1b\x0b\x1c\x1d\x16\x16\x15";
    char flag[15];
    memset(flag, 0, 15);
    puts("please input the flag:");
    scanf("%11s", flag);
    for (int i = 0; i < 11; i++) {
        encrypted_text[i] ^= flag[i];
    }
    for (int i = 0; i < 11; i++) {
        encrypted_text[i] = (((encrypted_text[i] - 0x61) + 11) % 26) + 0x61;
    }
    if (strcmp(encrypted_text, "qwertyuiopa") == 0) {
        puts("flag is correct!");
    } else {
        puts("wrong!");
    }
    return 0;
}