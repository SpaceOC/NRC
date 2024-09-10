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
#ifndef NRC_EXTRA_VARIABLES_H_
#define NRC_EXTRA_VARIABLES_H_
#include <iostream>
#include <string>
#include <functional>
#include <map>

namespace core {
    struct variableData {
        std::string description;
        std::function<void()> function;
    };

    class systemVariables {
        private:
            static inline std::map<std::string, variableData> systemVariablesData;
        public:
            virtual std::map<std::string, variableData> getVariable(std::string name) const;
            virtual void sendVariable(std::string variable) const;
            virtual void addSystemVar(std::string name, std::string description = "", std::function<void()> function = []{}) const;
            virtual std::map<std::string, std::string> getAllVars() const;
    };
}

#endif