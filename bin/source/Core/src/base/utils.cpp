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
#include <sstream>
#include "Core/base/utils.h"

core::Utils::Utils() {}

bool core::Utils::stringIsNumbers(const std::string& content) {
    for (const auto& letter : content) {

        if (letter == '-' && letter == content.at(0))
            continue;  
        else if (!isdigit(letter))
            return false;
    }
    return true;
}

std::vector<std::string> core::Utils::split(const std::string& content, const char what) {
    if (content.empty())
        return {};

    std::vector<std::string> result;
    std::string temp;
    std::istringstream tokenStream(content);
    while (std::getline(tokenStream, temp, what))
        result.push_back(temp);
    return result;
}