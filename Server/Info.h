#include <vector>
#include <string>
#include <iostream>
#include <thread>
#include <regex>

class Info {
public:
    void listClients(std::vector<std::unique_ptr<std::thread>>& clientThreads);
    void listFormat();
    bool nameCheck(const std::string& filename);
};