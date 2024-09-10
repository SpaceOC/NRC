#include <iostream>
#include "Core/main.h"
#include "Core/base/print.h"
extern void OS();

int main() {
    try
    {
        core::main start;
        OS();
    }
    catch(const std::exception& e)
    {
        core::print(core::colors::red, "ERROR: ");
        core::print(core::colors::red, e.what());
    }
    return 0;
}