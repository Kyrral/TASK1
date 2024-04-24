#pragma once
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <iostream>
#include <cstring>

class IServer
{
protected:
	std::string folder = "./screenshots/";
	std::string filename;

	std::mutex mtx;
	std::condition_variable cv;

	std::vector<std::unique_ptr<std::thread>> clientThreads;
	int counter = 0;

	std::atomic<int> numClient;
	std::atomic<bool> checklistInfo = true;

	virtual void handleClient(int clientSocket_t) = 0;
	virtual bool receiveData(const std::string& filename, int clientSocket_t) = 0;
	virtual std::string sendData(int clientSocket_t) = 0;

public:

	virtual void setConnection() = 0;

};