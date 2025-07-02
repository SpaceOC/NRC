#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include "Core/print/print.h"
#include "Core/command/handler_commands.h"
#include "Core/command/command_structs.h"
#include "Core/main.h"

extern core::main* nrc;

namespace core {
class User;
}

// Да, я взял код counter.cpp прямиком из SpaceDOS и немного ... переписал... переделал.... 
std::string counter(core::User*, core::CommandObject*) {
	int a = 0;
	int b = 0;

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
		return "";
	}

	if (b >= 1000000 || b <= -1000000) {
		std::string yourChoice;
		core::print("Attention! This number is huge and it will take a long time to finish working with such a number. Do you want to continue? (Y/N) \t", core::PrintColors::yellow);
		std::cin >> yourChoice;
		if (yourChoice == "N") {
			return "";
		}
	}
	std::cout << "----------------------------------------------------------" << std::endl;
	
	if (!std::filesystem::exists("Texts"))
		std::filesystem::create_directory("Texts");
	std::ofstream file("Texts/counter.txt", std::ios::out);

	if (file) {
		while (a != b) {
			a++;
			std::cout << a << '\n';
			file << a << '\n';
		}
		file.close();
	} else {
		core::print("Error!\n", core::PrintColors::red);
		return "";
	}

	core::print("----------------------------------------------------------\n");
	core::print("It's done! The file is saved in the Texts folder under the name \"counter.txt\"\n", core::PrintColors::green);
	std::string yourChoice;
	core::print("Exit? (Y/N) \t");
	std::cin >> yourChoice;
	if (yourChoice == "N") { return counter(nullptr, nullptr); } 
	return "";
}

std::string bye(core::User*, core::CommandObject*) {
	nrc->stopWork();
	return "";
}

std::string helloC(core::User*, core::CommandObject*) {
	srand(static_cast<unsigned int>(time(0)));
	std::vector<std::string> messages = {"hi", "HII", "hello :D", "hiiiiii"};
	return messages[rand() % (messages.size() - 1)];
}

void addCommands() {
	core::handlerCommands()->addCommand("counter", "No description", counter);
	core::handlerCommands()->addCommand("hi", "hiii!", helloC);
	core::handlerCommands()->addCommand("exit", "exit", bye);
}