#pragma once
#include <string>
#include <chrono>
#include <ctime>

class AScreenshot
{
protected:
	int number = 0;
	std::string makeName();

public:
	virtual void takeScreenshot() = 0;
};