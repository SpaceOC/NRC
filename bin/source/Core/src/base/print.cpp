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

const std::string core::ColorsSequence::black   = "\033[0;30m";
const std::string core::ColorsSequence::red     = "\033[0;31m";
const std::string core::ColorsSequence::green   = "\033[0;32m";
const std::string core::ColorsSequence::yellow  = "\033[0;33m";
const std::string core::ColorsSequence::blue    = "\033[0;34m";
const std::string core::ColorsSequence::purple  = "\033[0;35m";
const std::string core::ColorsSequence::aqua    = "\033[0;36m";
const std::string core::ColorsSequence::white   = "\033[0;37m";
const std::string core::ColorsSequence::grey    = "\033[1;30m";
const std::string core::ColorsSequence::light_red     = "\033[1;31m";
const std::string core::ColorsSequence::light_green   = "\033[1;32m";
const std::string core::ColorsSequence::light_yellow  = "\033[1;33m";
const std::string core::ColorsSequence::light_blue    = "\033[1;34m";
const std::string core::ColorsSequence::light_purple  = "\033[1;35m";
const std::string core::ColorsSequence::light_aqua    = "\033[1;36m";
const std::string core::ColorsSequence::bright_white  = "\033[1;37m";

std::string core::getColorEscapeSequence(const PrintColors& color) {
    switch (color) {
        case PrintColors::black:
            return core::ColorsSequence::black;
            break;
        case PrintColors::blue:
            return core::ColorsSequence::blue;
            break;
        case PrintColors::green:
            return core::ColorsSequence::green;
            break;
        case PrintColors::aqua:
            return core::ColorsSequence::aqua;
            break;
        case PrintColors::red:
            return core::ColorsSequence::red;
            break;
        case PrintColors::purple:
            return core::ColorsSequence::purple;
            break;
        case PrintColors::yellow:
            return core::ColorsSequence::yellow;
            break;
        case PrintColors::white:
            return core::ColorsSequence::white;
            break;
        case PrintColors::grey:
            return core::ColorsSequence::grey;
            break;
        case PrintColors::light_blue:
            return core::ColorsSequence::light_blue;
            break;
        case PrintColors::light_green:
            return core::ColorsSequence::light_green;
            break;
        case PrintColors::light_aqua:
            return core::ColorsSequence::light_aqua;
            break;
        case PrintColors::light_red:
            return core::ColorsSequence::light_red;
            break;
        case PrintColors::light_purple:
            return core::ColorsSequence::light_purple;
            break;
        case PrintColors::light_yellow:
            return core::ColorsSequence::light_yellow;
            break;
        case PrintColors::bright_white:
            return core::ColorsSequence::bright_white;
            break;
        default:
            break;
    }
    return ""; // Default to no color
}

void core::print() {
	std::cout << '\n';
}

void core::print(const std::string& message, const PrintColors& color) {
	std::string escapeSequence = core::getColorEscapeSequence(color);
    std::cout << escapeSequence << message << "\033[0m";
}

void core::print(PrintableClass &yourClass, const PrintColors& color) {
	std::string escapeSequence = core::getColorEscapeSequence(color);
    std::cout << escapeSequence << yourClass << "\033[0m";
}

/*
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
*/