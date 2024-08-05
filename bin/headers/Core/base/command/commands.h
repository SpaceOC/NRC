// Commands header
// by SpaceOC
#pragma once
#ifndef NRC_BASE_COMMAND_COMMANDS_H_
#define NRC_BASE_COMMAND_COMMANDS_H_

void CORE_COMMAND_help();
void CORE_COMMAND_info();
//void CORE_COMMAND_exit();

//   -------------- Pseudo FS Commands ---------------

//void CORE_COMMAND_cd();
//void CORE_COMMAND_tree();

//   -------------- Users "Manager" ---------------

void CORE_COMMAND_addUser();
void CORE_COMMAND_deleteUser();
void CORE_COMMAND_renameUser();
void CORE_COMMAND_setPermissionsUser();
//void CORE_COMMAND_addLocalVar();
//void CORE_COMMAND_renameLocalVar();
//void CORE_COMMAND_editLocalVarFunction();
//void CORE_COMMAND_editLocalVarDescription();

//   -------------- Users ---------------

void CORE_COMMAND_infoUser();
void CORE_COMMAND_allInfoUsers();
void CORE_COMMAND_logout();

//   ---------------- Other ---------------

//void CORE_COMMAND_addSystemVar();
void CORE_COMMAND_allSystemVars();
void CORE_COMMAND_allLocalVars();

#endif