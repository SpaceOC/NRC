#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <map>
#include <vector>

class systemVariables {
    private:
        static inline std::map<std::string, std::string> systemVariablesData, localVariablesData;
        static inline std::map<std::string, std::function<void()>> systemVariablesFunction, localVariablesFunction;
    public:
        virtual std::string getVariable(std::string VarName) const {
            if (systemVariablesData.size() == 0) {

            }
            else {
                try
                {
                    return systemVariablesData[VarName];
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                }
            }
        }

        virtual void sendVariable(std::string variable) const {
            auto it = systemVariablesFunction.find(variable);
            if (it != systemVariablesFunction.end()) {
                it->second();
            }
        }

        virtual void addSystemVar(std::string Name, std::string Var, std::function<void()> VarFunction) const {
            try {
                systemVariablesData[Name] = "%" + Var + "%";
                systemVariablesFunction["%" + Var + "%"] = VarFunction;
                allVars();
            }
            catch(const std::exception& e) {
                std::cerr << e.what() << '\n';
            }
        }
        
        virtual void allVars() const {
            for (auto elements : systemVariablesData) {
                std::cout << "Variable name ---> " << elements.first << " | Var ---> " << elements.second << std::endl;
            }
        }
};