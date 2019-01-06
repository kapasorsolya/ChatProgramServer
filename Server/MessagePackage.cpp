#include "MessagePackage.h"

#include <sstream>

MessagePackage::MessagePackage()
{}

MessagePackage::MessagePackage(string message) : composedMessage(message)
{}

MessagePackage::MessagePackage(int type, int file, string from, string to, string message) : messageType(type), isFile(file), from(from), to(to), message(message)
{}


tuple<int, int, string, string, string> MessagePackage::sliceMessage(string composed)
{
	int pos;
	pos = composed.find("<=>");
	messageType = std::stoi(composed.substr(0, pos));
	composed.erase(0, pos + 3);

	pos = composed.find("<=>");
	isFile = std::stoi(composed.substr(0, pos));
	composed.erase(0, pos + 3);

	pos = composed.find("<=>");
	from = composed.substr(0, pos);
	composed.erase(0, pos + 3);

	pos = composed.find("<=>");
	to = composed.substr(0, pos);
	composed.erase(0, pos + 3);

	message = composed;

	return make_tuple(messageType, isFile, from, to, message);
}

vector<string> MessagePackage::toUsernameList(string names)
{
	vector<string> returnVector;
	int pos = 0;
	while (pos != string::npos)
	{
		pos = names.find(",");
		returnVector.push_back(names.substr(0, pos));
		names.erase(0, pos + 1);
	}
	returnVector.push_back(names);
	return returnVector;
}

string MessagePackage::toUsernameString(vector<string> nameVector)
{
	string usernames;
	for (auto it = nameVector.begin(); it != nameVector.end(); ++it)
	{
		usernames += (*it);
		auto i = it;
		if (++i == nameVector.end())
			continue;
		usernames += ",";
	}
	return usernames;
}

string MessagePackage::concatenateMessage()
{
	return std::to_string(messageType) + "<=>" + std::to_string(isFile) + "<=>" + from + "<=>" + to + "<=>" + message;
}

string MessagePackage::concatenateMessage(int tpe, int file, string frm, string to, string msg)
{
	return std::to_string(tpe) + "<=>" + std::to_string(file) + "<=>" + frm + "<=>" + to + "<=>" + msg;
}

string MessagePackage::charArrayToString(char *charArray, int length)
{
	string str;
	str = charArray[0];
	for (int i = 1; i < length; ++i)
		str += charArray[i];
	return str;
}