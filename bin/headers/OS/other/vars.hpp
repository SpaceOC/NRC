#include <iostream>
#include <string>
#include <chrono>
#include <atomic>
#include "Core/base/print.h"
#include "Core/extra/variables.h"

extern std::atomic<int> atomicI;
extern void hi(); // from commands.hpp

void I() {
    core::print(core::colors::green, "i: " + std::to_string(atomicI) + "\n");
}

void addVariables() {
    core::SystemVariablesManager::addSystemVar("test1", "test", []{
        std::cout << "hello world" << std::endl;
    });

    core::SystemVariablesManager::addSystemVar("test2", "hello", hi);
    core::SystemVariablesManager::addSystemVar("test3", "i", I);
}
