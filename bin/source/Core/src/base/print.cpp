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
#include <color_console/color.hpp>
#include "Core/base/print.h"

core::print::print() {
	std::cout << '\n';
}

core::print::print(const std::string& message) {
	std::cout << message;
}

core::print::print(const colors& color, const std::string& message) {
	if (color == colors::black) { std::cout << dye::black(message); }
	else if (color == colors::blue) { std::cout << dye::blue(message); }
	else if (color == colors::green) { std::cout << dye::green(message); }
	else if (color == colors::aqua) { std::cout << dye::aqua(message); }
	else if (color == colors::red) { std::cout << dye::red(message); }
	else if (color == colors::purple) { std::cout << dye::purple(message); }
	else if (color == colors::yellow) { std::cout << dye::yellow(message); }
	else if (color == colors::white) { std::cout << dye::white(message); }
	else if (color == colors::grey) { std::cout << dye::grey(message); }
	else if (color == colors::light_blue) { std::cout << dye::light_blue(message); }
	else if (color == colors::light_green) { std::cout << dye::light_green(message); }
	else if (color == colors::light_aqua) { std::cout << dye::light_aqua(message); }
	else if (color == colors::light_red) { std::cout << dye::light_red(message); }
	else if (color == colors::light_purple) { std::cout << dye::light_purple(message); }
	else if (color == colors::light_yellow) { std::cout << dye::light_yellow(message); }
	else if (color == colors::bright_white) { std::cout << dye::bright_white(message); }
}

core::print::print(const colors& color, const std::vector<std::string>& yourVector) {
	std::string temp = "[ ";
	auto it = yourVector.begin();
	for (const std::string& str : yourVector) {
		if (it == yourVector.begin())
			temp += "\'" + str + "\'";
		else
			temp += ", \'" + str + "\'";
		it++;
	}
	temp += " ]";
	if (color == colors::black) { std::cout << dye::black(temp); }
	else if (color == colors::blue) { std::cout << dye::blue(temp); }
	else if (color == colors::green) { std::cout << dye::green(temp); }
	else if (color == colors::aqua) { std::cout << dye::aqua(temp); }
	else if (color == colors::red) { std::cout << dye::red(temp); }
	else if (color == colors::purple) { std::cout << dye::purple(temp); }
	else if (color == colors::yellow) { std::cout << dye::yellow(temp); }
	else if (color == colors::white) { std::cout << dye::white(temp); }
	else if (color == colors::grey) { std::cout << dye::grey(temp); }
	else if (color == colors::light_blue) { std::cout << dye::light_blue(temp); }
	else if (color == colors::light_green) { std::cout << dye::light_green(temp); }
	else if (color == colors::light_aqua) { std::cout << dye::light_aqua(temp); }
	else if (color == colors::light_red) { std::cout << dye::light_red(temp); }
	else if (color == colors::light_purple) { std::cout << dye::light_purple(temp); }
	else if (color == colors::light_yellow) { std::cout << dye::light_yellow(temp); }
	else if (color == colors::bright_white) { std::cout << dye::bright_white(temp); }
}

core::print::print(const colors& color, const std::map<std::string, std::string>& yourMap) {
	std::string temp = "[ ";
	auto it = yourMap.begin();
	for (const auto& str : yourMap) {
		if (it == yourMap.begin())
			temp += "[ " + str.first + ": \'" + str.second + "\']";
		else
			temp += ", [ " + str.first + ": \'" + str.second + "\']";
		it++;
	}
	temp += " ]";
	if (color == colors::black) { std::cout << dye::black(temp); }
	else if (color == colors::blue) { std::cout << dye::blue(temp); }
	else if (color == colors::green) { std::cout << dye::green(temp); }
	else if (color == colors::aqua) { std::cout << dye::aqua(temp); }
	else if (color == colors::red) { std::cout << dye::red(temp); }
	else if (color == colors::purple) { std::cout << dye::purple(temp); }
	else if (color == colors::yellow) { std::cout << dye::yellow(temp); }
	else if (color == colors::white) { std::cout << dye::white(temp); }
	else if (color == colors::grey) { std::cout << dye::grey(temp); }
	else if (color == colors::light_blue) { std::cout << dye::light_blue(temp); }
	else if (color == colors::light_green) { std::cout << dye::light_green(temp); }
	else if (color == colors::light_aqua) { std::cout << dye::light_aqua(temp); }
	else if (color == colors::light_red) { std::cout << dye::light_red(temp); }
	else if (color == colors::light_purple) { std::cout << dye::light_purple(temp); }
	else if (color == colors::light_yellow) { std::cout << dye::light_yellow(temp); }
	else if (color == colors::bright_white) { std::cout << dye::bright_white(temp); }
}