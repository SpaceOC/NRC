#include <iostream>
#include <string>
#include <chrono>
#include <atomic>
#include "Core/base/print.h"
#include "Core/extra/variables.h"

extern std::atomic<int> atomicI;
extern void hi(); // from commands.hpp

void I() {
    core::print(core::print::colors::green, "i: " + std::to_string(atomicI) + "\n");
}

void addVariables() {
    core::systemVariables sv;

    sv.addSystemVar("test1", "test", []{
        std::cout << "hello world" << std::endl;
    });

    sv.addSystemVar("test2", "hello", hi);
    sv.addSystemVar("test3", "i", I);
}
