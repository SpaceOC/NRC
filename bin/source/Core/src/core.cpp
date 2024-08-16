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
#include <string>
#include <vector>
#include <filesystem>
#include "Core/base/users/OOBE.h"
#include "Core/base/filesystem/pseudo_fs.h"
#include "Core/base/users/user_manager.h"
#include "Core/base/data/file_manager.h"
#include "Core/base/data/data_manager.h"
//#include "Core/experimental/python_script_support.h"

void Init() {
    userManager UM;
    UM.checkOOBE_Passed();
    if (!UM.getOOBE_Passed() && UM.yourUsername().empty()) {
        fileManager FM;
        FM.createFolders({"Data", "Data/Users", "Modules", "FS", "Temp"});
        FM.createFiles({"Data/MainData.json", "Data/Users.json"});
        dataManager DM;
        DM.createData("Data/MainData.json", "OOBE_Passed", "0");
        OOBE();
    }
    else {
        UM.userLists();
    }
}