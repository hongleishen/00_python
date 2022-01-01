1.去掉两端字符串： strip(), rstrip(),lstrip()

1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
#!/usr/bin/python3
 
s  =  '  -----abc123++++       '
 
# 删除两边空字符
print (s.strip())
 
# 删除左边空字符
print (s.rstrip())
 
# 删除右边空字符
print (s.lstrip())
 
# 删除两边 - + 和空字符
print (s.strip().strip( '-+' ))
2.删除单个固定位置字符： 切片 + 拼接

1
2
3
4
5
6
#!/usr/bin/python3
 
s  =  'abc:123'
# 字符串拼接方式去除冒号
new_s  =  s[: 3 ]  +  s[ 4 :]
print (new_s)
3.删除任意位置字符同时删除多种不同字符：replace(), re.sub()

1
2
3
4
5
6
7
8
9
10
11
#!/usr/bin/python3
 
# 去除字符串中相同的字符
s  =  '\tabc\t123\tisk'
print (s.replace( '\t' , ''))
 
 
import  re
# 去除\r\n\t字符
s  =  '\r\nabc\t123\nxyz'
print (re.sub( '[\r\n\t]' , '', s))
————————————————
版权声明：本文为CSDN博主「waitstory12」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/waitstory12/article/details/79885445