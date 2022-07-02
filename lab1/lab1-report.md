# Lab1 Report

<center>唐亚周 519021910804</center>

## Basic

1. 使用gcc编译源代码并执行观察输入输出。此处我额外 `#include <string.h>` 来消除 warnings。

   <img src="/home/adswt518/.config/Typora/typora-user-images/image-20220422202819869.png" alt="image-20220422202819869" style="zoom:50%;" />

2. 使用IDA Pro分析可执行文件并使用IDA Pro的hex ray插件(f5)进行反编译

   <img src="/home/adswt518/.config/Typora/typora-user-images/image-20220422202720925.png" alt="image-20220422202720925" style="zoom:50%;" />

3. 分析程序中分析的算法并试解出正确flag。

   反汇编得到的代码中，源代码 `encrypted_text` 由 `s1` 和 `v7` 组合而成。注意，虽然这里 `s1` 的初始长度只有 8，但后面循环中访问了 0~10 位，说明溢出到 `v7` 所在的地址区域了。

   ```c
   *(_QWORD *)s1 = 0x1D1C0B1B1E160D04LL;
   v7 = 1381910;
   ```

   考虑到我的电脑是 x86 架构，应为小端序，因此可以计算得到源代码 `encrypted_text` 的值为 `\x04\x0d\x16\x1e\x1b\x0b\x1c\x1d\x16\x16\x15`。

   GDB 调试查看内存证明了结论正确

   <img src="/home/adswt518/.config/Typora/typora-user-images/image-20220422202023319.png" alt="image-20220422202023319" style="zoom:50%;" />

   然后根据后面的算法，写一个 Python 脚本来求出反汇编代码中的 `s`：

   ```python
   result = [ord(i) for i in "qwertyuiopa"]
   s1 = [ord(i) for i in "\x04\x0d\x16\x1e\x1b\x0b\x1c\x1d\x16\x16\x15"]
   result1 = [(i - 97 + 86) for i in result]
   s = [i ^ j for i, j in zip(result1, s1)]
   s_str = "".join([chr(i) for i in s])
   print(s_str)
   ```

   得到 `s=baLyrevCrsC`。将其输入，发现是正确的。

   <img src="/home/adswt518/.config/Typora/typora-user-images/image-20220422202750651.png" alt="image-20220422202750651" style="zoom: 50%;" />

4. 尝试使用gdb动态分析程序的输入如何被处理。

   在输入完毕后，查看相应位置的内存。可以看到 `0x7fffffffde3d` 到 `0x7fffffffde478` 上存储的是 `encrypted_text`，而`0x7fffffffde49` 到 `0x7fffffffde54` 上存储的是输入的 `flag` 的值。

   <img src="/home/adswt518/.config/Typora/typora-user-images/image-20220422203101151.png" alt="image-20220422203101151" style="zoom:50%;" />

   执行异或操作的循环后，可以看到 `0x7fffffffde3d` 到 `0x7fffffffde478` 上存储的是异或操作的结果，而`0x7fffffffde49` 到 `0x7fffffffde54` 上依然存储着 `flag` 的值。

   <img src="/home/adswt518/.config/Typora/typora-user-images/image-20220422203854188.png" alt="image-20220422203854188" style="zoom:50%;" />

   执行完接下来的操作循环后，可以看到 `0x7fffffffde3d` 到 `0x7fffffffde478` 上存储的是最终的结果，而`0x7fffffffde49` 到 `0x7fffffffde54` 上依然存储着 `flag` 的值。

   <img src="/home/adswt518/.config/Typora/typora-user-images/image-20220422204414188.png" alt="image-20220422204414188" style="zoom:50%;" />

   最终将该结果与 `qwertyuiopa` 进行比较，并做出对应的输出。

## Challenge

> 这里有一个PDF文件加密程序chall，把秘密文件加密成了pdf.enc，能否将加密文件还原呢？
>
> 逆向分析chall文件并将pdf.enc文件解密。
>
> （chall1和chall2是不同架构的相同程序，选一个分析即可。arm架构如需动态调试需要了解qemu模拟）

### 前期准备

首先查看两个文件的架构，发现 chall1 是 x86 而 chall2 是 arm，因此这里我选择 chall1 进行分析。

<img src="/home/adswt518/.config/Typora/typora-user-images/image-20220422205320570.png" alt="image-20220422205320570" style="zoom: 33%;" />

### 反编译[^1][^2][^3]

1. `main` 函数

   <img src="/home/adswt518/.config/Typora/typora-user-images/image-20220422204920517.png" alt="image-20220422204920517" style="zoom:50%;" />

2.  `encrypt_file` 函数

   <img src="/home/adswt518/.config/Typora/typora-user-images/image-20220422205049556.png" alt="image-20220422205049556" style="zoom:50%;" />

3. `seed` 函数

   <img src="/home/adswt518/.config/Typora/typora-user-images/image-20220422211900050.png" alt="image-20220422211900050" style="zoom: 50%;" />

4. `file_size` 函数

   <img src="/home/adswt518/.config/Typora/typora-user-images/image-20220428170855033.png" alt="image-20220428170855033" style="zoom:50%;" />

4. `rand` 函数

   <img src="/home/adswt518/.config/Typora/typora-user-images/image-20220422212834797.png" alt="image-20220422212834797" style="zoom:50%;" />

### 分析

1. `seed` 函数将该文件的前 8 bytes 存储为 `RandSeed`。查找资料得知，PDF 文件的前 8 bytes 为 `%PDF=1.7` 形式（这里 `1.7` 是一个例子，表示版本号）。
2. `rand` 函数使用 `RandSeed` 进行一系列计算后返回一个伪随机数，同时更新 `RandSeed` 的值。
2. `file_size` 函数获得该文档的大小，单位为 byte。
3. `encrypt_file` 函数将原文件的每一个 byte 与 `rand` 函数返回值进行异或操作，再将其输出到加密后的文件。

### 思路

首先确定该 PDF 文件的版本号：对每个版本号对应的前 8 位字符串 `FirstEight` 进行测试，如果某个满足以下条件，则代表找到了正确的版本号。此处的 `FirstEightEnc` 代表加密后的前 8 位字符串，可以从 `pdf.enc` 文件中读取。

```pseudocode
For i in [0..7]
	FirstEightEnc[i] == FirstEightEnc[i] ^ rand()
```

然后根据版本号来算出每位加密用的 `rand()` 函数返回值，再将加密后的每位都与其对应的 `rand()` 函数返回值进行异或，即得到原文件。

### 解密程序

在实现上遇到了一些困难，因此没能实现，非常抱歉。

## 感悟

我对 C 语言的类型转换实在是掌握得不熟练，需要多加练习。

[^1]: https://reverseengineering.stackexchange.com/questions/8296/whats-the-function-of-lodword-and-hidword
[^2]: https://reverseengineering.stackexchange.com/questions/8317/what-does-pair-mean-in-hex-rays-decompiler-output
[^3]: https://wlaport.top/archives/231

