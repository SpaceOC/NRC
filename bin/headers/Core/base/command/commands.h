/*
    Copyright (C) 2024-2024  SpaceOC

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#ifndef NRC_BASE_COMMAND_COMMANDS_H_
#define NRC_BASE_COMMAND_COMMANDS_H_

#include "Core/base/filesystem/nrfs.h"

namespace core {
    namespace commands {
        void CORE_COMMAND_help(const std::vector<std::string>& args);
        void CORE_COMMAND_info();
        void CORE_COMMAND_time(const std::vector<std::string>& args);
        //void CORE_COMMAND_exit();

        //   -------------- Pseudo FS Commands ---------------

        void CORE_COMMAND_cd(const std::vector<std::string>& args);

        void CORE_COMMAND_createFile(const std::vector<std::string>& args);
        void CORE_COMMAND_deleteFile(const std::vector<std::string>& args);
        void CORE_COMMAND_renameFile(const std::vector<std::string>& args);
        void CORE_COMMAND_moveFile(const std::vector<std::string>& args);

        void CORE_COMMAND_createFolder(const std::vector<std::string>& args);
        void CORE_COMMAND_deleteFolder(const std::vector<std::string>& args);
        void CORE_COMMAND_renameFolder(const std::vector<std::string>& args);
        void CORE_COMMAND_moveFolder(const std::vector<std::string>& args);

        void CORE_COMMAND_NRFSSize();

        void CORE_COMMAND_whereIm();

        void CORE_COMMAND_showFileData(const std::vector<std::string>& args);
        void CORE_COMMAND_showFolderData(const std::vector<std::string>& args);

        void CORE_COMMAND_dir(const std::vector<std::string>& args);
        void CORE_COMMAND_tree(const std::vector<std::string>& args);
        void CORE_COMMAND_showAll(const std::vector<std::string>& args);

        void CORE_COMMAND_searchFile(const std::vector<std::string>& args);
        void CORE_COMMAND_searchFileHelper(const core::FolderData& curFolder, const std::string& what, std::vector<std::string> path, std::string stringPath);

        //   -------------- Users "Manager" ---------------

        void CORE_COMMAND_setPasswordWithDialogue();
        void CORE_COMMAND_setPassword(const std::vector<std::string>& args); 

        void CORE_COMMAND_editDisplayNameWithDialogue();
        void CORE_COMMAND_editDisplayName(const std::vector<std::string>& args);

        void CORE_COMMAND_addUserWithDialogue();
        void CORE_COMMAND_addUser(const std::vector<std::string>& args);

        void CORE_COMMAND_deleteUserWithDialogue();
        void CORE_COMMAND_deleteUser(const std::vector<std::string>& args);

        void CORE_COMMAND_renameUserWithDialogue();
        void CORE_COMMAND_renameUser(const std::vector<std::string>& args);

        void CORE_COMMAND_setPermissionsUserWithDialogue();
        void CORE_COMMAND_setPermissionsUser(const std::vector<std::string>& args);

        //void CORE_COMMAND_addLocalVar();
        //void CORE_COMMAND_renameLocalVar();
        //void CORE_COMMAND_editLocalVarFunction();
        //void CORE_COMMAND_editLocalVarDescription();

        //   -------------- Users ---------------

        void CORE_COMMAND_whoim();
        void CORE_COMMAND_infoUser(const std::vector<std::string>& args);
        void CORE_COMMAND_allInfoUsers();
        void CORE_COMMAND_logout();

        //   ---------------- Other ---------------

        //void CORE_COMMAND_addSystemVar();
        void CORE_COMMAND_allSystemVars();
        void CORE_COMMAND_allLocalVars();
    }
}
#endif