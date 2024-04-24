#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma once
#include <Winsock2.h>
#include <string>
#include <iostream>

#include "../IConnectionManager.h"

#pragma comment(lib, "ws2_32.lib")

class WINConnectionManager : public IConnectionManager
{
	WSADATA wsaData;
	int wsaerr;

	SOCKET clientSocket = INVALID_SOCKET;

	void initializeWinsock();

	std::string filename;

	void setSocket();
	void connectToServer();

public:

	void setConnection() override;

	void sendData(const std::string& filename) override;
	std::string receiveData() override;
};