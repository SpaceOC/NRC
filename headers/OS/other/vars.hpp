#include <iostream>
#include <string>
#include <chrono>
#include <atomic>
#include "Core/print/print.h"
#include "Core/other/variables.h"
#include "Core/command/command_structs.h"
#include "Core/users/user_permissions_enum.h"

namespace core {
class User;
}

extern std::string helloC(core::User*, core::CommandObject*); // from commands.hpp

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
		core::UserPermissions::ROOT,
		[](core::VariableData) -> std::string {
			std::cout << "Hello, World!" << std::endl;
			return "";
		},
		""
	);

	core::systemVariablesManager()->addVar("test2",
		core::VariableType::FUNC,
		core::UserPermissions::ROOT,
		[](core::VariableData) -> std::string {
			std::cout << helloC(nullptr, nullptr) << '\n';
			return "";
		},
		""
	);
}
