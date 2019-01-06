#include <iostream>
#include <vector>
#include <map>
#include "winsock2.h"
#include "MyThread.h"

using namespace std;
#pragma warning(disable:4996) 
int main()
{
	CRITICAL_SECTION criticalSection;
	SOCKET ListenSocket;
	SOCKET AcceptSocket;
	sockaddr_in service;
	WSADATA wsaData;
	u_short port=9000;

	vector<MyThread*> myThreads;
	int iResult;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR)
	{
		printf("Error at WSAStartup()\n");
		system("pause");
		return 1;
	}

	
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET)
	{
		printf("Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		system("pause");
		return 1;
	}

	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr("127.0.0.1");
	service.sin_port = htons(port);


	if (bind(ListenSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
	{
		printf("bind() failed.\n");
		closesocket(ListenSocket);
		WSACleanup();
		system("pause");
		return 1;
	}

	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		cout << "Error listening on socket.\n";
		closesocket(ListenSocket);
		WSACleanup();
		system("pause");
		return 1;
	}

	InitializeCriticalSection(&criticalSection);
	while (1)
	{  
		cout << "Waiting for client to connect..." << endl;

		AcceptSocket = accept(ListenSocket, NULL, NULL);
		if (AcceptSocket == INVALID_SOCKET)
		{
			printf("accept failed: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			system("pause");
			return 1;
		}
		cout << "Client connected!" << endl;
		cout << "Receiving datagrams..." << endl;
		MyThread *thread = new MyThread(myThreads);
		myThreads.push_back(thread);
		thread->setCriticalSection(&criticalSection); 
		thread->setSocket(AcceptSocket);
		thread->start();
	}

	closesocket(AcceptSocket);
	closesocket(ListenSocket);
	WSACleanup();
	return 0;
}