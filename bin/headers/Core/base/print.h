#pragma once
#ifndef PRINT
#define PRINT
#include <string>

class print {
	private:
	public:
		enum colors {
			black, white, blue, yellow,
			red, aqua, purple, green, grey,
			light_blue, light_green, light_aqua, light_red,
			light_purple, light_yellow, bright_white 
		};
		print(colors color, std::string message);
		print(std::string message);
};

#endif