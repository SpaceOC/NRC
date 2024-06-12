#include <string>
#include <vector>
#include <filesystem>
#include "Core/filesystem/pseudo_fs.h"
#include "Core/users/user_manager.h"
#include "Core/data/file_manager.h"
#include "Core/data/data_manager.h"
//#include "Core/experimental/python_script_support.h"

void Init() {
    pseudoFSBase FS;
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