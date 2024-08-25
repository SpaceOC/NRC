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
#pragma once
#ifndef NRC_BASE_PRINT_H_
#define NRC_BASE_PRINT_H_
#include <string>
#include <vector>
#include <map>

namespace core {
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
            print(const std::string& message);
            print(const colors& color, const std::string& message);

            print(const colors& color, const std::vector<std::string>& yourVector);
            //template<typename T>
            //print(const colors& color, const std::vector<T>& vector); // hard

            print(const colors& color, const std::map<std::string, std::string>& yourMap);
            //template<typename T, typename A>
            //print(const colors& color, const std::map<T, A>& map); // hard
    };
}

#endif