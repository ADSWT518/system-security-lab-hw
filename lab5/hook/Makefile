roshambo: roshambo.c evil_libc.so evil_libc_rand.so
	gcc -o roshambo roshambo.c

evil_libc.so: evil_libc.c
	gcc -fPIC -shared -o evil_libc.so evil_libc.c -ldl

evil_libc_rand.so: evil_libc_rand.c
	gcc -fPIC -shared -o evil_libc_rand.so evil_libc_rand.c -ldl