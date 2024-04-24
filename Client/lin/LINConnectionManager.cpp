#include "LINConnectionManager.h"

void LINConnectionManager::createSocket() {
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Failed to create the socket" << std::endl;
        return;
    }
    std::cout << "Socket created successfully" << std::endl;
}

void LINConnectionManager::setServerAddr() {
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(55555);

    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
        std::cerr << "Failed to set the address" << std::endl;
        close(clientSocket);
        return;
    }
}

void LINConnectionManager::setConnection() {
    createSocket();
    setServerAddr();

    std::cout << "Connecting to server..." << std::endl;
    while(true){
        if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) != -1)
            break;
    }
    std::cout << "Connected successfully!" << std::endl;
}

void LINConnectionManager::sendData(const std::string& filename) {
    char buffer[1024];

    std::string path = "./screenshots/" + filename;

    FILE* file = fopen(path.c_str(), "rb");

    if (file == nullptr) std::cout << "Failed to open the file" << std::endl;

    size_t bytes_read;
    while (!feof(file)) {
        if ((bytes_read = fread(&buffer, 1, sizeof(buffer), file)) > 0)
            send(clientSocket, buffer, bytes_read, 0);
        else break;
    }
    fclose(file);
    close(clientSocket);
    std::cout << "File sent!" << std::endl;
}

std::string LINConnectionManager::receiveData() {
    char buffer[1024];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

    if (bytesReceived == -1) {
        std::cerr << "Receive failed" << std::endl;
        close(clientSocket);
        return "none";
    }
    if (bytesReceived > 0) {
        filename = std::string(buffer, bytesReceived);
        std::cout << filename << " requested" << std::endl;
        return filename;
    }
    return "none";
}
