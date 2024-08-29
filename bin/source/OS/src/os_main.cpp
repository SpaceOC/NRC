#include <iostream>
#include <map>
#include <string>
#include <chrono>
#include "Core/extra/variables.h"
#include "other/vars.hpp"
#include "other/commands.hpp"
#include "other/process.hpp"

bool work = true;

void commandsZone() {
    core::handlerCommands hc;
    while(work) {
        core::print(">>> ");
        std::string userInputResult;
        
        while (!(std::cin >> std::ws)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
        getline(std::cin, userInputResult);
        std::vector<std::string> parsed = hc.parsing(userInputResult);
        hc.sendCommand(userInputResult, parsed);
    }
}

std::atomic<int> atomicI;

void OS() {
    std::cout << "Welcome to <Your pseudo-OS name>!" << '\n';
    addCommands();
    addVariables();
    startProcess();
}