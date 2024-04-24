#include "Info.h"

void Info::listFormat() {
    std::cout << "Enter the file name in format: yyyy.mm.dd_#\nExample: 2024.04.22_1" << std::endl;
}

bool Info::nameCheck(const std::string& filename) {
    std::regex pattern(R"(\d{4}\.\d{2}\.\d{2}_\d)");

    return std::regex_match(filename, pattern);
}

void Info::listClients(std::vector<std::unique_ptr<std::thread>>& clientThreads) {
    
    int yn;

    std::cout << "Would you like to check the list ov available clients? 1 - yes | 0 - no" << std::endl;
    
    std::cin >> yn;

    if (yn == 1) {
        if (clientThreads.empty()) std::cout << "No connections yet" << std::endl;
        else {
            std::cout << "Clients available now:" << std::endl;
            for (int i = 0; i < clientThreads.size(); ++i){
                if (clientThreads[i].get() != nullptr) std::cout << "#" << i + 1 << std::endl; 
            }        
        }
    }
    return;
}