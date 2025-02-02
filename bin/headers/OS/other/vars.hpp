#include <iostream>
#include <string>
#include <chrono>
#include <atomic>
#include "Core/base/print.h"
#include "Core/extra/variables.h"

class core::User;

extern std::atomic<int> atomicI;
extern std::string hello(core::User*, bool); // from commands.hpp

void addVariables() {
    /*
    core::SystemVariablesManager::addSystemVar("test1", "test", []{
        std::cout << "hello world" << std::endl;
    });

    core::SystemVariablesManager::addSystemVar("test2", "hello", hi);
    core::SystemVariablesManager::addSystemVar("test3", "i", I);
    */
    core::systemVariablesManager()->addVar("test1",
        core::VariableType::FUNC,
        core::Permissions::root,
        [](core::VariableData) -> std::string {
            std::cout << "Hello, World!" << std::endl;
            return "";
        },
        ""
    );

    core::systemVariablesManager()->addVar("test2",
        core::VariableType::FUNC,
        core::Permissions::root,
        [](core::VariableData) -> std::string {
            hello(nullptr, false);
            return "";
        },
        ""
    );

    core::systemVariablesManager()->addVar("test3",
        core::VariableType::FUNC,
        core::Permissions::root,
        [](core::VariableData) -> std::string {
            core::print(core::PrintColors::green, "i: " + std::to_string(atomicI) + "\n");
            return "";
        },
        ""
    );
}
