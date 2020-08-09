#include <stdlib.h>
#include <stdio.h>
#include <WinSock2.h>
#include <iostream>
#include <WS2tcpip.h>
#include <cstring>
#include<thread>
#include<windows.h>
#include <sstream>
#include<queue>
#include <mutex>
#pragma comment (lib,"ws2_32.lib")
using namespace std;

const int BUF_SIZE = 2048;


SOCKET sockSer;
SOCKADDR_IN addrSer1;
int naddr = sizeof(SOCKADDR_IN);
char sendbuf[BUF_SIZE]={ };
char recvbuf[BUF_SIZE] = { };
int num = 0;

mutex mu;

struct node//定义bfs节点
{
	int Message;
	char Sendbuf[BUF_SIZE] = { };

};
queue<node> q;

struct SockCli
{
	SOCKADDR_IN addrCli;
	SOCKET sockCli;
	int occupy=0;

	
}sockcli[10];


void Receive(int num)
{
	
	int Num = num;
	string str1;
	stringstream s1;
	s1 << Num;
	s1 >> str1;
	mu.lock();
	strcat(sendbuf, "欢迎");
	strcat(sendbuf, str1.c_str());
	strcat(sendbuf, "号进入聊天室");
	struct node u;
	u.Message = Num;
	strcpy(u.Sendbuf, sendbuf);
	q.push(u);
	memset(sendbuf, '\0', sizeof(sendbuf));
	mu.unlock();

	cout << Num << "号对话开始qian" << endl;
	while (1)
	{
		recv(sockcli[Num].sockCli, recvbuf, sizeof(recvbuf), 0);
		cout << recvbuf << endl;
	
		if (strlen(recvbuf) != 0)
		{ 
			if (strcmp(recvbuf, "exit") == 0)
			{
				closesocket(sockcli[Num].sockCli);
				sockcli[Num].occupy = 0;
				string str;
				stringstream ss;
				ss << Num;
				ss >> str;
				mu.lock();
				strcpy(sendbuf, str.c_str());
				strcat(sendbuf, "号已退出聊天室");
				
				struct node u;
				u.Message = Num;
				strcpy(u.Sendbuf, sendbuf);
				q.push(u);
				memset(recvbuf, '\0', sizeof(recvbuf));
				memset(sendbuf, '\0', sizeof(sendbuf));
				mu.unlock();

				break;

			}
			else
			{
				string str;
				stringstream ss;
				ss << Num;
				ss >> str;
				mu.lock();
				strcpy(sendbuf, str.c_str());
				strcat(sendbuf, "号：");
				strcat(sendbuf, recvbuf);
				struct node u;
				u.Message = Num;
				strcpy(u.Sendbuf, sendbuf);
				q.push(u);
				memset(recvbuf, '\0', sizeof(recvbuf));
				memset(sendbuf, '\0', sizeof(sendbuf));
				mu.unlock();
				
				
			}
			
		}
	}
}

void Send()
{
	while (1)
	{
		if (!q.empty())
		{
			node v = q.front();
			q.pop();
			for (int j = 0; j < 10; j++)
			{
				if (sockcli[j].occupy == 1 && j != v.Message)
				{
					if (send(sockcli[j].sockCli, v.Sendbuf, sizeof(v.Sendbuf), 0) > 0)
					{
						cout << "发送消息\| " << v.Sendbuf << " \|至" << j << "号客户端" << endl;
					}
				}
			}
		}
	}
}


void Listen()
{		
	int i = 0;
	while (1)
	{
		
		listen(sockSer, 5);
		sockcli[i].sockCli = accept(sockSer, (SOCKADDR*)& sockcli[i].addrCli, &naddr);
		if (sockcli[i].sockCli != INVALID_SOCKET)
		{

			cout << i<<"号对象连接成功" << endl;
			switch (i)
			{
			case 0: {thread t0(Receive, num); t0.detach();  sockcli[i].occupy = 1; break;}
			case 1: {thread t1(Receive, num); t1.detach();  sockcli[i].occupy = 1; break;}
			case 2: {thread t2(Receive, num); t2.detach();  sockcli[i].occupy = 1; break;}
			case 3: {thread t3(Receive, num); t3.detach();  sockcli[i].occupy = 1; break;}
			case 4: {thread t4(Receive, num); t4.detach();  sockcli[i].occupy = 1; break;}
			case 5: {thread t5(Receive, num); t5.detach();  sockcli[i].occupy = 1; break;}
			case 6: {thread t6(Receive, num); t6.detach();  sockcli[i].occupy = 1; break;}
			}
			cout << i << "为i值" << endl;
			cout << num << "为num值" << endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
			for (int j =0; j<10; j++)
				if (sockcli[j].occupy == 0)
				{
					i = j;
					break;
				}
			num = i;
			
		}
	}
}





int main()
{
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
	{
		cout << "载入socket库失败" << endl;
		system("pause");
		return 0;
	}

	sockSer = socket(AF_INET, SOCK_STREAM, 0);

	addrSer1.sin_addr.s_addr = inet_addr("192.168.0.107");
	addrSer1.sin_family = AF_INET;
	addrSer1.sin_port = htons(12588);


	bind(sockSer, (SOCKADDR*)& addrSer1, sizeof(SOCKADDR));

	thread L1(Listen);
	L1.detach();
	thread S1(Send);
	S1.detach();
	while (1);

	closesocket(sockSer);
	//closesocket(sockCli1);
	//closesocket(sockCli2);
	WSACleanup();
	return 0;
}

