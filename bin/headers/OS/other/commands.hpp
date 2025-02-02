#include <iostream>
#include <vector>
#include <map>
#include "Core/base/print.h"
#include "Core/base/command/handler_commands.h"
#include "Core/base/data/file_manager.h"

extern bool work;

class core::User;

// Да, я взял код counter.cpp прямиком из SpaceDOS и немного ... переписал... переделал.... 
std::string counter(core::User*, bool) {
    int a, b = 0;
    core::print("Min number: ");

    while(!(std::cin >> a)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        core::print("Min number: ");
    } 

    a--;
    core::print("Max number: ");

    while(!(std::cin >> b)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        core::print("Max number: ");
    }
    
    if (a > b) {
        core::print("The minimum number cannot be greater than the maximum number!!!\n", core::PrintColors::red);
        return;
    }

    if (b >= 1000000 || b <= -1000000) {
        std::string yourChoice;
        core::print("Attention! This number is huge and it will take a long time to finish working with such a number. Do you want to continue? (Y/N) \t", core::PrintColors::yellow);
        std::cin >> yourChoice;
        if (yourChoice == "N") {
            return;
        }
    }
    std::cout << "----------------------------------------------------------" << std::endl;
    core::FileManager FM;
    FM.createFolder("Texts");
    FM.createFile("Texts/counter.txt");
    // "Ладно, и так сойдёт" :skull: :cold_face:
    auto file = FM.getFileOfstream("Texts/counter.txt", std::ios::out);

    if (file) {
        while (a != b) {
            a++;
            std::cout << a << '\n';
            file << a << '\n';
        }
        file.close();
    } else {
        core::print("Error!\n", core::PrintColors::red);
        return;
    }

    core::print("----------------------------------------------------------\n");
    core::print("It's done! The file is saved in the Texts folder under the name \"counter.txt\"\n", core::PrintColors::green);
    std::string yourChoice;
    core::print("Exit? (Y/N) \t");
    std::cin >> yourChoice;
    if (yourChoice == "N") { return counter(nullptr, false); } 
    return "";
}

std::string bye(core::User*, bool) {
    work = false;
    return "";
}

std::string hello(core::User*, bool returnable) {
    srand(static_cast<unsigned int>(time(0)));
    std::vector<std::string> messages = {"hi", "HII", "hello :D", "hiiiiii"};
    if (!returnable)
        core::print(messages[rand() % (messages.size() - 1)] + '\n');
    return messages[rand() % (messages.size() - 1)];
}

void addCommands() {
    core::handlerCommands()->addCommand("counter", "No description", counter);
    core::handlerCommands()->addCommand("hi", "hiii!", hello);
    core::handlerCommands()->addCommand("exit", "exit", bye);
}