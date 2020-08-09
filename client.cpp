#include <stdlib.h>
#include <stdio.h>
#include <WinSock2.h>
#include <iostream>
#include <WS2tcpip.h>
#include <cstring>
#include<thread>
#include<windows.h>
#include <sstream>
#pragma comment (lib,"ws2_32.lib")
using namespace std;

const int BUF_SIZE = 2048;



SOCKET sockSer, sockCli;
SOCKADDR_IN addrSer, addrCli;


char recvbuf[BUF_SIZE];
char sendbuf[BUF_SIZE];


void Send()
{
	while (1)
	{
		cout << "我说:";
		cin >> sendbuf;
		send(sockCli, sendbuf, sizeof(sendbuf), 0);
		if (strcmp(sendbuf, "exit") == 0)
		{

			closesocket(sockCli);
			std::cout << "已退出聊天室" << std::endl;
			WSACleanup();
			break;
		}
	}
}
void Receive()
{
	while (1)
	{
		Sleep(1000);
		recv(sockCli, recvbuf, sizeof(recvbuf), 0);

		if (strlen(recvbuf) != 0)
		{
			cout << "\n" << recvbuf << endl;
			memset(recvbuf, '\0', sizeof(recvbuf));
		}
	}
}


int main()
{
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
	{
		std::cout << "载入socket库失败" << std::endl;
		system("pause");
		return 0;
	}

	sockCli = socket(AF_INET, SOCK_STREAM, 0);

	addrCli.sin_addr.s_addr = inet_addr("127.0.0.1");
	addrCli.sin_family = AF_INET;
	addrCli.sin_port = htons(12510);


	addrSer.sin_addr.s_addr = inet_addr("192.168.0.107");
	addrSer.sin_family = AF_INET;
	addrSer.sin_port = htons(12588);


	if (connect(sockCli, (SOCKADDR*)& addrSer, sizeof(addrSer)) != SOCKET_ERROR)
	{
		thread t1(Send);
		t1.detach();
		thread t2(Receive);
		t2.detach();
	}
	while (1);

	return 0;
}