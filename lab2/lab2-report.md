# Lab2 Report

<center>唐亚周 519021910804</center>

## Basic

1. 执行可执行程序并按照如下图所示输入。

   <img src="/home/adswt518/.config/Typora/typora-user-images/image-20220505105041500.png" alt="image-20220505105041500" style="zoom:50%;" />

2. 分析源代码，定位漏洞的位置并解释漏洞成因。

   在输入了 `%4$d` 之后，`welcome` 的值变成 `welcome,%4$d`，这里的百分号会被 `prinf` 当作字符串格式化的标志，代表取第 4 个参数并以十进制数的格式输出。但这里实际上 `printf` 是没有参数的，因此它会在栈上继续寻找第 4 个参数。而我们想要的 `secret` 值就存储在栈空间中，因此这里的“4”是后续通过栈空间分析来确定的。

3. 使用GDB调试，观察程序运行中printf的参数并解释上图所示中的输入所代表的意思。

   <img src="/home/adswt518/.config/Typora/typora-user-images/image-20220505212828343.png" alt="image-20220505212828343" style="zoom:25%;" />

   在程序的前面一部分中，`secret` 的值被存在了 `0xffffd040` 的地址上。而通过观察调用 `printf` 函数时的栈空间，我们可以发现，  `0xffffd040` 的地址正好对应着 `printf` 的第 4 个参数，因此当我们的输入为 `%4$d` 时，程序就会把 `secret` 给输出，然后我们再次输出该 `secret` 值，就能够成功地攻击该程序了。

4. 请列举其他格式化字符串可能带来的信息泄漏。

   这一题我参考了网上的博客。[^1]

   1. 可以查看任意地址的内存上的内容。当我们把想要查看的地址写入到栈中后，就可以利用 `%s` 来查看它对应的内容。
   2. 可以修改栈的内容。可以利用 `%n` 转换指示符将 `%n` 当前已经成功写入流或缓冲区中的字符个数存储到地址由参数指定的整数中。
   3. 可以修改任意地址的内存上的内容。可以利用单个字符作为占位符，结合 `%n` 来进行逐字节的修改。

## Challange

暂时没有想到较好的思路。

## 感悟

在分析栈空间时，我一度把 `0xffffd034` 当成了传入 `printf` 的字符串，而不是第 1 个参数，导致我一直觉得 `secret` 是第 3 个参数 :sob: 最终在与杨钦崴同学的交流中发现了这个问题，感谢他！

[^1]:https://firmianay.gitbooks.io/ctf-all-in-one/content/doc/3.1.1_format_string.html
