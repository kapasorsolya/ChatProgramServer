#ifndef MYTHREAD_H
#define MYTHREAD_H

#include "SysThread.h"
#include <string>
#include <vector>
#include <map>

using namespace std;

class MyThread : public SysThread
{
	
private:
	SOCKET acceptSocket;
	vector<MyThread*> &myThreads;
	CRITICAL_SECTION *criticalSection;
	string myName;

protected:
	void run(void);

public:
	MyThread(vector<MyThread*>&);
	void setSocket(SOCKET);
	SOCKET getSocket();
	void setMyThreads(vector<MyThread*>&);
	void setCriticalSection(CRITICAL_SECTION*);
	void setMyName(string);
};
#endif // !MYTHREAD_H
