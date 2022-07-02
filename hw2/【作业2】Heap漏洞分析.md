# 【作业2】Heap漏洞分析

<center>唐亚周 519021910804</center>

> 设计一个含有double free 或 UAF 漏洞的程序，攻击该程序，并使用调试工具解析攻击原理。

## 设计程序

这里我参考了网上的博客[^1]和老师 PPT 上的示例代码，设计了如下的程序。

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void (*func_ptr)(char*);

void evil_fuc(char command[]) { system(command); }

void echo(char content[]) { printf("%s", content); }

func_ptr* echoFunc;
func_ptr* evilFun;

int main(int argc, char** argv)
{
    char line[128];

    while (1) {
        printf("[ echoFunc = %p, evilFun = %p ]\n", echoFunc, evilFun);
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }

        if (strncmp(line, "setEcho", 7) == 0) {
            echoFunc = (func_ptr*)malloc(sizeof(func_ptr));
            memset(echoFunc, 0, sizeof(func_ptr));
            *echoFunc = echo;
        }
        if (strncmp(line, "setEvil", 7) == 0) {
            evilFun = (func_ptr*)malloc(sizeof(func_ptr));
            memset(evilFun, 0, sizeof(func_ptr));
            *evilFun = evil_fuc;
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
```

## 攻击漏洞

可以通过以下两种方式来攻击。

1. 依次输出如下指令，预计会输出 `echoFunc is NULL`，实际上会产生 segmentation fault。

   ```
   setEcho
   callEcho hello world
   delEcho
   callEcho hello again
   ```

   运行结果如下：

   <img src="/home/adswt518/.config/Typora/typora-user-images/image-20220421220948907.png" alt="image-20220421220948907" style="zoom:50%;" />

2. 依次输出如下指令，预计会输出 `echoFunc is NULL`，实际上会运行 `system("/bin/sh")`。

   ```
   setEcho
   callEcho hello world
   delEcho
   setEvil
   callEcho /bin/sh
   ```

   运行结果如下：

   <img src="/home/adswt518/.config/Typora/typora-user-images/image-20220421221013060.png" alt="image-20220421221013060" style="zoom:50%;" />

## 分析攻击原理

这里我使用 GDB 进行分析，具体分析步骤参考了课上播放的视频。[^2]

1. 首先在 GDB 中运行一次程序，再将其反汇编以得到正确的逻辑地址。[^3] 然后在原程序 19 行的 `printf` 语句对应的地址 `0x0000555555555245` 上打上断点。

   <img src="/home/adswt518/.config/Typora/typora-user-images/image-20220422110010993.png" alt="image-20220422110010993" style="zoom: 33%;" />

   <img src="/home/adswt518/.config/Typora/typora-user-images/image-20220422112153137.png" alt="image-20220422112153137" style="zoom:50%;" />

2. 设置执行到该断点时的指令，用于调试。之所以从 `0x555555559ab0` 开始查看堆的信息，是因为在之前运行时，我发现第一次分配的堆地址为 `0x555555559ac0`。

   <img src="/home/adswt518/.config/Typora/typora-user-images/image-20220422112316987.png" alt="image-20220422112316987" style="zoom:40%;" />

3. 不断 `continue` 进行调试。

   1. 第一种攻击方式：

      1. 首先 `setEcho`，可以看到在堆上 `0x555555559ac0` 处的值为 `0x5555555551d4`，即函数 `echo` 在内存上的地址。

         <img src="/home/adswt518/.config/Typora/typora-user-images/image-20220422112859372.png" alt="image-20220422112859372" style="zoom:33%;" />

      2. 然后 `callEcho hello world`，正常输出了 `hello world`。

         <img src="/home/adswt518/.config/Typora/typora-user-images/image-20220422112839936.png" alt="image-20220422112839936" style="zoom:33%;" />

      3. 然后 `delEcho`，可以看到原属于 `echoFunc` 的内存地址上变成了别的值，但 `echoFunc` 仍然指向该地址。

         <img src="/home/adswt518/.config/Typora/typora-user-images/image-20220422113008545.png" alt="image-20220422113008545" style="zoom:33%;" />

      4. 最后再一次 `callEcho hello again`，由于该地址指向的值已经不是原来的函数，甚至不是一个可调用的函数，因此产生 `SIGSEGV` 错误。

   2. 第二种攻击方式：

      1. 前三步与第一种攻击方式相同。

      2. 然后 `setEvil`，可以看到，原属于 `echoFunc` 的地址现在变成了 `evilFunc` 的地址，上面指向的值也变成了函数 `evil_fun` 的地址。

         <img src="/home/adswt518/.config/Typora/typora-user-images/image-20220422113546468.png" alt="image-20220422113546468" style="zoom:33%;" />

      3. 最后 `callEcho /bin/sh`，由于该地址指向的值已经不是原来的函数 `echo`，而是新的函数 `evil_fun`，因此会调用该函数，运行 `/bin/sh`。

[^1]: https://blog.csdn.net/qq_31481187/article/details/73612451
[^2]: https://youtu.be/ZHghwsTRyzQ
[^3]: https://blog.csdn.net/Dontla/article/details/117562006
