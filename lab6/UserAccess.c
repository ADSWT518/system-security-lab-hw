#include <stdio.h>

int main()
{
    char *kernel_data_addr = (char*)0xfb4bd0c0; // 替换为 secret 的地址
    char kernel_data = *kernel_data_addr;
    printf("I have reached here.\n");
    return 0;
}