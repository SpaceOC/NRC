#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <chrono>
#include "Core/extra/variables.h"
#include "OS/other/vars.hpp"
#include "OS/other/commands.hpp"
#include "OS/other/process.hpp"
#include "OS/other/pfs.hpp"
#include "OS/other/events.hpp"

bool work = true;

void commandsZone() {
    while(work) {
        core::print(">>> ");
        std::string userInputResult;
        
        while (!(std::cin >> std::ws)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
        getline(std::cin, userInputResult);
        std::vector<core::CommandObject> parsedCommands = core::handlerCommands()->parsing(userInputResult);

        for (const core::CommandObject& command : parsedCommands)
            core::handlerCommands()->sendCommand(command);
    }
}

std::atomic<int> atomicI;

void OS() {
    std::cout << "Welcome to <Your pseudo-OS name>!" << '\n';
    addCommands();
    addVariables();
    addFolderAndFiles();
    addEvents();
    startProcess();
}