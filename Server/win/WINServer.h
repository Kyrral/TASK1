#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

#pragma once
#include <iostream>
#include "../IServer.h"
#include "../Info.h"

class WINServer : public IServer, public Info
{
	SOCKET serverSocket = INVALID_SOCKET, clientSocket;
	WSADATA wsaData;

private:
	void initWinsock2();
	
	int createSocket();
	int bindSocket();
	void listenConnections();
	void acceptConnections();

	void info();

	int setServer();
public:

	void setConnection() override;
	
	bool receiveData(const std::string& filename, int clientSocket_t) override;
	virtual std::string sendData(int clientSocket_t) override;
	void handleClient(int clientSocket_t);
};