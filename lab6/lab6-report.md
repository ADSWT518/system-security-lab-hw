# Lab6 Report

<center>唐亚周 519021910804</center>

## Q1

> 完成上述实验，尝试输出secret中的第四个字符(D)；

### 任务一：从内存和CPU缓存中读取数据

运行结果如图所示。多次测试后，我将任务二中的阈值设置为 200。

<img src="/home/adswt518/.config/Typora/typora-user-images/image-20220608203527957.png" alt="image-20220608203527957" style="zoom:50%;" />

### 任务二：将CPU缓存作为SideChannel

运行结果如图所示。可以看到，几乎每次都成功判断到了 Cache Hit。

<img src="/home/adswt518/.config/Typora/typora-user-images/image-20220608210522345.png" alt="image-20220608210522345" style="zoom:50%;" />

### 任务三：熔断攻击

#### step 1：将secret注入内核

运行结果如图所示。可以看到 secret 被成功注入到内核。

<img src="/home/adswt518/.config/Typora/typora-user-images/image-20220608211509073.png" alt="image-20220608211509073" style="zoom:50%;" />

之后尝试直接读取 secret，代码如下。[^1]

```c
#include <stdio.h>

int main()
{
    char *kernel_data_addr = (char*)0xfb4bd0c0; // 替换为 secret 的地址
    char kernel_data = *kernel_data_addr;
    printf("I have reached here.\n");
    return 0;
}
```

运行结果如下，产生了 segmentation fault。

<img src="/home/adswt518/.config/Typora/typora-user-images/image-20220608212257106.png" alt="image-20220608212257106" style="zoom:50%;" />

#### step 2：实施攻击

运行结果如下，可以看到攻击失败了。

<img src="/home/adswt518/.config/Typora/typora-user-images/image-20220608212838663.png" alt="image-20220608212838663" style="zoom:50%;" />

检查我的电脑的 CPU 是否存在 Meltdown 的 bug[^2]，发现并不存在。我的电脑的 CPU 为 11th Gen Intel i7-11800H，应该已经修复了该 bug。

<img src="/home/adswt518/.config/Typora/typora-user-images/image-20220609214357333.png" alt="image-20220609214357333" style="zoom:50%;" />

## Q2

> 解释使用UserAccess读取数据为何会产生Segmentation Fault；

答：因为 secret 在内核区域的内存上，不可以从用户空间直接访问。

## Q3

> 解释实验中array的有效数字之间存在较大的间隔，为何在任务二和任务三中还需要增加一个偏移量Delta；

答[^1]：因为 `array` 数组所在的内存的相邻区域上可能有其它的变量，当某个在 `array` 的“前面”的变量被缓存时，`array` 的前几个位置也可能因为处于同一个缓存块中，也一起被读入缓存中。因此我们不能使用 `array` 的前几个位置来进行测试。也就是说，如果我们不增加偏移量 Delta，那么测试 `array[0*4096]` 时，它可能已经被缓存了，这就造成了实验结果不准确。

而为了保证测试的一致性，我们就在每次取测试点时，都加上一个偏移量 Delta，因此我们使用 `array[k*4096+DELTA]` 作为测试点。

## Q4

> 解释`meltdown_asm`中汇编指令的作用。（选做）

答[^1]：这段汇编指令执行了 400 次将 `0x141` 这个值加到 `eax` 寄存器的行为。这实际上是无效的计算，但它的目的是通过占用 CPU 中的 ALU 来增加判断访问异常的时间，从而提高攻击的成功率。

[^1]: https://seedsecuritylabs.org/Labs_16.04/PDF/Meltdown_Attack.pdf
[^2]: https://xuanxuanblingbling.github.io/ctf/pwn/2020/06/30/meltdown/
