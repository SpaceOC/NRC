/*
    Copyright (C) 2024-2024  SpaceOC

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include <iostream>
#include <vector>
#include <map>
#include <color_console/color.hpp>
#include "Core/base/print.h"

print::print(colors color, std::string message) {
	if (color == colors::black) { std::cout << dye::black(message); }
	if (color == colors::blue) { std::cout << dye::blue(message); }
	if (color == colors::green) { std::cout << dye::green(message); }
	if (color == colors::aqua) { std::cout << dye::aqua(message); }
	if (color == colors::red) { std::cout << dye::red(message); }
	if (color == colors::purple) { std::cout << dye::purple(message); }
	if (color == colors::yellow) { std::cout << dye::yellow(message); }
	if (color == colors::white) { std::cout << dye::white(message); }
	if (color == colors::grey) { std::cout << dye::grey(message); }
	if (color == colors::light_blue) { std::cout << dye::light_blue(message); }
	if (color == colors::light_green) { std::cout << dye::light_green(message); }
	if (color == colors::light_aqua) { std::cout << dye::light_aqua(message); }
	if (color == colors::light_red) { std::cout << dye::light_red(message); }
	if (color == colors::light_purple) { std::cout << dye::light_purple(message); }
	if (color == colors::light_yellow) { std::cout << dye::light_yellow(message); }
	if (color == colors::bright_white) { std::cout << dye::bright_white(message); }
}
print::print(std::string message) {
	std::cout << message;
}
