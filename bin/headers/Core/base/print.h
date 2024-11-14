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
#ifndef NRC_BASE_PRINT_H_
#define NRC_BASE_PRINT_H_
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <type_traits>
#include <any>
#include "Core/base/printable.h"

#define DEFAULT_PRINT_COLOR     core::PrintColors::white

template<typename T>
using is_not_printable_class = std::enable_if_t<!std::is_base_of_v<core::PrintableClass, T>, bool>;

namespace core {
    // colors for the print class
    enum PrintColors {
        black, white, blue, yellow,
        red, aqua, purple, green, grey,
        light_blue, light_green, light_aqua, light_red,
        light_purple, light_yellow, bright_white 
    };

    class ColorsSequence {
        public:
            static const std::string black, red, green, yellow, blue;
            static const std::string purple, aqua, white, grey;
            static const std::string light_red, light_green, light_yellow, light_blue;
            static const std::string light_purple, light_aqua,bright_white;
    };

    std::string getColorEscapeSequence(const PrintColors& color);

    // for print
    template<typename T>
    std::string valueToString(const T& value) {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }

    void print();
    void print(const std::string& message, const PrintColors& color = DEFAULT_PRINT_COLOR);
    template<typename T, typename = is_not_printable_class<T>>
    void print(const T& message, const PrintColors& color = DEFAULT_PRINT_COLOR) {
        std::string escapeSequence = core::getColorEscapeSequence(color);
        std::cout << escapeSequence << message << "\033[0m";
    }
    template<typename T, typename = is_not_printable_class<T>>
    void print(const std::vector<T> yourVector, const PrintColors& color = DEFAULT_PRINT_COLOR) {
        std::string escapeSequence = core::getColorEscapeSequence(color);
        std::string temp = escapeSequence + "[ ";
        auto it = yourVector.begin();
        for (const T& str : yourVector) {
            if (it == yourVector.begin())
                temp += valueToString(str);
            else
                temp += ", " + valueToString(str);
            it++;
        }
        temp += " ]";
        std::cout << temp << "\033[0m";
    }
    template<typename T, typename A, typename = is_not_printable_class<A>, typename = is_not_printable_class<T>>
    void print(const std::map<T, A> yourMap, const PrintColors& color = DEFAULT_PRINT_COLOR) {
        std::string escapeSequence = core::getColorEscapeSequence(color);
        std::string temp = escapeSequence + "[ ";
        auto it = yourMap.begin();
        for (const auto& str : yourMap) {
            if (it == yourMap.begin())
                temp += "[ '" + valueToString(str.first) + "': '" + valueToString(str.second) + "']";
            else
                temp += ", [ '" + valueToString(str.first) + "': '" + valueToString(str.second) + "']";
            it++;
        }
        temp += " ]";
        std::cout << temp << "\033[0m";
    }
    void print(PrintableClass &yourClass, const PrintColors& color = DEFAULT_PRINT_COLOR);
}

#endif