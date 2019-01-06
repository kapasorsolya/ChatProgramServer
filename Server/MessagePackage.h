#ifndef MESSAGEPACKAGE_H
#define MESSAGEPACKAGE_H

#include <string>
#include <vector>
#include <tuple>

using namespace std;

class MessagePackage
{
private:
	int messageType;
	int isFile;
	string from;
	string to;
	string message;
	string composedMessage;

public:
	MessagePackage();
	MessagePackage(string);
	MessagePackage(int, int, string, string, string);
	tuple<int, int, string, string, string> sliceMessage(string);
	vector<string> toUsernameList(string);
	string toUsernameString(vector<string>);
	string concatenateMessage();
	string concatenateMessage(int, int, string, string, string);
	string charArrayToString(char*, int);
};

#endif // !MESSAGEPACKAGE_H
