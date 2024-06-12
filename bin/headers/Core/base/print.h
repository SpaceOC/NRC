#pragma once

#include <string>

class print {
private:
	bool colorCheck(std::string color);

	void colorPrint(std::string color, std::string message);
public:
	print(std::string color, std::string message);
	print(std::string message);
};