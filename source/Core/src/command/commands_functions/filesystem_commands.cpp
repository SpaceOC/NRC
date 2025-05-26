#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <map>
#include "Core/print/print.h"
#include "Core/print/gprint.h"
#include "Core/command/commands.h"
#include "Core/command/handler_commands.h"
#include "Core/command/command_structs.h"
#include "Core/users/user_manager.h"
#include "Core/users/user.h"
#include "Core/filesystem/pseudo_fs.h"
#include "Core/filesystem/nrfs.h"
#include "Core/other/utils.h"

std::string core::commands::CORE_COMMAND_cd(core::User*, core::CommandObject* thisObj) {
	if (thisObj->args.at(0).empty()) {
		return core::gprint("Command error: The first argument cannot be empty!\n", core::PrintColors::red);
	}
	else {
		std::string where = (thisObj->args.at(0) == ".." && !thisObj->args.at(0)._Starts_with("./") ? ".." : thisObj->args.at(0));
		
		int code;
		size_t curDisk = core::pseudoFS()->getCurDiskId();
		FolderData* folder = (where == ".." ? nullptr :
			new FolderData(core::pseudoFS()->getFolderData(
				(
				(thisObj->args.at(0)._Starts_with("./") ? thisObj->args.at(0) : core::pseudoFS()->getCurrentPath() + where)
				),
				curDisk,
				code
			)
		));
		if ((where != ".." || thisObj->args.at(0)._Starts_with("./"))) {
			if (folder && folder->link != nullptr && !folder->linkPath.empty()) {
				if (!core::pseudoFS()->changePath(folder->linkPath, curDisk)) {
					return core::gprint("Error: Change path operation failed!\n", core::PrintColors::red);
				}
			}
			else {
				if (!core::pseudoFS()->changeDirectory(where, curDisk)) {
					return core::gprint("Error: Change directory operation failed!\n", core::PrintColors::red);
				}
			}
		}
		else {
			if (!core::pseudoFS()->changeDirectory(where, curDisk)) {
				return core::gprint("Error: Change directory operation failed!\n", core::PrintColors::red);
			}
		}
	}
	
	return "";
}

std::string core::commands::CORE_COMMAND_createFile(core::User* who, core::CommandObject* thisObj) {
	if (thisObj->args.at(0).empty()) {
		return core::gprint("Command error: The first argument cannot be empty!\n", core::PrintColors::red);
	}

	std::string checkResult = core::checkUserPermissionsForCommand(who);
	COMMAND_ERROR_OUTPUT(checkResult)

	std::string where = (thisObj->args.at(0)._Starts_with("./") ? thisObj->args.at(0) : core::pseudoFS()->getCurrentPath() + thisObj->args.at(0));
	size_t curDisk = core::pseudoFS()->getCurDiskId();
	int code = core::pseudoFS()->createFile(where, curDisk);
	if (code == core::PseudoFSCodes::ALREADY_EXISTS) {
		FileData file{
			"",
			"",
			std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()),
			std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()),
			false,
			false,
			nullptr,
			""
		};
		fixCreateSamePFSObject(where, curDisk, file, code);
	}
	
	checkResult = checkFileCodeForPFSCommand(code);
	COMMAND_ERROR_OUTPUT(checkResult)

	if (thisObj->args.size() > 1 && thisObj->args.at(1).find("o") != std::string::npos)
		code = core::pseudoFS()->setFileAtt(where, curDisk, "owner", who);
	checkResult = checkFileCodeForPFSCommand(code);
	COMMAND_ERROR_OUTPUT(checkResult)

	core::pseudoFS()->getNRFS()->saveData();
	return "";
}

std::string core::commands::CORE_COMMAND_createLinkFile(core::User* who, core::CommandObject* thisObj) {
	if (thisObj->args.at(0).empty() || thisObj->args.at(1).empty()) {
		return core::gprint("Command error: One of the arguments is empty!\n", core::PrintColors::red);
	}

	std::string checkResult = core::checkUserPermissionsForCommand(who);
	COMMAND_ERROR_OUTPUT(checkResult)

	std::string where = (thisObj->args.at(0)._Starts_with("./") ? thisObj->args.at(0) : core::pseudoFS()->getCurrentPath() + thisObj->args.at(0));
	std::string where2 = (thisObj->args.at(1)._Starts_with("./") ? thisObj->args.at(1) : core::pseudoFS()->getCurrentPath() + thisObj->args.at(1));
	int code;
	size_t curDisk = core::pseudoFS()->getCurDiskId();
	FileData file = core::pseudoFS()->getFileData(where, curDisk, code);
	checkResult = core::checkFileCodeForPFSCommand(code);
	COMMAND_ERROR_OUTPUT(checkResult)

	core::FileData* linkFile = new core::FileData{
		core::Utils::split(where2, '/').back(),
		"",
		0,
		0,
		false,
		file.hidden,
		&file,
		where,
		file.owner
	};

	code = core::pseudoFS()->createFile(where2, curDisk, linkFile);
	if (code == core::PseudoFSCodes::ALREADY_EXISTS)
		fixCreateSamePFSObject(where2, curDisk, *linkFile, code);
	
	checkResult = core::checkFileCodeForPFSCommand(code);
	COMMAND_ERROR_OUTPUT(checkResult)

	core::pseudoFS()->getNRFS()->saveData();
	return "";
}

std::string core::commands::CORE_COMMAND_deleteFile(core::User* who, core::CommandObject* thisObj) {
	if (thisObj->args.at(0).empty()) {
		return core::gprint("Command error: The first argument cannot be empty!\n", core::PrintColors::red);
	}

	std::string checkResult = core::checkUserPermissionsForCommand(who);
	COMMAND_ERROR_OUTPUT(checkResult)

	std::string where = (thisObj->args.at(0)._Starts_with("./") ? thisObj->args.at(0) : core::pseudoFS()->getCurrentPath() + thisObj->args.at(0));
	int code;
	size_t curDisk = core::pseudoFS()->getCurDiskId();
	core::pseudoFS()->getFileData(where, curDisk, code);
	checkResult = core::checkFileCodeForPFSCommand(code);
	COMMAND_ERROR_OUTPUT(checkResult)

	code = core::pseudoFS()->deleteFile(where, curDisk);
	checkResult = core::checkFileCodeForPFSCommand(code);
	COMMAND_ERROR_OUTPUT(checkResult)

	core::pseudoFS()->getNRFS()->saveData();
	return "";
}

std::string core::commands::CORE_COMMAND_renameFile(core::User* who, core::CommandObject* thisObj) {
	if (thisObj->args.at(0).empty() || thisObj->args.at(1).empty()) {
		return core::gprint("Command error: One of the arguments is empty!\n", core::PrintColors::red);
	}

	std::string checkResult = core::checkUserPermissionsForCommand(who);
	COMMAND_ERROR_OUTPUT(checkResult)

	std::string where = (thisObj->args.at(0)._Starts_with("./") ? thisObj->args.at(0) : core::pseudoFS()->getCurrentPath() + thisObj->args.at(0));
	int code;
	size_t curDisk = core::pseudoFS()->getCurDiskId();
	FileData f = core::pseudoFS()->getFileData(where, curDisk, code);
	checkResult = core::checkFileCodeForPFSCommand(code);
	COMMAND_ERROR_OUTPUT(checkResult)

	checkResult = core::checkUserPermissionsForPFSCommand(who, f);
	COMMAND_ERROR_OUTPUT(checkResult)

	code = core::pseudoFS()->renameFile(thisObj->args.at(0), curDisk, thisObj->args.at(1));
	if (code == core::PseudoFSCodes::ALREADY_EXISTS) {
		f.timeEdit = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		fixCreateSamePFSObject(where, curDisk, f, code);
	}

	checkResult = core::checkFileCodeForPFSCommand(code);
	COMMAND_ERROR_OUTPUT(checkResult)

	core::pseudoFS()->getNRFS()->saveData();
	return "";
}

std::string core::commands::CORE_COMMAND_moveFile(core::User* who, core::CommandObject* thisObj) {
	if (thisObj->args.at(0).empty() || thisObj->args.at(1).empty()) {
		return core::gprint("Command error: One of the arguments is empty!\n", core::PrintColors::red);
	}

	std::string checkResult = core::checkUserPermissionsForCommand(who);
	COMMAND_ERROR_OUTPUT(checkResult)

	int code;
	size_t curDisk = core::pseudoFS()->getCurDiskId();
	FileData f = core::pseudoFS()->getFileData(thisObj->args.at(0), curDisk, code);
	checkResult = core::checkFileCodeForPFSCommand(code);
	COMMAND_ERROR_OUTPUT(checkResult)

	checkResult = core::checkUserPermissionsForPFSCommand(who, f);
	COMMAND_ERROR_OUTPUT(checkResult)

	code = core::pseudoFS()->moveFile(thisObj->args.at(0), curDisk, thisObj->args.at(1), curDisk);
	if (code == core::PseudoFSCodes::ALREADY_EXISTS)
		fixCreateSamePFSObject(thisObj->args.at(0), curDisk, f, code);

	checkResult = core::checkFileCodeForPFSCommand(code);
	COMMAND_ERROR_OUTPUT(checkResult)

	core::pseudoFS()->getNRFS()->saveData();
	return "";
}

std::string core::commands::CORE_COMMAND_editFile(core::User* who, core::CommandObject* thisObj) {
	if (thisObj->args.at(0).empty()) {
		return core::gprint("Command error: Arguments is empty!\n", core::PrintColors::red);
	}
	std::string checkResult = core::checkUserPermissionsForCommand(who);
	COMMAND_ERROR_OUTPUT(checkResult)

	int code;
	size_t curDisk = core::pseudoFS()->getCurDiskId();
	FileData f = core::pseudoFS()->getFileData(thisObj->args.at(0), curDisk, code);
	checkResult = core::checkFileCodeForPFSCommand(code);
	COMMAND_ERROR_OUTPUT(checkResult)

	checkResult = core::checkUserPermissionsForPFSCommand(who, f);
	COMMAND_ERROR_OUTPUT(checkResult)

	#ifndef NRC_WEB
	if (!thisObj->returnable) {
		if (thisObj->args.size() < 2) {
			core::print("--- Write ':quit:file' to quit editing the file and save all the contents. ---\n");
			size_t linen = 0;
			std::string text, line;
			while (true) {
				core::print(DEFAULT_PRINT_COLOR, "Line ", linen, " > ");
				std::cin >> line;
				if (line == ":quit:file") break;
				if (!line.empty()) {
					text += line + "\n";
					++linen;
				}
			}
			if (text.empty()) return "";
			core::pseudoFS()->setFileAtt(thisObj->args.at(0), curDisk, "content", text);
			core::pseudoFS()->getNRFS()->saveData();
		}
		else if (thisObj->args.size() > 1) {
			if (thisObj->args.at(1).empty()) return "";
			core::pseudoFS()->setFileAtt(thisObj->args.at(0), curDisk, "content", thisObj->args.at(1));
			core::pseudoFS()->getNRFS()->saveData();
		}
		return "";
	}
	#endif

	if (thisObj->returnable && thisObj->args.size() > 1) {
		if (thisObj->args.at(1).empty()) return core::gprint("Command error: The second argument of the command is empty!", core::PrintColors::red);
		std::string oldContent = core::pseudoFS()->getFileData(thisObj->args.at(0), curDisk, code).content;
		core::pseudoFS()->setFileAtt(thisObj->args.at(0), curDisk, "content", thisObj->args.at(1));
		core::pseudoFS()->getNRFS()->saveData();
		return core::gprint("Update file content!\nOld: " + oldContent + "\nNew: " + thisObj->args.at(1) + "\n", core::PrintColors::green);
	}

	return "";
}

std::string core::commands::CORE_COMMAND_showFileData(core::User* who, core::CommandObject* thisObj) {
	if (thisObj->args.at(0).empty()) {
		return core::gprint("Command error: Arguments is empty!\n", core::PrintColors::red);
	}
	std::string checkResult = core::checkUserPermissionsForCommand(who);
	COMMAND_ERROR_OUTPUT(checkResult)

	int code;
	size_t curDisk = core::pseudoFS()->getCurDiskId();
	std::string where = (thisObj->args.at(0)._Starts_with("./") ? thisObj->args.at(0) : core::pseudoFS()->getCurrentPath() + thisObj->args.at(0));
	const FileData targetFile = core::pseudoFS()->getFileData(where, curDisk, code);
	checkResult = core::checkFileCodeForPFSCommand(code);
	COMMAND_ERROR_OUTPUT(checkResult)

	std::string fileOwnerUsername = (targetFile.owner ? targetFile.owner->getUsername() : (targetFile.system ? "[ SYSTEM ]" : "[ NONE ]"));
		
	std::string result = core::gprint(core::PrintColors::aqua, "File name: " + targetFile.name + "\n",
		"File owner: " + fileOwnerUsername + "\n"
	);

	return result;
}

std::string core::commands::CORE_COMMAND_readFile(core::User* who, core::CommandObject* thisObj) {
	if (thisObj->args.at(0).empty()) {
		return core::gprint("Command error: Arguments is empty!\n", core::PrintColors::red);
	}
	std::string checkResult = core::checkUserPermissionsForCommand(who);
	COMMAND_ERROR_OUTPUT(checkResult)

	int code;
	size_t curDisk = core::pseudoFS()->getCurDiskId();
	std::string where = (thisObj->args.at(0)._Starts_with("./") ? thisObj->args.at(0) : core::pseudoFS()->getCurrentPath() + thisObj->args.at(0));
	const FileData targetFile = core::pseudoFS()->getFileData(where, curDisk, code);
	checkResult = core::checkFileCodeForPFSCommand(code);
	COMMAND_ERROR_OUTPUT(checkResult)

	std::string result = core::gprint(core::PrintColors::aqua, targetFile.content + "\n");
	return result;
}

std::string core::commands::CORE_COMMAND_writeOnNewLineFile(core::User* who, core::CommandObject* thisObj) {
	if (thisObj->args.at(0).empty() || thisObj->args[1].empty()) {
		return core::gprint("Command error: Arguments is empty!\n", core::PrintColors::red);
	}
	std::string checkResult = core::checkUserPermissionsForCommand(who);
	COMMAND_ERROR_OUTPUT(checkResult)

	size_t curDisk = core::pseudoFS()->getCurDiskId();
	int code;
	const FileData f = core::pseudoFS()->getFileData(thisObj->args.at(0), curDisk, code);
	std::string content = f.content + "\n" + thisObj->args.at(1);
	checkResult = core::checkFileCodeForPFSCommand(code);
	COMMAND_ERROR_OUTPUT(checkResult)

	checkResult = core::checkUserPermissionsForPFSCommand(who, f);
	COMMAND_ERROR_OUTPUT(checkResult)

	core::pseudoFS()->setFileAtt(thisObj->args.at(0), curDisk, "content", content);
	core::pseudoFS()->getNRFS()->saveData();
	return "";
}

std::string core::commands::CORE_COMMAND_writeFile(core::User* who, core::CommandObject* thisObj) {
	if (thisObj->args.at(0).empty() || thisObj->args[1].empty()) {
		return core::gprint("Command error: Arguments is empty!\n", core::PrintColors::red);
	}
	std::string checkResult = core::checkUserPermissionsForCommand(who);
	COMMAND_ERROR_OUTPUT(checkResult)

	int code;
	size_t curDisk = core::pseudoFS()->getCurDiskId();
	const FileData f = core::pseudoFS()->getFileData(thisObj->args.at(0), curDisk, code);
	std::string content = f.content + thisObj->args.at(1);
	checkResult = core::checkFileCodeForPFSCommand(code);
	COMMAND_ERROR_OUTPUT(checkResult)

	checkResult = core::checkUserPermissionsForPFSCommand(who, f);
	COMMAND_ERROR_OUTPUT(checkResult)

	core::pseudoFS()->setFileAtt(thisObj->args.at(0), curDisk, "content", content);
	core::pseudoFS()->getNRFS()->saveData();
	return "";
}

std::string core::commands::CORE_COMMAND_rewriteFile(core::User* who, core::CommandObject* thisObj) {
	if (thisObj->args.at(0).empty() || thisObj->args[1].empty()) {
		return core::gprint("Command error: Arguments is empty!\n", core::PrintColors::red);
	}
	std::string checkResult = core::checkUserPermissionsForCommand(who);
	COMMAND_ERROR_OUTPUT(checkResult)

	int code;
	size_t curDisk = core::pseudoFS()->getCurDiskId();
	const FileData f = core::pseudoFS()->getFileData(thisObj->args.at(0), curDisk, code);
	checkResult = core::checkFileCodeForPFSCommand(code);
	COMMAND_ERROR_OUTPUT(checkResult)

	checkResult = core::checkUserPermissionsForPFSCommand(who, f);
	COMMAND_ERROR_OUTPUT(checkResult)

	core::pseudoFS()->setFileAtt(thisObj->args.at(0), curDisk, "content", thisObj->args.at(1));
	core::pseudoFS()->getNRFS()->saveData();
	return "";
}

std::string core::commands::CORE_COMMAND_clearFile(core::User* who, core::CommandObject* thisObj) {
	if (thisObj->args.at(0).empty()) {
		return core::gprint("Command error: Arguments is empty!\n", core::PrintColors::red);
	}
	std::string checkResult = core::checkUserPermissionsForCommand(who);
	COMMAND_ERROR_OUTPUT(checkResult)

	int code;
	size_t curDisk = core::pseudoFS()->getCurDiskId();
	const FileData f = core::pseudoFS()->getFileData(thisObj->args.at(0), curDisk, code);
	checkResult = core::checkFileCodeForPFSCommand(code);
	COMMAND_ERROR_OUTPUT(checkResult)

	checkResult = core::checkUserPermissionsForPFSCommand(who, f);
	COMMAND_ERROR_OUTPUT(checkResult)

	core::pseudoFS()->setFileAtt(thisObj->args.at(0), curDisk, "content", "");
	core::pseudoFS()->getNRFS()->saveData();
	return "";
}

std::string core::commands::CORE_COMMAND_setNewFileOwner(core::User* who, core::CommandObject* thisObj) {
	if (thisObj->args.at(0).empty() || thisObj->args[1].empty()) {
		return core::gprint("Command error: Arguments is empty!\n", core::PrintColors::red);
	}
	std::string checkResult = core::checkUserPermissionsForCommand(who);
	COMMAND_ERROR_OUTPUT(checkResult)

	int code;
	size_t curDisk = core::pseudoFS()->getCurDiskId();
	const FileData f = core::pseudoFS()->getFileData(thisObj->args.at(0), curDisk, code);
	checkResult = core::checkFileCodeForPFSCommand(code);
	COMMAND_ERROR_OUTPUT(checkResult)

	checkResult = core::checkUserPermissionsForPFSCommand(who, f);
	COMMAND_ERROR_OUTPUT(checkResult)

	core::pseudoFS()->setFileAtt(thisObj->args.at(0), curDisk, "owner", (thisObj->args.at(1) != "[ NONE ]" ? &core::userManager()->getUser(thisObj->args.at(1)) : nullptr));
	core::pseudoFS()->getNRFS()->saveData();
	return "";
}

std::string core::commands::CORE_COMMAND_createFolder(core::User* who, core::CommandObject* thisObj) {
	if (thisObj->args.at(0).empty()) {
		return core::gprint("Command error: Arguments is empty!\n", core::PrintColors::red);
	}
	std::string checkResult = core::checkUserPermissionsForCommand(who);
	COMMAND_ERROR_OUTPUT(checkResult)

	int code;
	size_t curDisk = core::pseudoFS()->getCurDiskId();
	std::string where = (thisObj->args.at(0)._Starts_with("./") ? thisObj->args.at(0) : core::pseudoFS()->getCurrentPath() + thisObj->args.at(0));
	code = core::pseudoFS()->createFolder(where, curDisk);
	if (code == core::PseudoFSCodes::ALREADY_EXISTS) {
		FolderData folder{
			"",
			std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()),
			std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()),
			{},
			{},
			false,
			false,
			nullptr
		};
		fixCreateSamePFSObject(where, curDisk, folder, code);
	}

	if (thisObj->args.size() > 1 && thisObj->args.at(1).find("o") != std::string::npos)
		core::pseudoFS()->setFileAtt(where, curDisk, "owner", who);
	core::pseudoFS()->getNRFS()->saveData();
	return "";
}

std::string core::commands::CORE_COMMAND_createLinkFolder(core::User* who, core::CommandObject* thisObj) {
	if (thisObj->args.at(0).empty()) {
		return core::gprint("Command error: Arguments is empty!\n", core::PrintColors::red);
	}
	std::string checkResult = core::checkUserPermissionsForCommand(who);
	COMMAND_ERROR_OUTPUT(checkResult)

	std::string where = (thisObj->args.at(0)._Starts_with("./") ? thisObj->args.at(0) : core::pseudoFS()->getCurrentPath() + thisObj->args.at(0));
	std::string where2 = (thisObj->args.at(1)._Starts_with("./") ? thisObj->args.at(1) : core::pseudoFS()->getCurrentPath() + thisObj->args.at(1));
	int code;
	size_t curDisk = core::pseudoFS()->getCurDiskId();
	FolderData folder = core::pseudoFS()->getFolderData(where, curDisk, code);
	checkResult = core::checkFileCodeForPFSCommand(code);
	COMMAND_ERROR_OUTPUT(checkResult)

	FolderData* linkFolder = new FolderData{
		core::Utils::split(where2, '/').back(),
		0,
		0,
		{},
		{},
		false,
		folder.hidden,
		&folder,
		where
	};

	code = core::pseudoFS()->createFolder(where2, curDisk, linkFolder);
	if (code == core::PseudoFSCodes::ALREADY_EXISTS)
		fixCreateSamePFSObject(where, curDisk, *linkFolder, code);

	if (thisObj->args.size() > 1 && thisObj->args.at(1).find("o") != std::string::npos)
		core::pseudoFS()->setFileAtt(where, curDisk, "owner", who);

	core::pseudoFS()->getNRFS()->saveData();
	return "";
}

std::string core::commands::CORE_COMMAND_deleteFolder(core::User* who, core::CommandObject* thisObj) {
	if (thisObj->args.at(0).empty()) {
		return core::gprint("Command error: Arguments is empty!\n", core::PrintColors::red);
	}
	std::string checkResult = core::checkUserPermissionsForCommand(who);
	COMMAND_ERROR_OUTPUT(checkResult)

	std::string where = (thisObj->args.at(0)._Starts_with("./") ? thisObj->args.at(0) : core::pseudoFS()->getCurrentPath() + thisObj->args.at(0));
	size_t curDisk = core::pseudoFS()->getCurDiskId();
	int code;
	const FolderData f = core::pseudoFS()->getFolderData(where, curDisk, code);
	checkResult = core::checkFileCodeForPFSCommand(code);
	COMMAND_ERROR_OUTPUT(checkResult)

	checkResult = core::checkUserPermissionsForPFSCommand(who, f);
	COMMAND_ERROR_OUTPUT(checkResult)

	core::pseudoFS()->deleteFolder(where, curDisk);
	core::pseudoFS()->getNRFS()->saveData();
	return "";
}

std::string core::commands::CORE_COMMAND_renameFolder(core::User* who, core::CommandObject* thisObj) {
	if (thisObj->args.at(0).empty() || thisObj->args[1].empty()) {
		return core::gprint("Command error: Arguments is empty!\n", core::PrintColors::red);
	}
	std::string checkResult = core::checkUserPermissionsForCommand(who);
	COMMAND_ERROR_OUTPUT(checkResult)

	std::string where = (thisObj->args.at(0)._Starts_with("./") ? thisObj->args.at(0) : core::pseudoFS()->getCurrentPath() + thisObj->args.at(0));
	size_t curDisk = core::pseudoFS()->getCurDiskId();
	int code;
	FolderData f = core::pseudoFS()->getFolderData(where, curDisk, code);
	if (code == core::PseudoFSCodes::ALREADY_EXISTS) {
		f.timeEdit = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		fixCreateSamePFSObject(where, curDisk, f, code);
	}
	
	checkResult = core::checkFileCodeForPFSCommand(code);
	COMMAND_ERROR_OUTPUT(checkResult)

	checkResult = core::checkUserPermissionsForPFSCommand(who, f);
	COMMAND_ERROR_OUTPUT(checkResult)

	core::pseudoFS()->renameFolder(where, curDisk, thisObj->args.at(1));
	core::pseudoFS()->getNRFS()->saveData();
	return "";
}

std::string core::commands::CORE_COMMAND_moveFolder(core::User* who, core::CommandObject* thisObj) {
	 if (thisObj->args.at(0).empty() || thisObj->args[1].empty()) {
		return core::gprint("Command error: Arguments is empty!\n", core::PrintColors::red);
	}
	std::string checkResult = core::checkUserPermissionsForCommand(who);
	COMMAND_ERROR_OUTPUT(checkResult)

	size_t curDisk = core::pseudoFS()->getCurDiskId();
	int code;
	FolderData f = core::pseudoFS()->getFolderData(thisObj->args.at(0), curDisk, code);
	if (code == core::PseudoFSCodes::ALREADY_EXISTS) {
		fixCreateSamePFSObject(thisObj->args.at(0), curDisk, f, code);
	}

	checkResult = core::checkFileCodeForPFSCommand(code);
	COMMAND_ERROR_OUTPUT(checkResult)

	checkResult = core::checkUserPermissionsForPFSCommand(who, f);
	COMMAND_ERROR_OUTPUT(checkResult)

	code = core::pseudoFS()->moveFolder(thisObj->args.at(0), curDisk, thisObj->args.at(1), curDisk);
	if (code == core::PseudoFSCodes::ALREADY_EXISTS)
		fixCreateSamePFSObject(thisObj->args.at(0), curDisk, f, code);

	core::pseudoFS()->getNRFS()->saveData();
	return "";
}

std::string core::commands::CORE_COMMAND_showFolderData(core::User* who, core::CommandObject* thisObj) {
	 if (thisObj->args.at(0).empty() || thisObj->args[1].empty()) {
		return core::gprint("Command error: Arguments is empty!\n", core::PrintColors::red);
	}
	std::string checkResult = core::checkUserPermissionsForCommand(who);
	COMMAND_ERROR_OUTPUT(checkResult)

	size_t curDisk = core::pseudoFS()->getCurDiskId();
	int code;
	std::string where = (thisObj->args.at(0)._Starts_with("./") ? thisObj->args.at(0) : core::pseudoFS()->getCurrentPath() + thisObj->args.at(0));
	const FolderData targetFolder = core::pseudoFS()->getFolderData(where, curDisk, code);
	checkResult = core::checkFileCodeForPFSCommand(code);
	COMMAND_ERROR_OUTPUT(checkResult)

	std::string folderOwnerUsername = (targetFolder.owner ? targetFolder.owner->getUsername() : (targetFolder.system ? "[ SYSTEM ]" : "[ NONE ]"));
		
	std::string result = core::gprint(core::PrintColors::aqua, "Folder name: " + targetFolder.name + "\n",
		"Folder owner: " + folderOwnerUsername + "\n"
	);

	return result;
}

std::string core::commands::CORE_COMMAND_setNewFolderOwner(core::User* who, core::CommandObject* thisObj) {
	 if (thisObj->args.at(0).empty() || thisObj->args[1].empty()) {
		return core::gprint("Command error: Arguments is empty!\n", core::PrintColors::red);
	}
	std::string checkResult = core::checkUserPermissionsForCommand(who);
	COMMAND_ERROR_OUTPUT(checkResult)

	size_t curDisk = core::pseudoFS()->getCurDiskId();
	int code;
	std::string where = (thisObj->args.at(0)._Starts_with("./") ? thisObj->args.at(0) : core::pseudoFS()->getCurrentPath() + thisObj->args.at(0));
	const FolderData targetFolder = core::pseudoFS()->getFolderData(where, curDisk, code);
	checkResult = core::checkFileCodeForPFSCommand(code);
	COMMAND_ERROR_OUTPUT(checkResult)

	checkResult = core::checkUserPermissionsForPFSCommand(who, targetFolder);
	COMMAND_ERROR_OUTPUT(checkResult)

	core::pseudoFS()->setFolderAtt(thisObj->args.at(0), curDisk, "owner", (thisObj->args.at(1) != "[ NONE ]" ? &core::userManager()->getUser(thisObj->args.at(1)) : nullptr));
	core::pseudoFS()->getNRFS()->saveData();
	return "";
}

std::string core::commands::CORE_COMMAND_dir(core::User*, core::CommandObject* thisObj) {
	std::string output;

	bool showHidden = (thisObj->args.empty() ? false : thisObj->args.at(0) == "h");
	int fCount = 0;
	int code;
	size_t curDisk = core::pseudoFS()->getCurDiskId();
	const std::vector<std::shared_ptr<FolderData>> folders = (core::pseudoFS()->getCurrentPath() == "./" ? core::pseudoFS()->getNRFS()->getRoot()->getFolders() : core::pseudoFS()->getFolderData(core::pseudoFS()->getCurrentPath(), curDisk, code).folders);
	const std::vector<std::shared_ptr<FileData>> files = (core::pseudoFS()->getCurrentPath() == "./" ? core::pseudoFS()->getNRFS()->getRoot()->getFiles() : core::pseudoFS()->getFolderData(core::pseudoFS()->getCurrentPath(), curDisk, code).files);

	for (const auto& folder : folders) {
		if (folder.get() == nullptr) continue;
		bool a = (fCount >= 5 && folders.size() > 4);

		if (folder->hidden && showHidden) {
			output += core::gprint(folder->name + "    " + (a ? "\n" : ""), core::PrintColors::light_aqua);
			if (a) fCount = 0;
			else ++fCount;
		}
		else if (!folder->hidden) {
			output += folder->name + "    " + (fCount >= 5 && folders.size() > 4 ? "\n" : "");
			if (fCount >= 5) fCount = 0;
			else ++fCount;
		}
	}
	for (const auto& file : files) {
		if (file.get() == nullptr) continue;
		bool a = (fCount >= 5 && files.size() > 4);

		if (file->hidden && showHidden) {
			output += core::gprint(file->name + "    " + (a ? "\n" : ""), core::PrintColors::light_aqua);
			if (a) fCount = 0;
			else ++fCount;
		}
		else if (!file->hidden) {
			output += file->name + "    " + (a ? "\n" : "");
			if (a) fCount = 0;
			else ++fCount;
		}
	}
	return output + "\n";
}

std::string core::commands::CORE_COMMAND_tree(core::User*, core::CommandObject* thisObj) {
	size_t curDisk = core::pseudoFS()->getCurDiskId();
	std::string str;
	if (thisObj->args.empty()) {
		core::pseudoFS()->showTree(false, curDisk, false, "./", str);
	}
	else {
		core::pseudoFS()->showTree(
			(thisObj->args.size() > 1 ? (thisObj->args.at(1).find("h") != std::string::npos) : false),
			curDisk,
			(thisObj->args.size() > 1 ? (thisObj->args.at(1).find("f") != std::string::npos) : false),
			thisObj->args.at(0),
			str
		);
	}
	return str;
}

std::string core::commands::CORE_COMMAND_showAll(core::User*, core::CommandObject* thisObj) {
	size_t curDisk = core::pseudoFS()->getCurDiskId();
	std::string str;
	if (thisObj->args.empty()) {
		core::pseudoFS()->printAll(false, curDisk, "./", str);
	}
	else {
		core::pseudoFS()->printAll(
			(thisObj->args.size() > 1 ? thisObj->args.at(1).find("h") != std::string::npos : false),
			curDisk,
			thisObj->args.at(0),
			str
		);
	}
	return str;
}

std::string core::commands::CORE_COMMAND_searchFile(core::User*, core::CommandObject* thisObj) {
	if (thisObj->args.empty()) {
		return core::gprint("Error: thisObj->args is empty!", core::PrintColors::red);
	}

	std::string result = "";
	std::string path = (thisObj->args.size() > 1 ? thisObj->args.at(1) : core::pseudoFS()->getCurrentPath());
	bool fullSearch = thisObj->args.size() < 2;
	std::vector<std::string> parsedPath = core::Utils::split(path, '/');
	for (std::weak_ptr<FileData> file : core::pseudoFS()->getNRFS()->getRoot()->getFiles()) {
		if (file.lock()->name.find(thisObj->args.at(0)) < LONG_LONG_MAX) {
			result += "./" + file.lock()->name + "\n";
		}
	}

	for (std::weak_ptr<FolderData> folder : core::pseudoFS()->getNRFS()->getRoot()->getFolders()) {
		if (fullSearch) {
			result += core::commands::CORE_COMMAND_searchFileHelper(*folder.lock().get(), thisObj->args.at(0), {}, "./" + folder.lock()->name);
		}
		else {
			parsedPath.erase(parsedPath.begin());
			for (std::weak_ptr<FolderData> anotherFolder : core::pseudoFS()->getNRFS()->getRoot()->getFolders()) {
				if (parsedPath.at(0) == anotherFolder.lock()->name) {
					result += core::commands::CORE_COMMAND_searchFileHelper(*anotherFolder.lock().get(), thisObj->args.at(0), parsedPath, "./" + anotherFolder.lock()->name);
				}
			}
		}
	}
	return result;
}

std::string core::commands::CORE_COMMAND_searchFileHelper(const core::FolderData& curFolder, const std::string& what, std::vector<std::string> path, std::string stringPath) {
	bool fullSearch = path.empty();
	if (curFolder.files.empty() && curFolder.folders.empty()) {
		return core::gprint("Error: ", core::PrintColors::red);
	}
	
	std::string output = "";
	for (std::weak_ptr<core::FileData> file : curFolder.files) {
		if (file.lock()->name.find(what) < LONG_LONG_MAX) {
			output += stringPath + "/" + file.lock()->name + "\n";
		}
	}

	for (std::weak_ptr<core::FolderData> folder : curFolder.folders) {
		if (fullSearch) {
			output += core::commands::CORE_COMMAND_searchFileHelper(*folder.lock().get(), what, {}, stringPath + "/" + folder.lock()->name);
		}
		else {
			path.erase(path.begin());
			for (std::weak_ptr<FolderData> anotherFolder : core::pseudoFS()->getNRFS()->getRoot()->getFolders()) {
				if (path.at(0) == folder.lock()->name) {
					output += core::commands::CORE_COMMAND_searchFileHelper(*anotherFolder.lock().get(), what, path, stringPath + "./" + anotherFolder.lock()->name);
				}
			}
		}
	}
	return output;
}

std::string core::commands::CORE_COMMAND_printDiskSize(core::User*, core::CommandObject*) {
	size_t curDisk = core::pseudoFS()->getCurDiskId();
	int filesSize = core::pseudoFS()->getNRFS()->getDisks().at(curDisk)->getFilesSize();
	int foldersSize = core::pseudoFS()->getNRFS()->getDisks().at(curDisk)->getFoldersSize();
	int diskSize = core::pseudoFS()->getNRFS()->getDisks().at(curDisk)->getDiskSize();

	std::string result = core::gprint(DEFAULT_PRINT_COLOR,
		"Files: ", filesSize, "\n",
		"Folders: ", foldersSize, "\n",
		"All (files & folders): ", diskSize, "\n"
	);

	return result;
}

std::string core::commands::CORE_COMMAND_whereIm(core::User*, core::CommandObject*) {
	std::vector<std::string> parsedPath = core::Utils::split(core::pseudoFS()->getCurrentPath(), '/');

	std::string result = gprint(core::PrintColors::aqua, "You're in the folder: " + parsedPath.back() + "\n",
		"Full path: " + core::pseudoFS()->getCurrentPath() + "\n",
		"Disk: ", core::pseudoFS()->getNRFS()->getDisks().at(core::pseudoFS()->getCurDiskId())->getLetter(), "\n"
	);
	
	return result;
}