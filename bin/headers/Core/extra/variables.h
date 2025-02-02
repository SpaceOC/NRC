#ifndef NRC_EXTRA_VARIABLES_H_
#define NRC_EXTRA_VARIABLES_H_
#include <sstream>
#include <iostream>
#include <string>
#include <functional>
#include <map>
#include <type_traits>
#include <cassert>
#include "Core/base/print.h"
#include "Core/base/users/permissions_enum_class.h"
#include "Core/base/command/handler_commands.h"
#include "Core/experimental/run_js_code.h"
#include "Core/base/utils.h"
#include "Core/extra/variable_type_enum.h"

namespace core {
    // Stores a name, type and function.
    struct VariableData {
        std::string name;
        VariableType type;
        bool outputReturn;
        core::Permissions permissionsRun = core::Permissions::user;
        std::string username;
        std::string commandOrCode;
        std::function<std::string(VariableData)> function;

        friend std::ostream& operator<<(std::ostream& os, const VariableData& vd) {
            return os << "{ Name: " << vd.name << ", Type: " << (vd.type == VariableType::FUNC ? "Function" : (vd.type == VariableType::COMMAND ? "Command" : "JavaScript code")) << " }";
        }
    };

    class VariablesManager {
        private:
            std::vector<VariableData> data;
        protected:
            bool isSystem;
        public:
			VariablesManager() = default;
            VariablesManager(bool a) : isSystem(a) {};
			VariablesManager(VariablesManager&) = delete;
			VariablesManager(const VariablesManager&&) = delete;

            // Returns the data of the variable.
            VariableData getVariable(std::string_view name);

            bool exists(std::string_view name);

            // Runs a variable function.
            void start(std::string_view variableName);
            void start(std::string_view variableName, std::string& str);

            void rename(const std::string& oldName, const std::string& newName);

            void addVar(std::string name, VariableType type, core::Permissions permissionsRun, const std::string& f, const std::string& username, bool outputReturn = false);
            void addVar(std::string name, VariableType type, core::Permissions permissionsRun, const std::function<std::string(VariableData)>& f, const std::string& username, bool outputReturn = false);

            // Returns all variables.
            std::vector<core::VariableData> getAllVars();

            friend std::ostream& operator<<(std::ostream& os, VariablesManager& vm) {
                return os << "[VariablesManager] { " << core::Utils::vectorToString(vm.getAllVars()) <<  " }";
            }
    };

    extern VariablesManager* systemVariablesManager();
}

#endif