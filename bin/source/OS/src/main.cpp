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
        core::print("ERROR: ", core::PrintColors::red);
        core::print(e.what(), core::PrintColors::red);
    }
    return 0;
}