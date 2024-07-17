#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "thirdPartyLibraries/include/color_console/color.hpp"
#include "Core/base/print.h"

bool print::colorCheck(std::string color) {
	std::vector<std::string> temp = {
		"black", "blue", "green", "aqua",
		"red", "purple", "yellow", "white",
		"grey", "light blue", "light green", "light aqua",
		"light red", "light purple", "light yellow", "bright white"

	};
	return std::find(temp.begin(), temp.end(), color) != temp.end();
}

void print::colorPrint(std::string color, std::string message) {
	if (color == "black") { std::cout << dye::black(message); }
	if (color == "blue") { std::cout << dye::blue(message); }
	if (color == "green") { std::cout << dye::green(message); }
	if (color == "aqua") { std::cout << dye::aqua(message); }
	if (color == "red") { std::cout << dye::red(message); }
	if (color == "purple") { std::cout << dye::purple(message); }
	if (color == "yellow") { std::cout << dye::yellow(message); }
	if (color == "white") { std::cout << dye::white(message); }
	if (color == "grey") { std::cout << dye::grey(message); }
	if (color == "light blue") { std::cout << dye::light_blue(message); }
	if (color == "light green") { std::cout << dye::light_green(message); }
	if (color == "light aqua") { std::cout << dye::light_aqua(message); }
	if (color == "light red") { std::cout << dye::light_red(message); }
	if (color == "light purple") { std::cout << dye::light_purple(message); }
	if (color == "light yellow") { std::cout << dye::light_yellow(message); }
	if (color == "bright white") { std::cout << dye::bright_white(message); }
}


print::print(std::string color, std::string message) {
	if (colorCheck(color)) { colorPrint(color, message); }
	else { std::cout << message; }
}
print::print(std::string message) {
	std::cout << message;
}
