#include <thread>
#include <filesystem>
#include <iostream>
#include <fstream>

#include "IConnectionManager.h"
#include "AScreenshot.h"

#ifdef _WIN32
#include "win/WINConnectionManager.h"
#include "win/WINScreenshot.h"
#elif __linux__
#include "lin/LINConnectionManager.h"
#include "lin/LINScreenshot.h"
#endif

void takeScreenshot();
void createFolder();
bool checkFolder(const std::string filename);

int main()
{
	createFolder();

	std::thread sreenshoting(takeScreenshot);
	sreenshoting.detach();

#ifdef _WIN32
	IConnectionManager* clientSocket = new WINConnectionManager();
#elif __linux__
	IConnectionManager* clientSocket = new LINConnectionManager();
#endif

	clientSocket->setConnection();
	
	std::string filename = clientSocket->receiveData();
	if (filename != "none") if (checkFolder(filename)) clientSocket->sendData(filename);
}

void takeScreenshot() {
#ifdef _WIN32
	AScreenshot* scrshot = new WINScreenshot();
#elif __linux__
	AScreenshot* scrshot = new LINScreenshot();
#endif
	while(true) {
		scrshot->takeScreenshot();
		std::cout << "screenshot" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(10));
	}
}

void createFolder() {
    std::string folderName { "screenshots" };

    if (!std::filesystem::exists(folderName)) {
        if (std::filesystem::create_directory(folderName)) {
            std::cout << "Folder created successfully" << std::endl;
        } else {
            std::cerr << "Failed to create folder: " << folderName << std::endl;
        }
    } else {
        std::cout << "Folder " << folderName << " already exists" << std::endl;
    }
}

bool checkFolder(const std::string filename) {
	std::string folderPath = "./screenshots";

    std::filesystem::path filePath = folderPath;
    filePath /= filename;

    if (std::filesystem::exists(filePath)) {
		std::cout << "File exists" << std::endl;
		return true;
    } else {
		std::cout << "Requested file doesn't exist" << std::endl;
		return false;
    }
}