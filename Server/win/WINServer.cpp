#include "WINServer.h"

int WINServer::createSocket()
{
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (serverSocket == -1) {
		std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
		return 1;
	}
	else std::cout << "Socket is OK!" << std::endl;
}

int WINServer::bindSocket()
{
	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr("127.0.0.1");
	service.sin_port = htons(55555);

	if (bind(serverSocket, reinterpret_cast<SOCKADDR*>(&service), sizeof(service)) == SOCKET_ERROR) {
		std::cout << "Socket binding failed: " << WSAGetLastError() << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return 1;
	}
	else {
		std::cout << "Socket binded successfully" << std::endl;
	}

	return 0;
}

void WINServer::initWinsock2()
{
	int wsaerr = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (wsaerr != 0)
	{
		std::cerr << "WSAStartup failed with error: " << wsaerr << std::endl;
	}
	else {
		std::cout << "The Winsock dll found" << std::endl;
		std::cout << "The status: " << wsaData.szSystemStatus << std::endl;
	}
}

int WINServer::setServer()
{
	initWinsock2();

	createSocket();
	bindSocket();

	return 0;
}

bool WINServer::receiveData(const std::string& filename, int clientSocket_t)
{
	//char receiveBuffer[1024];
	//size_t datasize;
	//FILE* fd;

	//std::string path = folder + filename;
	//
	//int err = fopen_s(&fd, path.c_str(), "wb");

	//if (err != 0) {
	//	std::cerr << "Unable to open file" << std::endl;
	//}

	//do {
	//	datasize = recv(clientSocket, receiveBuffer, sizeof(receiveBuffer), 0);
	//	if (datasize > 0) fwrite(&receiveBuffer, 1, datasize, fd);
	//	if (datasize == 0) break;
	//} while (datasize > 0);

	//fclose(fd);

	//return true;


	char buffer[1024];
	size_t datasize;

	std::string path = folder + filename;
	FILE* file = fopen(path.c_str(), "wb");
	do {
		datasize = recv(clientSocket_t, buffer, sizeof(buffer), 0);
		if (datasize > 0) fwrite(&buffer, 1, datasize, file);
	} while (datasize > 0);
	fclose(file);
	return true;
}

std::string WINServer::sendData(int clientSocket_t)
{
	if (send(clientSocket_t, filename.c_str(), strlen(filename.c_str()), 0) == -1) {
		std::cerr << "error sendData()" << std::endl;
		closesocket(clientSocket_t);
		closesocket(serverSocket);
	}
	std::cout << "Request sent" << std::endl;

	return filename;
}

void WINServer::setConnection()
{
	setServer();
	listenConnections();

	std::thread a(&WINServer::acceptConnections, this);
	a.join();
}

void WINServer::listenConnections()
{
	if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
		std::cout << "listen(): Error listening on socket: " << WSAGetLastError() << std::endl;
	else std::cout << "listen() is OK! Waiting for new connections..." << std::endl;
}

void WINServer::acceptConnections()
{
	bool isFirstCall = true;

	while (true) {
		clientSocket = accept(serverSocket, nullptr, nullptr);
		if (clientSocket == INVALID_SOCKET) {
			std::cout << "Accept failed: " << WSAGetLastError() << std::endl;
			closesocket(serverSocket);
			WSACleanup();
			break;
		}
		clientThreads.emplace_back(std::make_unique<std::thread>(&WINServer::handleClient, this, clientSocket))->detach();
		if (isFirstCall == true) {
			isFirstCall = false;
			checklistInfo = false;
			std::thread info(&WINServer::info, this);
			info.detach();
		}
	}
}

void WINServer::info()
{
	listFormat();
	do {
		std::cin >> filename;
		if (nameCheck(filename) == false) std::cout << "Wrong format! Try again: ";
	} while (nameCheck(filename) == false);

	filename += ".bmp";

	bool proceed = false;

	do {
		proceed = clientThreads.size() != 0;
	} while (proceed == false);

	std::unique_lock lck(mtx);
	cv.wait(lck, [proceed] { return proceed; });

	listClients(clientThreads);
	std::cout << "Enter client #: ";
	int temp;
	std::cin >> temp;
	numClient = temp;

	checklistInfo = true;
}

void WINServer::handleClient(int clientSocket_t) {
	//std::cout << "handleCLient()";

	//const int num = ++counter;

 //   bool proceed = num == numClient;

 //   std::unique_lock lck(mtx);
 //   cv.wait(lck, [proceed] { return proceed; });

 //   sendData(clientSocket_t);
 //   if(receiveData(filename, clientSocket_t)) std::cout << filename << " is loaded" << std::endl;

	//closesocket(clientSocket_t);

 //   std::cout << "Client #" << num << " closed" << std::endl;

	//

	bool proceed = false;

	const int num = ++counter;

	do {
		proceed = num == numClient;
	} while (proceed == false);

	std::unique_lock lck(mtx);
	cv.wait(lck, [proceed] { return proceed; });

	sendData(clientSocket_t);
	if (receiveData(filename, clientSocket_t)) std::cout << filename << " is loaded" << std::endl;

	if (checklistInfo == true) {
		checklistInfo = false;
		std::thread info(&WINServer::info, this);
		info.detach();
	}

	closesocket(clientSocket_t);
	std::cout << "Client #" << num << " closed" << std::endl;
	clientThreads.at(num - 1) = nullptr;
}