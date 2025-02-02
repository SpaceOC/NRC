#ifndef NRC_BASE_COMMAND_COMMANDS_H_
#define NRC_BASE_COMMAND_COMMANDS_H_

#include "Core/settings.h"

// if str is not empty - the print function is called with parameters: str + "\n", core::PrintColors::red. And also return the function the value of str
#define COMMAND_ERROR_OUTPUT(str, returnable) if (!str.empty()) {       \
    if (!returnable)                                                    \
        core::print(str + "\n", core::PrintColors::red);                \
    return str;                                                         \
}

namespace core {
    class User;
    struct FolderData;
    struct FileData;

    namespace commands {
        std::string CORE_COMMAND_help(core::User* who, const std::vector<std::string>& args, bool returnable);
        std::string CORE_COMMAND_info(core::User* who, bool returnable);
        std::string CORE_COMMAND_time(core::User* who, const std::vector<std::string>& args, bool returnable);
        //void CORE_COMMAND_exit(core::User* who, bool returnable);

        //   -------------- Pseudo FS Commands ---------------

        std::string CORE_COMMAND_cd(core::User* who, const std::vector<std::string>& args, bool returnable);

        std::string CORE_COMMAND_createFile(core::User* who, const std::vector<std::string>& args, bool returnable);
        std::string CORE_COMMAND_createLinkFile(core::User* who, const std::vector<std::string>& args, bool returnable);
        std::string CORE_COMMAND_deleteFile(core::User* who, const std::vector<std::string>& args, bool returnable);
        std::string CORE_COMMAND_renameFile(core::User* who, const std::vector<std::string>& args, bool returnable);
        std::string CORE_COMMAND_moveFile(core::User* who, const std::vector<std::string>& args, bool returnable);
        std::string CORE_COMMAND_editFile(core::User* who, const std::vector<std::string>& args, bool returnable);
        std::string CORE_COMMAND_writeOnNewLineFile(core::User* who, const std::vector<std::string>& args, bool returnable);
        std::string CORE_COMMAND_writeFile(core::User* who, const std::vector<std::string>& args, bool returnable);
        std::string CORE_COMMAND_rewriteFile(core::User* who, const std::vector<std::string>& args, bool returnable);
        std::string CORE_COMMAND_clearFile(core::User* who, const std::vector<std::string>& args, bool returnable);
        std::string CORE_COMMAND_setNewFileOwner(core::User* who, const std::vector<std::string>& args, bool returnable);

        std::string CORE_COMMAND_createFolder(core::User* who, const std::vector<std::string>& args, bool returnable);
        std::string CORE_COMMAND_createLinkFolder(core::User* who, const std::vector<std::string>& args, bool returnable);
        std::string CORE_COMMAND_deleteFolder(core::User* who, const std::vector<std::string>& args, bool returnable);
        std::string CORE_COMMAND_renameFolder(core::User* who, const std::vector<std::string>& args, bool returnable);
        std::string CORE_COMMAND_moveFolder(core::User* who, const std::vector<std::string>& args, bool returnable);
        std::string CORE_COMMAND_clearFolder(core::User* who, const std::vector<std::string>& args, bool returnable);
        std::string CORE_COMMAND_setNewFolderOwner(core::User* who, const std::vector<std::string>& args, bool returnable);

        std::string CORE_COMMAND_whereIm(core::User* who, bool returnable);

        std::string CORE_COMMAND_showFileData(core::User* who, const std::vector<std::string>& args, bool returnable);
        std::string CORE_COMMAND_showFolderData(core::User* who, const std::vector<std::string>& args, bool returnable);

        std::string CORE_COMMAND_dir(core::User* who, const std::vector<std::string>& args, bool returnable);
        std::string CORE_COMMAND_tree(core::User* who, const std::vector<std::string>& args, bool returnable);
        std::string CORE_COMMAND_showAll(core::User* who, const std::vector<std::string>& args, bool returnable);

        std::string CORE_COMMAND_searchFile(core::User* who, const std::vector<std::string>& args, bool returnable);
        std::string CORE_COMMAND_searchFileHelper(const core::FolderData& curFolder, const std::string& what, std::vector<std::string> path, std::string stringPath, bool returnable = false);

        std::string CORE_COMMAND_printDiskSize(core::User* who, bool returnable);

        //   -------------- Users "Manager" ---------------

        std::string CORE_COMMAND_setPassword(core::User* who, const std::vector<std::string>& args, bool returnable); 
        std::string CORE_COMMAND_editDisplayName(core::User* who, const std::vector<std::string>& args, bool returnable);
        std::string CORE_COMMAND_createUser(core::User* who, const std::vector<std::string>& args, bool returnable);
        std::string CORE_COMMAND_deleteUser(core::User* who, const std::vector<std::string>& args, bool returnable);
        std::string CORE_COMMAND_renameUser(core::User* who, const std::vector<std::string>& args, bool returnable);
        std::string CORE_COMMAND_setPermissionsUser(core::User* who, const std::vector<std::string>& args, bool returnable);

        std::string CORE_COMMAND_addLocalVar(core::User* who, const std::vector<std::string>& args, bool returnable);
        std::string CORE_COMMAND_renameLocalVar(core::User* who, const std::vector<std::string>& args, bool returnable);

        //   -------------- Users ---------------

        std::string CORE_COMMAND_whoim(core::User* who, bool returnable);
        std::string CORE_COMMAND_infoUser(core::User* who, const std::vector<std::string>& args, bool returnable);
        std::string CORE_COMMAND_allInfoUsers(core::User* who, bool returnable);
        std::string CORE_COMMAND_logout(core::User* who, bool returnable);

        //   ---------------- Other ---------------

        //std::string CORE_COMMAND_addSystemVar(core::User* who, const std::vector<std::string>& args, bool returnable);
        std::string CORE_COMMAND_allSystemVars(core::User* who, bool returnable);
        std::string CORE_COMMAND_allLocalVars(core::User* who, bool returnable);
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