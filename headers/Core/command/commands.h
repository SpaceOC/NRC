#ifndef NRC_BASE_COMMAND_COMMANDS_H_
#define NRC_BASE_COMMAND_COMMANDS_H_

#include "Core/settings.h"
#include <string>
#include <vector>

// if str is not empty - the gprint function is called with parameters: str + "\n", core::PrintColors::red. And also return the function the value of str
#define COMMAND_ERROR_OUTPUT(str) if (!str.empty()) {       \
	str = core::gprint(str + "\n", core::PrintColors::red);             \
	return str;                                                         \
}

namespace core {
	class User;
	struct FolderData;
	struct FileData;
	struct CommandObject;

	namespace commands {
		std::string CORE_COMMAND_help(core::User*, core::CommandObject*);
		std::string CORE_COMMAND_info(core::User*, core::CommandObject*);
		std::string CORE_COMMAND_time(core::User*, core::CommandObject*);
		//void CORE_COMMAND_exit(core::User* who, bool returnable);

		//   -------------- Pseudo FS Commands ---------------

		std::string CORE_COMMAND_cd(core::User*, core::CommandObject*);

		std::string CORE_COMMAND_createFile(core::User*, core::CommandObject*);
		std::string CORE_COMMAND_createLinkFile(core::User*, core::CommandObject*);
		std::string CORE_COMMAND_deleteFile(core::User*, core::CommandObject*);
		std::string CORE_COMMAND_renameFile(core::User*, core::CommandObject*);
		std::string CORE_COMMAND_moveFile(core::User*, core::CommandObject*);
		std::string CORE_COMMAND_editFile(core::User*, core::CommandObject*);
		std::string CORE_COMMAND_writeOnNewLineFile(core::User*, core::CommandObject*);
		std::string CORE_COMMAND_writeFile(core::User*, core::CommandObject*);
		std::string CORE_COMMAND_rewriteFile(core::User*, core::CommandObject*);
		std::string CORE_COMMAND_clearFile(core::User*, core::CommandObject*);
		std::string CORE_COMMAND_setNewFileOwner(core::User*, core::CommandObject*);

		std::string CORE_COMMAND_createFolder(core::User*, core::CommandObject*);
		std::string CORE_COMMAND_createLinkFolder(core::User*, core::CommandObject*);
		std::string CORE_COMMAND_deleteFolder(core::User*, core::CommandObject*);
		std::string CORE_COMMAND_renameFolder(core::User*, core::CommandObject*);
		std::string CORE_COMMAND_moveFolder(core::User*, core::CommandObject*);
		std::string CORE_COMMAND_clearFolder(core::User*, core::CommandObject*);
		std::string CORE_COMMAND_setNewFolderOwner(core::User*, core::CommandObject*);

		std::string CORE_COMMAND_whereIm(core::User*, core::CommandObject*);

		std::string CORE_COMMAND_showFileData(core::User*, core::CommandObject*);
		std::string CORE_COMMAND_readFile(core::User*, core::CommandObject*);
		std::string CORE_COMMAND_showFolderData(core::User*, core::CommandObject*);

		std::string CORE_COMMAND_dir(core::User*, core::CommandObject*);
		std::string CORE_COMMAND_tree(core::User*, core::CommandObject*);
		std::string CORE_COMMAND_showAll(core::User*, core::CommandObject*);

		std::string CORE_COMMAND_searchFile(core::User*, core::CommandObject*);
		std::string CORE_COMMAND_searchFileHelper(const core::FolderData& curFolder, const std::string& what, std::vector<std::string> path, std::string stringPath);

		std::string CORE_COMMAND_printDiskSize(core::User*, core::CommandObject*);

		//   -------------- Users "Manager" ---------------

		std::string CORE_COMMAND_setPassword(core::User*, core::CommandObject*); 
		std::string CORE_COMMAND_editDisplayName(core::User*, core::CommandObject*);
		std::string CORE_COMMAND_createUser(core::User*, core::CommandObject*);
		std::string CORE_COMMAND_deleteUser(core::User*, core::CommandObject*);
		std::string CORE_COMMAND_renameUser(core::User*, core::CommandObject*);
		std::string CORE_COMMAND_setPermissionsUser(core::User*, core::CommandObject*);

		std::string CORE_COMMAND_addLocalVar(core::User*, core::CommandObject*);
		std::string CORE_COMMAND_renameLocalVar(core::User*, core::CommandObject*);

		//   -------------- Users ---------------

		std::string CORE_COMMAND_whoim(core::User*, core::CommandObject*);
		std::string CORE_COMMAND_infoUser(core::User*, core::CommandObject*);
		std::string CORE_COMMAND_allInfoUsers(core::User*, core::CommandObject*);
		std::string CORE_COMMAND_logout(core::User*, core::CommandObject*);

		//   ---------------- Other ---------------

		//std::string CORE_COMMAND_addSystemVar(core::User*, core::CommandObject*);
		std::string CORE_COMMAND_allSystemVars(core::User*, core::CommandObject*);
		std::string CORE_COMMAND_allLocalVars(core::User*, core::CommandObject*);
	}

	std::string checkUserPermissionsForCommand(core::User* who);
	std::string checkUserPermissionsForPFSCommand(core::User* who, const core::FileData& target);
	std::string checkUserPermissionsForPFSCommand(core::User* who, const core::FolderData& target);
	std::string checkFileCodeForPFSCommand(int code);
	std::string checkFolderCodeForPFSCommand(int code);
	void fixCreateSamePFSObject(const std::string& path, size_t curDisk, const core::FileData& target, int& code);
	void fixCreateSamePFSObject(const std::string& path, size_t curDisk, const core::FolderData& target, int& code);
}

#endif