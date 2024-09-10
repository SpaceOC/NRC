#include <iostream>
#include <map>
#include <string>
#include <vector>
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
        std::vector<core::CommandObject> parsedCommands = hc.parsing(userInputResult);
        for (const core::CommandObject& command : parsedCommands) {
            HC.sendCommand(command);
        }
    }
}

std::atomic<int> atomicI;

void OS() {
    std::cout << "Welcome to <Your pseudo-OS name>!" << '\n';
    addCommands();
    addVariables();
    startProcess();
}