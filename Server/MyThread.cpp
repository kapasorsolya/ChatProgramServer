#include "MessagePackage.h"
#include "MyThread.h"

#include <iostream>
#include <string>

MyThread::MyThread(vector<MyThread*> &threads) : myThreads(threads)
{

}


void MyThread::run(void)
{
	char RecvBuf[65536];
	int BufLen = 65536;
	int iResult;
	while (1)
	{
		iResult = recv(acceptSocket, RecvBuf, BufLen, 0);
		if (iResult == SOCKET_ERROR)
		{
			std::cout << "Receive error!" << std::endl;
			return;
		}

		EnterCriticalSection(criticalSection);
		MessagePackage mp;
		string fullMessage = mp.charArrayToString(RecvBuf, iResult);
		cout << fullMessage << endl;
		tuple<int, int, string, string, string> myTuple = mp.sliceMessage(fullMessage);
		//cout << get<0>(myTuple) << get<1>(myTuple) << get<2>(myTuple) << get<3>(myTuple) << get<4>(myTuple) << endl;
		switch (get<0>(myTuple))
		{
		case 4:
			{
				  int ok = 1;
				  for (auto it = myThreads.begin(); it != myThreads.end(); ++it)
				  {
					  if (get<2>(myTuple) == (*it)->myName)
					  {
						  string errorMessage = "5<=>0<=><=><=>";
						  const char *buff = errorMessage.c_str();
						  SOCKET socket = this->getSocket();
						  if (send(socket, buff, errorMessage.length(), 0) == SOCKET_ERROR)
						  {
							  std::cout << "Sending error!" << std::endl;
							  return;
						  }
						  ok = 0;
						  break;
					  }
				  }
				  if (ok == 0)
					  break;

				  this->setMyName(get<2>(myTuple));
				  string usernames = "0<=>0<=><=><=>";
				  for (auto it = myThreads.begin(); it != myThreads.end(); ++it)
				  {
					  if ((*it)->myName != "")
					  {
						  usernames += (*it)->myName;
						  auto i = it;
						  if (++i == myThreads.end())
							  continue;
						  usernames += ",";
					  }
				  }
				  const char *buff = usernames.c_str();
				  for (auto it = myThreads.begin(); it != myThreads.end(); ++it)
				  {
				  	if ((*it)->isRunning())
				  	{
				  		SOCKET socket = (*it)->getSocket();
				  		//if (socket != this->acceptSocket)
				  		//{
				  			if (send(socket, buff, usernames.length(), 0) == SOCKET_ERROR)
				  			{
				  				std::cout << "Sending error!" << std::endl;
				  				return;
				  			}
				  		//}
				  	}
				  	else
				  	{
				  		myThreads.erase(it);
				  		delete (*it);
				  	}
				  }
			}break;
		case 1:
			{
				  const char *buff = fullMessage.c_str();
				  for (auto it = myThreads.begin(); it != myThreads.end(); ++it)
				  {
					  if ((*it)->myName == get<2>(myTuple) || (*it)->myName == get<3>(myTuple))
					  {
						  if ((*it)->isRunning())
						  {
							  SOCKET socket = (*it)->getSocket();
							  if (send(socket, RecvBuf, iResult, 0) == SOCKET_ERROR)
							  {
								  std::cout << "Sending error!" << std::endl;
								  return;
							  }cout << "private message sent !!!" << endl;
						  }
						  else
						  {
							  myThreads.erase(it);
							  delete (*it);
						  }
					  }
				  }
			}break;
		case 2:
			{
				  vector<string> chatRoomPartners = mp.toUsernameList(get<3>(myTuple));
				  const char *buff = fullMessage.c_str();
				  for (auto it = myThreads.begin(); it != myThreads.end(); ++it)
				  {
					  for (string name : chatRoomPartners)
					  {
						  if (name == (*it)->myName)
						  {
							  if ((*it)->isRunning())
							  {
								  SOCKET socket = (*it)->getSocket();
								  if (send(socket, RecvBuf, iResult, 0) == SOCKET_ERROR)
								  {
									  std::cout << "Sending error!" << std::endl;
									  return;
								  }cout << "private message sent !!!" << endl;
							  }
							  else
							  {
								  myThreads.erase(it);
								  delete (*it);
							  }
							  break;
						  }
					  }
				  }
			}break;
		case 3:
			{
				  const char *buff = fullMessage.c_str();
				  for (auto it = myThreads.begin(); it != myThreads.end(); ++it)
				  {
					  if ((*it)->isRunning())
					  {
						  SOCKET socket = (*it)->getSocket();
						  if (send(socket, RecvBuf, iResult, 0) == SOCKET_ERROR)
						  {
							  std::cout << "Sending error!" << std::endl;
							  return;
						  }
					  }
					  else
					  {
						  myThreads.erase(it);
						  delete (*it);
					  }
				  }
			}break;
		default:
			break;
		}
		LeaveCriticalSection(criticalSection);
		
	}
}

void MyThread::setSocket(SOCKET acceptSocket)
{
	this->acceptSocket = acceptSocket;
}

SOCKET MyThread::getSocket()
{
	return this->acceptSocket;
}

void MyThread::setMyThreads(vector<MyThread*> &my)
{
	this->myThreads = my;
}

void MyThread::setCriticalSection(CRITICAL_SECTION *criticalSection)
{
	this->criticalSection = criticalSection;
}

void MyThread::setMyName(string name)
{
	this->myName = name;
}