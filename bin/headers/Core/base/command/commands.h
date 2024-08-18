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