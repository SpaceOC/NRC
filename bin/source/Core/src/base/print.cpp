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
#include "Core/base/print.h"

const std::string core::print::colorsSequence::black   = "\033[0;30m";
const std::string core::print::colorsSequence::red     = "\033[0;31m";
const std::string core::print::colorsSequence::green   = "\033[0;32m";
const std::string core::print::colorsSequence::yellow  = "\033[0;33m";
const std::string core::print::colorsSequence::blue    = "\033[0;34m";
const std::string core::print::colorsSequence::purple  = "\033[0;35m";
const std::string core::print::colorsSequence::aqua    = "\033[0;36m";
const std::string core::print::colorsSequence::white   = "\033[0;37m";
const std::string core::print::colorsSequence::grey    = "\033[1;30m";
const std::string core::print::colorsSequence::light_red     = "\033[1;31m";
const std::string core::print::colorsSequence::light_green   = "\033[1;32m";
const std::string core::print::colorsSequence::light_yellow  = "\033[1;33m";
const std::string core::print::colorsSequence::light_blue    = "\033[1;34m";
const std::string core::print::colorsSequence::light_purple  = "\033[1;35m";
const std::string core::print::colorsSequence::light_aqua    = "\033[1;36m";
const std::string core::print::colorsSequence::bright_white  = "\033[1;37m";

std::string core::print::getColorEscapeSequence(const colors& color) {
    if (color == colors::black) return core::print::colorsSequence::black;
    else if (color == colors::blue) return core::print::colorsSequence::blue;
    else if (color == colors::green) return core::print::colorsSequence::green;
    else if (color == colors::aqua) return core::print::colorsSequence::aqua;
    else if (color == colors::red) return core::print::colorsSequence::red;
    else if (color == colors::purple) return core::print::colorsSequence::purple;
    else if (color == colors::yellow) return core::print::colorsSequence::yellow;
    else if (color == colors::white) return core::print::colorsSequence::white;
    else if (color == colors::grey) return core::print::colorsSequence::grey;
    else if (color == colors::light_blue) return core::print::colorsSequence::light_blue;
	else if (color == colors::light_green) return core::print::colorsSequence::light_green;
    else if (color == colors::light_aqua) return core::print::colorsSequence::light_aqua;
    else if (color == colors::light_red) return core::print::colorsSequence::light_red;
    else if (color == colors::light_purple) return core::print::colorsSequence::light_purple;
    else if (color == colors::light_yellow) return core::print::colorsSequence::light_yellow;
	else if (color == colors::bright_white) return core::print::colorsSequence::bright_white;
    return ""; // Default to no color
}

core::print::print() {
	std::cout << '\n';
}

core::print::print(const std::string& message) {
	std::cout << message;
}

core::print::print(const colors& color, const std::string& message) {
	std::string escapeSequence = core::print::getColorEscapeSequence(color);
    std::cout << escapeSequence << message << "\033[0m";
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
	core::print(color, temp);
}

core::print::print(const colors& color, const std::vector<int>& yourVector) {
	std::string temp = "[ ";
	auto it = yourVector.begin();
	for (const int& str : yourVector) {
		if (it == yourVector.begin())
			temp += std::to_string(str);
		else
			temp += ", " + std::to_string(str);
		it++;
	}
	temp += " ]";
	core::print(color, temp);
}

core::print::print(const colors& color, const std::vector<float>& yourVector) {
	std::string temp = "[ ";
	auto it = yourVector.begin();
	for (const float& str : yourVector) {
		if (it == yourVector.begin())
			temp += std::to_string(str);
		else
			temp += ", " + std::to_string(str);
		it++;
	}
	temp += " ]";
	core::print(color, temp);
}

core::print::print(const colors& color, const std::vector<double>& yourVector) {
	std::string temp = "[ ";
	auto it = yourVector.begin();
	for (const double& str : yourVector) {
		if (it == yourVector.begin())
			temp += std::to_string(str);
		else
			temp += ", " + std::to_string(str);
		it++;
	}
	temp += " ]";
	core::print(color, temp);
}

core::print::print(const colors& color, const std::vector<char>& yourVector) {
	std::string temp = "[ ";
	auto it = yourVector.begin();
	for (const char& str : yourVector) {
		if (it == yourVector.begin()) {
			temp += "\'";
			temp += str;
			temp += "\'";
		}
		else {
			temp += ", \'";
			temp += str;
			temp += "\'";
		}
		it++;
	}
	temp += " ]";
	core::print(color, temp);
}

core::print::print(const colors& color, const std::vector<bool>& yourVector) {
	std::string temp = "[ ";
	auto it = yourVector.begin();
	for (const bool& str : yourVector) {
		std::string trueOrFalse = (str ? "true" : "false");
		if (it == yourVector.begin()) {
			temp += trueOrFalse;
		}
		else {
			temp += ", " + trueOrFalse;
		}
		it++;
	}
	temp += " ]";
	core::print(color, temp);
}

core::print::print(const colors& color, const std::map<std::string, std::string>& yourMap) {
	std::string temp = "[ ";
	auto it = yourMap.begin();
	for (const auto& str : yourMap) {
		if (it == yourMap.begin())
			temp += "[ '" + str.first + "': '" + str.second + "']";
		else
			temp += ", [ '" + str.first + "': '" + str.second + "']";
		it++;
	}
	temp += " ]";
	core::print(color, temp);
}

core::print::print(const colors& color, const std::map<std::string, int>& yourMap) {
	std::string temp = "[ ";
	auto it = yourMap.begin();
	for (const auto& str : yourMap) {
		if (it == yourMap.begin())
			temp += "[ '" + str.first + "': " + std::to_string(str.second) + "]";
		else
			temp += ", [ '" + str.first + "': " + std::to_string(str.second) + "]";
		it++;
	}
	temp += " ]";
	core::print(color, temp);
}

core::print::print(const colors& color, const std::map<std::string, float>& yourMap) {
	std::string temp = "[ ";
	auto it = yourMap.begin();
	for (const auto& str : yourMap) {
		if (it == yourMap.begin())
			temp += "[ '" + str.first + "': " + std::to_string(str.second) + "]";
		else
			temp += ", [ '" + str.first + "': " + std::to_string(str.second) + "]";
		it++;
	}
	temp += " ]";
	core::print(color, temp);
}

core::print::print(const colors& color, const std::map<std::string, double>& yourMap) {
	std::string temp = "[ ";
	auto it = yourMap.begin();
	for (const auto& str : yourMap) {
		if (it == yourMap.begin())
			temp += "[ '" + str.first + "': " + std::to_string(str.second) + "]";
		else
			temp += ", [ '" + str.first + "': " + std::to_string(str.second) + "]";
		it++;
	}
	temp += " ]";
	core::print(color, temp);
}

core::print::print(const colors& color, const std::map<std::string, char>& yourMap) {
	std::string temp = "[ ";
	auto it = yourMap.begin();
	for (const auto& str : yourMap) {
		if (it == yourMap.begin())
			temp += "[ '" + str.first + "': \'" + str.second + "\']";
		else
			temp += ", [ '" + str.first + "': \'" + str.second + "\']";
		it++;
	}
	temp += " ]";
	core::print(color, temp);
}

core::print::print(const colors& color, const std::map<std::string, bool>& yourMap) {
	std::string temp = "[ ";
	auto it = yourMap.begin();
	for (const auto& str : yourMap) {
		std::string trueOrFalse = (str.second ? "true" : "false");
		if (it == yourMap.begin())
			temp += "[ '" + str.first + "': " + trueOrFalse + "]";
		else
			temp += ", [ '" + str.first + "': " + trueOrFalse + "]";
		it++;
	}
	temp += " ]";
	core::print(color, temp);
}