#include <string>
#include <vector>

#include "Core/command/command_sender.h"

#include "Core/print/print.h"
#include "Core/command/commands_handler.h"
#include "Core/command/command_structs.h"
#include "Core/filesystem/pseudo_fs.h"
#include "Core/filesystem/nrfs.h"
#include "Core/users/user_manager.h"
#include "Core/users/user.h"

void core::CommandSenderBasic::zone() const {
	core::print(">>> ");
	std::string userInputResult;
	while (!(std::cin >> std::ws)) {
		std::cin.clear();
		std::cin.ignore(10000, '\n');
	}
	std::getline(std::cin, userInputResult);
	try {
		core::User* who = &core::userManager()->getCurrentUserData();
		std::vector<core::CommandObject> parsedCommands = core::commandsHandler()->parser()->parse(userInputResult, true, who);
		for (const core::CommandObject& command : parsedCommands) {
			if (command.returnable) {
				std::string fileOutput;
				core::commandsHandler()->sendCommand(command, fileOutput);

				if (!command.whereOutput.empty() && core::pseudoFS()->isFile(command.whereOutput)) {
					std::string path = (command.whereOutput._Starts_with("./")
						|| (
							core::pseudoFS()->getNRFS()->getDiskFromLetter(command.whereOutput.at(0)) != 1001
							&& command.whereOutput.substr(1, 2) == ":/"
						)
						? command.whereOutput :
						core::pseudoFS()->currentPath() + command.whereOutput);
						
					size_t newDisk = core::pseudoFS()->getNRFS()->getDiskFromLetter(command.whereOutput.at(0));
					size_t curDisk = (
						(newDisk != 1001 && command.whereOutput.substr(1, 2) == ":/")
						? newDisk
						: core::pseudoFS()->currentDiskId()
					);

					core::pseudoFS()->createFile(path, curDisk);
					core::pseudoFS()->setFileAtt(path, curDisk, "content", fileOutput);
					core::pseudoFS()->getNRFS()->saveData();
				}
			}
			else
				core::commandsHandler()->sendCommand(command);
		}
	}
	catch(const std::exception& e) {
		std::cerr << e.what() << '\n';
		_lastMessageError = e.what();
	}

}