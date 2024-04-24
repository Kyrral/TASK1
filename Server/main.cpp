#include <filesystem>

#ifdef _WIN32
#include "win/WINServer.h"
#elif __linux__
#include "lin/LINServer.h"
#endif

#include "IServer.h"

void createFolder();

int main()
{
	createFolder();

#ifdef _WIN32
	IServer* server = new WINServer();
#elif __linux__
	IServer* server = new LINServer();
#endif

	server->setConnection();
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