#pragma once

void CORE_COMMAND_help(std::vector<std::string> args);
void CORE_COMMAND_info(std::vector<std::string> args);

//   -------------- Pseudo FS Commands ---------------

void CORE_COMMAND_cd(std::vector<std::string> args);
void CORE_COMMAND_tree(std::vector<std::string> args);

//   -------------- Users "Manager" ---------------

void CORE_COMMAND_addUser(std::vector<std::string> args);
void CORE_COMMAND_deleteUser(std::vector<std::string> args);
void CORE_COMMAND_renameUser(std::vector<std::string> args);
void CORE_COMMAND_setPermissionsUser(std::vector<std::string> args);

//   -------------- Users ---------------

void CORE_COMMAND_infoUser(std::vector<std::string> args);
void CORE_COMMAND_allInfoUsers(std::vector<std::string> args);
void CORE_COMMAND_logout(std::vector<std::string> args);