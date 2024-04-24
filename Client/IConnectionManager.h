#pragma once
#include <string>

class IConnectionManager
{
public:
	virtual void setConnection() = 0;
	virtual void sendData(const std::string& filename) = 0;
	virtual std::string receiveData() = 0;
};
