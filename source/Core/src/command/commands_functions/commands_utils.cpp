#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <ctime>
#include <chrono>
#include "Core/command/commands.h"
#include "Core/users/user_permissions_enum.h"
#include "Core/users/user.h"
#include "Core/filesystem/pseudo_fs.h"
#include "Core/filesystem/nrfs.h"

std::string core::checkUserPermissionsForCommand(core::User* who) {
	if (!who)
		return "User is not initialised! (who == NULL)";
	else if (who->getPermissions() == core::UserPermissions::GHOST)
		return "Insufficient permissions for this operation: the current user permission level is lower than normal";
	return "";
}

std::string core::checkUserPermissionsForPFSCommand(core::User* who, const core::FileData& target) {
	if (target.link)
		return core::checkUserPermissionsForPFSCommand(who, *(target.link));
	else if (target.system && !target.owner && who->getPermissions() < core::UserPermissions::ADMIN)
		return "Insufficient rights to operate on a system file: administrator rights required";
	else if (target.owner && target.owner != who && who->getPermissions() < core::UserPermissions::ADMIN)
		return "Insufficient rights to operate on this file: administrator rights required";
	return "";
}

std::string core::checkUserPermissionsForPFSCommand(core::User* who, const core::FolderData& target) {
	if (target.link)
		return core::checkUserPermissionsForPFSCommand(who, *(target.link));
	else if (target.system && !target.owner && who->getPermissions() < core::UserPermissions::ADMIN)
		return "Insufficient rights to work with the system folder: administrator rights required";
	else if (target.owner && target.owner != who && who->getPermissions() < core::UserPermissions::ADMIN)
		return "Insufficient rights to operate on this folder: administrator rights required";
	return "";
}

std::string core::checkFileCodeForPFSCommand(int code) {
	switch (code)
	{
	case core::PseudoFSCodes::NOT_FOUND:            return "File not found!";
	case core::PseudoFSCodes::ALREADY_EXISTS:       return "This file already exists!";
	case core::PseudoFSCodes::PATH_IS_EMPTY:        return "The file operation was not completed - PATH IS EMPTY!";
	case core::PseudoFSCodes::BAD_PATH:             return "Incorrect folder path. Check if the path is correct.";
	case core::PseudoFSCodes::OK:                   return "";
	default:                                        return "Unknown Error";
	}
	return "";
}

std::string core::checkFolderCodeForPFSCommand(int code) {
	switch (code)
	{
	case core::PseudoFSCodes::NOT_FOUND:            return "Folder not found!";
	case core::PseudoFSCodes::ALREADY_EXISTS:       return "This folder already exists!";
	case core::PseudoFSCodes::PATH_IS_EMPTY:        return "The operation with the folder was not completed - PATH IS EMPTY!";
	case core::PseudoFSCodes::BAD_PATH:             return "Incorrect folder path. Check if the path is correct.";
	case core::PseudoFSCodes::OK:                   return "";
	default:                                        return "Unknown Error";
	}
	return "";
}

void core::fixCreateSamePFSObject(const std::string& path, size_t curDisk, const core::FileData& target, int& code) {
	int temp = 2;
	std::string fileName = core::pseudoFS()->getFileFromPath(path);
	std::string partPath = path.substr(0, path.length() - fileName.length());
	while (code == core::PseudoFSCodes::ALREADY_EXISTS) {
		std::string newFileName =
			fileName.substr(
				0,
				fileName.find_last_of(".")
			) + "(" + std::to_string(temp++) + ")" +
			fileName.substr(
				fileName.find_last_of("."),
			   fileName.length()
			);
		core::FileData* targetPtr = new core::FileData(target);
		targetPtr->name = newFileName;
		code = core::pseudoFS()->createFile(path + newFileName, curDisk, targetPtr);
	}
}

void core::fixCreateSamePFSObject(const std::string& path, size_t curDisk, const core::FolderData& target, int& code) {
	int temp = 2;
	std::string folderName = core::pseudoFS()->getFolderFromPath(path);
	std::string partPath = path.substr(0, path.length() - folderName.length());
	while (code == core::PseudoFSCodes::ALREADY_EXISTS) {
		std::string newFolderName =
			folderName.substr(
				0,
				folderName.find_last_of(".")
			) + "(" + std::to_string(temp++) + ")" +
			folderName.substr(
				folderName.find_last_of("."),
			   folderName.length()
			);
		core::FolderData* targetPtr = new core::FolderData(target);
		targetPtr->name = newFolderName;
		code = core::pseudoFS()->createFolder(path + newFolderName, curDisk, targetPtr);
	}
}