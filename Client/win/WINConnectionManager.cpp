#include "WINConnectionManager.h"

void WINConnectionManager::initializeWinsock()
{
	wsaerr = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (wsaerr != 0) {
		std::cout << "The winsock dll not found" << std::endl;
	}
	else {
		std::cout << "The Winsock dll is found" << std::endl;
		std::cout << "Status: " << wsaData.szSystemStatus << std::endl;
	}
}

void WINConnectionManager::setSocket()
{
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (clientSocket == INVALID_SOCKET) {
		std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
		WSACleanup();
	}
	else {
		std::cout << "The socket is OK!" << std::endl;
	}
}

void WINConnectionManager::connectToServer()
{
	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
	clientService.sin_port = htons(55555);

	std::cout << "Trying to connect..." << std::endl;
	while (true) {
		if (connect(clientSocket, reinterpret_cast<SOCKADDR*>(&clientService), sizeof(clientService)) != -1)
			break;
	}

	std::cout << "Connection established!" << std::endl;
	std::cout << "Can start receiving data..." << std::endl;
}

void WINConnectionManager::setConnection()
{
	initializeWinsock();
	setSocket();
	connectToServer();
}

void WINConnectionManager::sendData(const std::string& filename)
{
	char buffer[1024];

	std::string path = "./screenshots/" + filename;

	FILE* fd = fopen(path.c_str(), "rb");

	int bytes_read;

	while (!feof(fd)) {
		if ((bytes_read = fread(&buffer, 1, sizeof(buffer), fd)) > 0)
			send(clientSocket, buffer, bytes_read, 0);
		else break;
	}
	fclose(fd);
	closesocket(clientSocket);
	std::cout << "File sent successfully" << std::endl;
}

std::string WINConnectionManager::receiveData()
{
	char buffer[1024];
	int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

	if (bytesReceived == -1) {
		std::cerr << "Receive failed" << std::endl;
		closesocket(clientSocket);
		return "none";
	}
	if (bytesReceived > 0) {
		filename = std::string(buffer, bytesReceived);
		std::cout << filename << " requested" << std::endl;
		return filename;
	}
	return "none";
}