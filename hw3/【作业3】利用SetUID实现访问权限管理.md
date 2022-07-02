# 【作业3】利用SetUID实现访问权限管理

<center>唐亚周 519021910804</center>

> 你在为一家审计代理处工作，调查一家公司是否存在诈骗行为。为了这个目的，你需要阅读这家公司在Unix系统中的所有文件。公司系统管理者为了保护系统的可靠性，必须保证审计者可以阅读系统中所有的文件，但不能改写或删除文件。
> 利用SetUID特性，系统管理员如何实现该诉求？
>
> 提示：审计员在Unix系统下使用cat命令查看公司文件内容

## 代码

我在查阅资料[^1]后，编写程序如下：

```c
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    char *v[3];
    if(argc < 2) {
        printf("Please type a file name.\n");
        return 1;
    }
    v[0] = "/bin/cat"; v[1] = argv[1]; v[2] = NULL;

    execve(v[0], v, NULL);

    return 0 ;
}
```

## 测试

首先将上述程序编译，然后将其所有者设置为 root 用户，权限设置为 4755。这样它就成了一个 set-uid 程序。当其他用户执行它时，可以获得 root 用户的权限。

<img src="/home/adswt518/.config/Typora/typora-user-images/image-20220504171617740.png" alt="image-20220504171617740" style="zoom:50%;" />

然后我们使用 root 用户在 **/root 目录下** 创建一个文件 `file.txt`，将其内容设置为“hello world”，并将其权限设置为 700，即 root 用户拥有全部权限，但普通用户任何权限都没有。注意不能在普通用户的 home 目录创建该文件，否则即是设置为无法删除，普通用户依然能够删除该文件。

<img src="/home/adswt518/.config/Typora/typora-user-images/image-20220504174002309.png" alt="image-20220504174002309" style="zoom:50%;" />

然后我们切换到普通用户，尝试读取该文件，发现没有权限。但如果我们使用刚刚编写的 set-uid 程序读取该文件，则发现可以读取。同时普通用户尽管使用该 set-uid 程序，也不能对该文件进行修改或删除。

<img src="/home/adswt518/.config/Typora/typora-user-images/image-20220504174220338.png" alt="image-20220504174220338" style="zoom:50%;" />

[^1]: https://blog.csdn.net/qq_51927659/article/details/122765563

