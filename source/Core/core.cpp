#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <filesystem>
#include "users/user_manager.cpp"
#include "data/file_manager.cpp"
#include "data/data_manager.cpp"
#include "base/command/commands.cpp"
#include "base/print.cpp"
#include "filesystem/pseudo_fs.cpp"
#include "users/OOBE.cpp"
#include "Fix.cpp"

void Init() {
    userManager UM;
    UM.checkOOBE_Passed();
    if (!UM.getOOBE_Passed() && UM.YourUsername().empty()) {
        fileManager FM;
        const std::vector<std::filesystem::path> Folders = {"Data", "Data/Users", "Modules", "FS", "Temp"};
        FM.createFolders(Folders);
        FM.createFiles({"Data/MainData.json", "Data/Users.json"});
        dataManager DM;
        DM.createData("Data/MainData.json", "OOBE_Passed", "0");
    }
    else {
        UM.userListManagerStart();
    }
}