#pragma once
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>

#include "../IConnectionManager.h"

class LINConnectionManager : public IConnectionManager
{
    int clientSocket;
    sockaddr_in serverAddr;

    std::string filename;

    void createSocket();
    void setServerAddr();

public:
    void setConnection() override;

    void sendData(const std::string& filename) override;
    std::string receiveData() override;
};