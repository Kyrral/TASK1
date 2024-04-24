#include "AScreenshot.h"

std::string AScreenshot::makeName()
{
	auto currentTime = std::chrono::system_clock::now();

	std::time_t currentTimeT = std::chrono::system_clock::to_time_t(currentTime);

	std::tm timeinfo;
	
#ifdef _WIN32
	localtime_s(&timeinfo, &currentTimeT);
#elif __linux__
	localtime_r(&currentTimeT, &timeinfo);
#endif

	char dateBuff[11];
	std::strftime(dateBuff, sizeof(dateBuff), "%Y.%m.%d", &timeinfo);

#ifdef _WIN32
	std::string name = static_cast<std::string>(dateBuff) + "_" + std::to_string(number) + ".bmp";
#elif __linux__
	std::string name = static_cast<std::string>(dateBuff) + "_" + std::to_string(number) + ".ppm";
#endif

	return "./screenshots/" + name;
}
