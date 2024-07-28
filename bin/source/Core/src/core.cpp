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