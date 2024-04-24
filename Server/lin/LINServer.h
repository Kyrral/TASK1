#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "../IServer.h"
#include "../Info.h"

class LINServer : public IServer, public Info {

    int serverSocket, clientSocket;
    sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    void createSocket();
    void bindSocket();
    void listenClient();
    void acceptClient();

    void info();

    std::string sendData(int clientSocket_t) override;
    bool receiveData(const std::string& filename, int clientSocket_t) override;
    void handleClient(int clientSocket_t) override;

public:
    void setConnection() override;
};