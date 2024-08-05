// Print header
// by SpaceOC
#pragma once
#ifndef NRC_BASE_PRINT_H_
#define NRC_BASE_PRINT_H_
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
		print();
		print(colors color, std::string message);
		print(std::string message);
};

#endif