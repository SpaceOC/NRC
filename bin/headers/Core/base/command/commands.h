#pragma once
#ifndef COMMANDS
#define COMMANDS

void CORE_COMMAND_help();
void CORE_COMMAND_info();

//   -------------- Pseudo FS Commands ---------------

void CORE_COMMAND_cd();
void CORE_COMMAND_tree();

//   -------------- Users "Manager" ---------------

void CORE_COMMAND_addUser();
void CORE_COMMAND_deleteUser();
void CORE_COMMAND_renameUser();
void CORE_COMMAND_setPermissionsUser();

//   -------------- Users ---------------

void CORE_COMMAND_infoUser();
void CORE_COMMAND_allInfoUsers();
void CORE_COMMAND_logout();
#endif