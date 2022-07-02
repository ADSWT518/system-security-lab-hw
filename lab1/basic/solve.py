result = [ord(i) for i in "qwertyuiopa"]
s1 = [ord(i) for i in "\x04\x0d\x16\x1e\x1b\x0b\x1c\x1d\x16\x16\x15"]
result1 = [(i - 97 + 86) for i in result]
s = [i ^ j for i, j in zip(result1, s1)]
s_str = "".join([chr(i) for i in s])
print(s_str)
