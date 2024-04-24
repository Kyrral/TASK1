#include "LINServer.h"

void LINServer::createSocket() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) std::cout << "Socket creating error" << std::endl;
    std::cout << "Socket created successfully" << std::endl;
}

void LINServer::bindSocket() {
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(55555);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
    {
        std::cout << "Socket binding failed" << std::endl;
        close(serverSocket);
    }
    std::cout << "Socket binded successfully" << std::endl;
}

void LINServer::listenClient() {
    if (listen(serverSocket, SOMAXCONN) == -1)
    {
        std::cout << "listen() error" << std::endl;
        close(serverSocket);
    } else std::cout << "Server is listening" << std::endl;
}

void LINServer::acceptClient() {
    socklen_t clientAddrLen = sizeof(clientAddr);

    bool isFirstCall = true;

    while(true) {
        clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket == -1) {
            std::cerr << "Failed to accpet client #" << counter + 1 << std::endl;
            close(serverSocket);
            break;
        }
        clientThreads.emplace_back(std::make_unique<std::thread>(&LINServer::handleClient, this, clientSocket))->detach();
        if (isFirstCall == true) {
            isFirstCall = false;
            checklistInfo = false;
            std::thread info(&LINServer::info, this);
            info.detach();
        }
    }
}

void LINServer::info() {

    listFormat();
    do {
        std::cin >> filename;
        if (nameCheck(filename) == false) std::cout << "Wrong format! Try again: ";
    } while (nameCheck(filename) == false);

    filename += ".ppm";

    bool proceed = false;

    do {
        proceed = clientThreads.size() != 0;
    } while(proceed == false);

    std::unique_lock lck(mtx);
    cv.wait(lck, [proceed] { return proceed; });

    listClients(clientThreads);
    std::cout << "Enter client #: ";
    int temp;
    std::cin >> temp;
    numClient = temp;

    checklistInfo = true;
}

void LINServer::setConnection() {
    createSocket();
    bindSocket();
    listenClient();

    std::cout << "Server is set up. Waiting for connections..." << std::endl;

    std::thread a(&LINServer::acceptClient, this);
    a.join();
}

std::string LINServer::sendData(int clientSocket_t) {
    if (send(clientSocket_t, filename.c_str(), strlen(filename.c_str()), 0) == -1) {
        std::cerr << "error sendData()" << std::endl;
        close(clientSocket_t);
        close(serverSocket);
    }
    std::cout << "Request sent" << std::endl;

    return filename;
}

bool LINServer::receiveData(const std::string& filename, int clientSocket_t) {
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

void LINServer::handleClient(int clientSocket_t) {

    bool proceed = false;

    const int num = ++counter;

    do {
        proceed = num == numClient;
    } while(proceed == false);

    std::unique_lock lck(mtx);
    cv.wait(lck, [proceed] { return proceed; });

    sendData(clientSocket_t);
    if(receiveData(filename, clientSocket_t)) std::cout << filename << " is loaded" << std::endl;

    if (checklistInfo == true) {
        checklistInfo = false;
        std::thread info(&LINServer::info, this);
        info.detach();
    }

    close(clientSocket_t);
    std::cout << "Client #" << num << " closed" << std::endl;
    clientThreads.at(num - 1) = nullptr;
}