#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include<iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")  //加载ws2_32.lib库

int main(int argc, char* argv[])
{
    //加载套接字
    WORD sockVersion = MAKEWORD(2, 2);//指定socket版本2.2
    WSADATA wsaData;//存储被WSAStartup函数调用后返回的Windows Sockets数据
    if (WSAStartup(sockVersion, &wsaData) != 0) return 0;

    //创建服务端socket,三个参数分别代表IP地址族，流式套接字，TCP协议
    SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (slisten == INVALID_SOCKET)
    {
        printf("socket error!");
        return 0;
    }

    //绑定服务端ip和端口
    sockaddr_in sin;//存放了地址族、端口、ip地址，用来处理通信地址
    sin.sin_family = AF_INET;//ip地址族
    sin.sin_port = htons(8888);//转换网络编码,侦听本地8888端口
    sin.sin_addr.S_un.S_addr = INADDR_ANY;//接收任意地址的连接
    if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
    {
        printf("bind error!");
    }

    //开始侦听
    if (listen(slisten, 5) == SOCKET_ERROR)
    {
        printf("listen error!");
        return 0;
    }

    SOCKET sClient;//创建客户端socket
    sockaddr_in remoteAddr;//客户端的地址信息
    int nAddrlen = sizeof(remoteAddr);
    char revData[256];//接收数据缓冲区

    //循环接收数据
    while (1)
    {
        printf("waiting...\n");
        //等待连接，返回客户端socket
        sClient = accept(slisten, (SOCKADDR*)&remoteAddr, &nAddrlen);
        if (sClient == INVALID_SOCKET)
        {
            printf("accept error!");
            continue;
        }
        printf("Connection successful:%s\n", inet_ntoa(remoteAddr.sin_addr));

        //接收数据
        int ret = recv(sClient, revData, 255, 0);//若无错误，返回读入字节数
        if (ret > 0)
        {
            std::cout << ret << std::endl;
            revData[ret] = '\0';
            printf("%s\n", revData);
        }

        //发送数据
        char sendData[255];
        gets_s(sendData);
        send(sClient, sendData, strlen(sendData), 0);

        closesocket(sClient);
    }

    closesocket(slisten);//关闭服务端socket
    WSACleanup();
    return 0;
}