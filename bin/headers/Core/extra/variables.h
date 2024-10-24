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
    // Stores a description and a function.
    struct VariableData {
        std::string description;
        std::function<void()> function;
    };

    class SystemVariablesManager {
        private:
            static inline std::map<std::string, VariableData> data;
        public:
            // Returns the data of the variable.
            static std::map<std::string, VariableData> getVariable(std::string name);

            // Runs a variable function.
            static void sendVariable(std::string variable);

            // Runs a variable function.
            static void addSystemVar(std::string name, std::string description = "", std::function<void()> function = []{});

            // Returns all variables.
            static std::map<std::string, std::string> getAllVars();
    };
}

#endif