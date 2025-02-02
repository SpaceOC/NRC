#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <ctime>
#include <chrono>
#include "Core/base/print.h"
#include "Core/base/command/commands.h"
#include "Core/base/command/handler_commands.h"
#include "Core/CORE_info.h"
#include "Core/base/users/user_manager.h"
#include "Core/base/users/user.h"
#include "Core/base/filesystem/pseudo_fs.h"
#include "Core/base/filesystem/nrfs.h"
#include "Core/base/utils.h"
#include "Core/extra/variables.h"

std::string core::commands::CORE_COMMAND_help(core::User*, const std::vector<std::string>& args, bool returnable) {
    if (args.empty()) {
        std::string result;
        for (const auto& command : core::handlerCommands()->getAllCommands()) {
            std::string argsNames = "";
            if (!command.second.argsNames.empty()) {
                for (auto arg : command.second.argsNames) {
                    argsNames += " <" + arg + ">";
                }
            }
            if (!returnable)
                print(command.first + argsNames + command.second.description + '\n', PrintColors::light_green);
            else
                result += command.first + argsNames + command.second.description + "\n";
        }
        if (returnable)
            return result;
    }
    else {
        if (core::handlerCommands()->getCommand(args.at(0)).empty()) {
            if (!returnable)
                core::print("Command not found\n");
            else {
                return "Command not found\n";
            }
        }
        else {
            std::map<std::string, core::CommandDescription> temp = core::handlerCommands()->getCommand(args.at(0));
            std::string argsNames = "";
            if (!temp[args.at(0)].argsNames.empty()) {
                for (auto arg : temp[args.at(0)].argsNames) {
                    argsNames += " <" + arg + ">";
                }
            }
            if (!returnable)
                print(args.at(0) + argsNames + temp[args.at(0)].description + '\n', PrintColors::light_green);
            else
                return args.at(0) + argsNames + temp[args.at(0)].description + "\n";
        }
    }
    return "";
};

std::string core::commands::CORE_COMMAND_info(core::User*, bool returnable) {
    if (!returnable) {
        std::cout << CORE_NAME << " by " << CORE_DEVELOPER << '\n' << CORE_VERSION << " " << CORE_VERSION_TYPE << '\n';
        std::cout << "NRFS by SpaceOC" << '\n' << NRFS_VERSION << " " << NRFS_VERSION_TYPE << '\n';
        std::cout << "JSON library by nlohmann - https://github.com/nlohmann/json \n";
        std::cout << " --- Special Thanks ---\nAlone Knight - migrating NRC from Makefile to CMake\n";
        return "";
    }

    std::string result = CORE_NAME + std::string(" by ") + CORE_DEVELOPER + "\n" + CORE_VERSION + std::string(" ") + CORE_VERSION_TYPE + "\n";
    result += "NRFS (Not Real FileSystem) by SpaceOC\n" + std::string(NRFS_VERSION) + " " + NRFS_VERSION_TYPE + "\n";
    result += "JSON library by nlohmann - https://github.com/nlohmann/json \n";
    result += " --- Special Thanks ---\nAlone Knight - migrating NRC from Makefile to CMake\n";
    return result;
};

std::string core::commands::CORE_COMMAND_time(core::User* who, const std::vector<std::string>& args, bool returnable) {
    auto start = std::chrono::steady_clock::now();
    std::string temp;

    for (const std::string& anotherTemp : args) {
        if (anotherTemp != args.at(0))
            temp += (anotherTemp == core::handlerCommands()->getCommandSeparator() ? "" : "\"") + anotherTemp + (anotherTemp != args.back() ? (anotherTemp == core::handlerCommands()->getCommandSeparator() ? "" : "\" ") : (anotherTemp == core::handlerCommands()->getCommandSeparator() ? "" : "\""));
        else
            temp += anotherTemp + " ";
    }
    core::CommandObject command = core::handlerCommands()->parsing(temp, who, returnable).at(0);
    std::string result;
    if (!returnable)
        core::handlerCommands()->sendCommand(who, command);
    else
        core::handlerCommands()->sendCommand(who, command, result);
    auto end = std::chrono::steady_clock::now();
    long long duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    if (!returnable) {
        core::print("time: ", core::PrintColors::light_green);
        std::cout << duration;
        core::print("ms\n", core::PrintColors::light_green);
        return "";
    }
    result += "\ntime: " + Utils::valueToString(duration) + "ms";
    return result;
}

/*
std::string core::commands:: core::commands:: CORE_COMMAND_exit() {
    work = false;
}
*/

//   -------------- Pseudo FS Commands ---------------


std::string core::commands::CORE_COMMAND_cd(core::User* /*who*/, const std::vector<std::string>& args, bool returnable) {
    if (args.at(0).empty()) {
        if (!returnable) 
            core::print("Command error: The first argument cannot be empty!\n", core::PrintColors::red);
        return "Command error: The first argument cannot be empty!";
    }
    else {
        std::string where = (args.at(0) == ".." && !args.at(0)._Starts_with("./") ? ".." : args.at(0));
        
        int code;
        size_t curDisk = core::pseudoFS()->getCurDiskId();
        FolderData* folder = (where == ".." ? nullptr :
            new FolderData(core::pseudoFS()->getFolderData(
                (
                (args.at(0)._Starts_with("./") ? args.at(0) : core::pseudoFS()->getCurrentPath() + where)
                ),
                curDisk,
                code
            )
        ));
        if ((where != ".." || args.at(0)._Starts_with("./"))) {
            if (folder && folder->link != nullptr && !folder->linkPath.empty()) {
                if (!core::pseudoFS()->changePath(folder->linkPath, curDisk)) {
                    if (!returnable)
                        core::print("Error: Change path operation failed!\n", core::PrintColors::red);
                    return "Error: Change path operation failed!";
                }
            }
            else {
                if (!core::pseudoFS()->changeDirectory(where, curDisk)) {
                    if (!returnable) 
                        core::print("Error: Change directory operation failed!\n", core::PrintColors::red);
                    return "Error: Change directory operation failed!";
                }
            }
        }
        else {
            if (!core::pseudoFS()->changeDirectory(where, curDisk)) {
                if (!returnable) 
                    core::print("Error: Change directory operation failed!\n", core::PrintColors::red);
                return "Error: Change directory operation failed!";
            }
        }
    }
    return "";
}

std::string core::commands::CORE_COMMAND_createFile(core::User* who, const std::vector<std::string>& args, bool returnable) {
    if (args.at(0).empty()) {
        if (!returnable)
            core::print("Command error: The first argument cannot be empty!\n", core::PrintColors::red);
        return "Command error: The first argument cannot be empty!";
    }

    std::string checkResult = core::checkUserPermissionsForCommand(who);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    std::string where = (args.at(0)._Starts_with("./") ? args.at(0) : core::pseudoFS()->getCurrentPath() + args.at(0));
    size_t curDisk = core::pseudoFS()->getCurDiskId();
    int code = core::pseudoFS()->createFile(where, curDisk);
    if (code == core::PseudoFSCodes::ALREADY_EXISTS) {
        FileData file{
            "",
            "",
            0,
            std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()),
            std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()),
            false,
            false,
            nullptr,
            ""
        };
        fixCreateSamePFSObject(where, curDisk, file, code);
    }
    
    checkResult = checkFileCodeForPFSCommand(code);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    if (args.size() > 1 && args.at(1).find("o") != std::string::npos)
        code = core::pseudoFS()->setFileAtt(where, curDisk, "owner", who);
    checkResult = checkFileCodeForPFSCommand(code);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    core::pseudoFS()->savePFS();
    return "";
}

std::string core::commands::CORE_COMMAND_createLinkFile(core::User* who, const std::vector<std::string>& args, bool returnable) {
    if (args.at(0).empty() || args.at(1).empty()) {
        if (!returnable)
            core::print("Command error: One of the arguments is empty!\n", core::PrintColors::red);
        return "Command error: One of the arguments is empty!";
    }

    std::string checkResult = core::checkUserPermissionsForCommand(who);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    std::string where = (args.at(0)._Starts_with("./") ? args.at(0) : core::pseudoFS()->getCurrentPath() + args.at(0));
    std::string where2 = (args.at(1)._Starts_with("./") ? args.at(1) : core::pseudoFS()->getCurrentPath() + args.at(1));
    int code;
    size_t curDisk = core::pseudoFS()->getCurDiskId();
    FileData file = core::pseudoFS()->getFileData(where, curDisk, code);
    checkResult = core::checkFileCodeForPFSCommand(code);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    core::FileData* linkFile = new core::FileData{
        core::Utils::split(where2, '/').back(),
        "",
        -1,
        0,
        0,
        false,
        file.hidden,
        &file,
        where,
        file.owner
    };

    code = core::pseudoFS()->createFile(where2, curDisk, linkFile);
    if (code == core::PseudoFSCodes::ALREADY_EXISTS)
        fixCreateSamePFSObject(where2, curDisk, *linkFile, code);
    
    checkResult = core::checkFileCodeForPFSCommand(code);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    core::pseudoFS()->savePFS();
    return "";
}

std::string core::commands::CORE_COMMAND_deleteFile(core::User* who, const std::vector<std::string>& args, bool returnable) {
    if (args.at(0).empty()) {
        if (!returnable)
            core::print("Command error: The first argument cannot be empty!\n", core::PrintColors::red);
        return "Command error: The first argument cannot be empty!";
    }

    std::string checkResult = core::checkUserPermissionsForCommand(who);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    std::string where = (args.at(0)._Starts_with("./") ? args.at(0) : core::pseudoFS()->getCurrentPath() + args.at(0));
    int code;
    size_t curDisk = core::pseudoFS()->getCurDiskId();
    core::pseudoFS()->getFileData(where, curDisk, code);
    checkResult = core::checkFileCodeForPFSCommand(code);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    code = core::pseudoFS()->deleteFile(where, curDisk);
    checkResult = core::checkFileCodeForPFSCommand(code);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    core::pseudoFS()->savePFS();
    return "";
}

std::string core::commands::CORE_COMMAND_renameFile(core::User* who, const std::vector<std::string>& args, bool returnable) {
    if (args.at(0).empty() || args.at(1).empty()) {
        if (!returnable)
            core::print("Command error: One of the arguments is empty!\n", core::PrintColors::red);
        return "Command error: One of the arguments is empty!";
    }

    std::string checkResult = core::checkUserPermissionsForCommand(who);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    std::string where = (args.at(0)._Starts_with("./") ? args.at(0) : core::pseudoFS()->getCurrentPath() + args.at(0));
    int code;
    size_t curDisk = core::pseudoFS()->getCurDiskId();
    FileData f = core::pseudoFS()->getFileData(where, curDisk, code);
    checkResult = core::checkFileCodeForPFSCommand(code);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    checkResult = core::checkUserPermissionsForPFSCommand(who, f);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    code = core::pseudoFS()->renameFile(args.at(0), curDisk, args.at(1));
    if (code == core::PseudoFSCodes::ALREADY_EXISTS) {
        f.dataEdit = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        fixCreateSamePFSObject(where, curDisk, f, code);
    }

    checkResult = core::checkFileCodeForPFSCommand(code);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    core::pseudoFS()->savePFS();
    return "";
}

std::string core::commands::CORE_COMMAND_moveFile(core::User* who, const std::vector<std::string>& args, bool returnable) {
    if (args.at(0).empty() || args.at(1).empty()) {
        if (!returnable)
            core::print("Command error: One of the arguments is empty!\n", core::PrintColors::red);
        return "Command error: One of the arguments is empty!";
    }

    std::string checkResult = core::checkUserPermissionsForCommand(who);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    int code;
    size_t curDisk = core::pseudoFS()->getCurDiskId();
    FileData f = core::pseudoFS()->getFileData(args.at(0), curDisk, code);
    checkResult = core::checkFileCodeForPFSCommand(code);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    checkResult = core::checkUserPermissionsForPFSCommand(who, f);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    code = core::pseudoFS()->moveFile(args.at(0), curDisk, args.at(1), curDisk);
    if (code == core::PseudoFSCodes::ALREADY_EXISTS)
        fixCreateSamePFSObject(args.at(0), curDisk, f, code);

    checkResult = core::checkFileCodeForPFSCommand(code);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    core::pseudoFS()->savePFS();
    return "";
}

std::string core::commands::CORE_COMMAND_editFile(core::User* who, const std::vector<std::string>& args, bool returnable) {
    if (args.at(0).empty()) {
        if (!returnable)
            core::print("Command error: Arguments is empty!\n", core::PrintColors::red);
        return "Command error: Arguments is empty!";
    }
    std::string checkResult = core::checkUserPermissionsForCommand(who);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    int code;
    size_t curDisk = core::pseudoFS()->getCurDiskId();
    FileData f = core::pseudoFS()->getFileData(args.at(0), curDisk, code);
    checkResult = core::checkFileCodeForPFSCommand(code);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    checkResult = core::checkUserPermissionsForPFSCommand(who, f);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    #ifndef NRC_WEB
    if (!returnable) {
        if (args.size() < 2) {
            core::print("--- Write ':quit:file' to quit editing the file and save all the contents. ---\n");
            size_t linen = 0;
            std::string text, line;
            while (true) {
                core::print(DEFAULT_PRINT_COLOR, "Line ", linen, " > ");
                std::cin >> line;
                if (line == ":quit:file") break;
                if (!line.empty()) {
                    text += line + "\n";
                    ++linen;
                }
            }
            if (text.empty()) return "";
            core::pseudoFS()->setFileAtt(args.at(0), curDisk, "content", text);
            core::pseudoFS()->savePFS();
        }
        else if (args.size() > 1) {
            if (args.at(1).empty()) return "";
            core::pseudoFS()->setFileAtt(args.at(0), curDisk, "content", args.at(1));
            core::pseudoFS()->savePFS();
        }
        return "";
    }
    #endif

    if (args.size() > 1) {
        if (args.at(1).empty()) return "Command error: The second argument of the command is empty!";
        std::string oldContent = core::pseudoFS()->getFileData(args.at(0), curDisk, code).content;
        core::pseudoFS()->setFileAtt(args.at(0), curDisk, "content", args.at(1));
        core::pseudoFS()->savePFS();
        return "Update file content!\nOld: " + oldContent + "\nNew: " + args.at(1);
    }
    core::pseudoFS()->savePFS();
    return "";
}

std::string core::commands::CORE_COMMAND_showFileData(core::User* who, const std::vector<std::string>& args, bool returnable) {
    if (args.at(0).empty()) {
        if (!returnable)
            core::print("Command error: Arguments is empty!\n", core::PrintColors::red);
        return "Command error: Arguments is empty!";
    }
    std::string checkResult = core::checkUserPermissionsForCommand(who);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    int code;
    size_t curDisk = core::pseudoFS()->getCurDiskId();
    std::string where = (args.at(0)._Starts_with("./") ? args.at(0) : core::pseudoFS()->getCurrentPath() + args.at(0));
    const FileData targetFile = core::pseudoFS()->getFileData(where, curDisk, code);
    checkResult = core::checkFileCodeForPFSCommand(code);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    std::string fileOwnerUsername = (targetFile.owner ? targetFile.owner->getUsername() : (targetFile.system ? "[ SYSTEM ]" : "[ NONE ]"));
        
    if (!returnable) {
        core::print("File name: " + targetFile.name + "\n", core::PrintColors::aqua);
        core::print("File content: " + (targetFile.content.empty() ? targetFile.content : "\n'''\n" + targetFile.content) + "\n'''\n", core::PrintColors::aqua);
        core::print("File ID: " + std::to_string(targetFile.id) + "\n", core::PrintColors::aqua);
        core::print("File owner: " + fileOwnerUsername + "\n", core::PrintColors::aqua);
    } else {
        std::string result = "File name: " + targetFile.name + "\n";
        result += "File content: " + (targetFile.content.empty() ? targetFile.content : "\n'''\n" + targetFile.content) + "\n'''\n" + "\n";
        result += "File ID: " + std::to_string(targetFile.id) + "\n";
        result += "File owner: " + fileOwnerUsername + "\n";
        return result;
    }
    return "";
}

std::string core::commands::CORE_COMMAND_writeOnNewLineFile(core::User* who, const std::vector<std::string>& args, bool returnable) {
    if (args.at(0).empty() || args[1].empty()) {
        if (!returnable)
            core::print("Command error: Arguments is empty!\n", core::PrintColors::red);
        return "Command error: Arguments is empty!";
    }
    std::string checkResult = core::checkUserPermissionsForCommand(who);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    size_t curDisk = core::pseudoFS()->getCurDiskId();
    int code;
    const FileData f = core::pseudoFS()->getFileData(args.at(0), curDisk, code);
    std::string content = f.content + "\n" + args.at(1);
    checkResult = core::checkFileCodeForPFSCommand(code);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    checkResult = core::checkUserPermissionsForPFSCommand(who, f);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    core::pseudoFS()->setFileAtt(args.at(0), curDisk, "content", content);
    core::pseudoFS()->savePFS();
    return "";
}

std::string core::commands::CORE_COMMAND_writeFile(core::User* who, const std::vector<std::string>& args, bool returnable) {
    if (args.at(0).empty() || args[1].empty()) {
        if (!returnable)
            core::print("Command error: Arguments is empty!\n", core::PrintColors::red);
        return "Command error: Arguments is empty!";
    }
    std::string checkResult = core::checkUserPermissionsForCommand(who);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    int code;
    size_t curDisk = core::pseudoFS()->getCurDiskId();
    const FileData f = core::pseudoFS()->getFileData(args.at(0), curDisk, code);
    std::string content = f.content + args.at(1);
    checkResult = core::checkFileCodeForPFSCommand(code);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    checkResult = core::checkUserPermissionsForPFSCommand(who, f);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    core::pseudoFS()->setFileAtt(args.at(0), curDisk, "content", content);
    core::pseudoFS()->savePFS();
    return "";
}

std::string core::commands::CORE_COMMAND_rewriteFile(core::User* who, const std::vector<std::string>& args, bool returnable) {
    if (args.at(0).empty() || args[1].empty()) {
        if (!returnable)
            core::print("Command error: Arguments is empty!\n", core::PrintColors::red);
        return "Command error: Arguments is empty!";
    }
    std::string checkResult = core::checkUserPermissionsForCommand(who);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    int code;
    size_t curDisk = core::pseudoFS()->getCurDiskId();
    const FileData f = core::pseudoFS()->getFileData(args.at(0), curDisk, code);
    checkResult = core::checkFileCodeForPFSCommand(code);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    checkResult = core::checkUserPermissionsForPFSCommand(who, f);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    core::pseudoFS()->setFileAtt(args.at(0), curDisk, "content", args.at(1));
    core::pseudoFS()->savePFS();
    return "";
}

std::string core::commands::CORE_COMMAND_clearFile(core::User* who, const std::vector<std::string>& args, bool returnable) {
    if (args.at(0).empty()) {
        if (!returnable)
            core::print("Command error: Arguments is empty!\n", core::PrintColors::red);
        return "Command error: Arguments is empty!";
    }
    std::string checkResult = core::checkUserPermissionsForCommand(who);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    int code;
    size_t curDisk = core::pseudoFS()->getCurDiskId();
    const FileData f = core::pseudoFS()->getFileData(args.at(0), curDisk, code);
    checkResult = core::checkFileCodeForPFSCommand(code);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    checkResult = core::checkUserPermissionsForPFSCommand(who, f);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    core::pseudoFS()->setFileAtt(args.at(0), curDisk, "content", "");
    core::pseudoFS()->savePFS();
    return "";
}

std::string core::commands::CORE_COMMAND_setNewFileOwner(core::User* who, const std::vector<std::string>& args, bool returnable) {
    if (args.at(0).empty() || args[1].empty()) {
        if (!returnable)
            core::print("Command error: Arguments is empty!\n", core::PrintColors::red);
        return "Command error: Arguments is empty!";
    }
    std::string checkResult = core::checkUserPermissionsForCommand(who);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    int code;
    size_t curDisk = core::pseudoFS()->getCurDiskId();
    const FileData f = core::pseudoFS()->getFileData(args.at(0), curDisk, code);
    checkResult = core::checkFileCodeForPFSCommand(code);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    checkResult = core::checkUserPermissionsForPFSCommand(who, f);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    core::pseudoFS()->setFileAtt(args.at(0), curDisk, "owner", (args.at(1) != "[ NONE ]" ? &core::userManager()->getUser(args.at(1)) : nullptr));
    core::pseudoFS()->savePFS();
    return "";
}

std::string core::commands::CORE_COMMAND_createFolder(core::User* who, const std::vector<std::string>& args, bool returnable) {
    if (args.at(0).empty()) {
        if (!returnable)
            core::print("Command error: Arguments is empty!\n", core::PrintColors::red);
        return "Command error: Arguments is empty!";
    }
    std::string checkResult = core::checkUserPermissionsForCommand(who);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    int code;
    size_t curDisk = core::pseudoFS()->getCurDiskId();
    std::string where = (args.at(0)._Starts_with("./") ? args.at(0) : core::pseudoFS()->getCurrentPath() + args.at(0));
    code = core::pseudoFS()->createFolder(where, curDisk);
    if (code == core::PseudoFSCodes::ALREADY_EXISTS) {
        FolderData folder{
            "",
            0,
            std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()),
            std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()),
            {},
            {},
            false,
            false,
            nullptr
        };
        fixCreateSamePFSObject(where, curDisk, folder, code);
    }

    if (args.size() > 1 && args.at(1).find("o") != std::string::npos)
        core::pseudoFS()->setFileAtt(where, curDisk, "owner", who);
    core::pseudoFS()->savePFS();
    return "";
}

std::string core::commands::CORE_COMMAND_createLinkFolder(core::User* who, const std::vector<std::string>& args, bool returnable) {
    if (args.at(0).empty()) {
        if (!returnable)
            core::print("Command error: Arguments is empty!\n", core::PrintColors::red);
        return "Command error: Arguments is empty!";
    }
    std::string checkResult = core::checkUserPermissionsForCommand(who);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    std::string where = (args.at(0)._Starts_with("./") ? args.at(0) : core::pseudoFS()->getCurrentPath() + args.at(0));
    std::string where2 = (args.at(1)._Starts_with("./") ? args.at(1) : core::pseudoFS()->getCurrentPath() + args.at(1));
    int code;
    size_t curDisk = core::pseudoFS()->getCurDiskId();
    FolderData folder = core::pseudoFS()->getFolderData(where, curDisk, code);
    checkResult = core::checkFileCodeForPFSCommand(code);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    FolderData* linkFolder = new FolderData{
        core::Utils::split(where2, '/').back(),
        -1,
        0,
        0,
        {},
        {},
        false,
        folder.hidden,
        &folder,
        where
    };

    code = core::pseudoFS()->createFolder(where2, curDisk, linkFolder);
    if (code == core::PseudoFSCodes::ALREADY_EXISTS)
        fixCreateSamePFSObject(where, curDisk, *linkFolder, code);

    if (args.size() > 1 && args.at(1).find("o") != std::string::npos)
        core::pseudoFS()->setFileAtt(where, curDisk, "owner", who);

    core::pseudoFS()->savePFS();
    return "";
}

std::string core::commands::CORE_COMMAND_deleteFolder(core::User* who, const std::vector<std::string>& args, bool returnable) {
    if (args.at(0).empty()) {
        if (!returnable)
            core::print("Command error: Arguments is empty!\n", core::PrintColors::red);
        return "Command error: Arguments is empty!";
    }
    std::string checkResult = core::checkUserPermissionsForCommand(who);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    std::string where = (args.at(0)._Starts_with("./") ? args.at(0) : core::pseudoFS()->getCurrentPath() + args.at(0));
    size_t curDisk = core::pseudoFS()->getCurDiskId();
    int code;
    const FolderData f = core::pseudoFS()->getFolderData(where, curDisk, code);
    checkResult = core::checkFileCodeForPFSCommand(code);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    checkResult = core::checkUserPermissionsForPFSCommand(who, f);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    core::pseudoFS()->deleteFolder(where, curDisk);
    core::pseudoFS()->savePFS();
    return "";
}

std::string core::commands::CORE_COMMAND_renameFolder(core::User* who, const std::vector<std::string>& args, bool returnable) {
    if (args.at(0).empty() || args[1].empty()) {
        if (!returnable)
            core::print("Command error: Arguments is empty!\n", core::PrintColors::red);
        return "Command error: Arguments is empty!";
    }
    std::string checkResult = core::checkUserPermissionsForCommand(who);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    std::string where = (args.at(0)._Starts_with("./") ? args.at(0) : core::pseudoFS()->getCurrentPath() + args.at(0));
    size_t curDisk = core::pseudoFS()->getCurDiskId();
    int code;
    FolderData f = core::pseudoFS()->getFolderData(where, curDisk, code);
    if (code == core::PseudoFSCodes::ALREADY_EXISTS) {
        f.dataEdit = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        fixCreateSamePFSObject(where, curDisk, f, code);
    }
    
    checkResult = core::checkFileCodeForPFSCommand(code);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    checkResult = core::checkUserPermissionsForPFSCommand(who, f);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    core::pseudoFS()->renameFolder(where, curDisk, args.at(1));
    core::pseudoFS()->savePFS();
    return "";
}

std::string core::commands::CORE_COMMAND_moveFolder(core::User* who, const std::vector<std::string>& args, bool returnable) {
     if (args.at(0).empty() || args[1].empty()) {
        if (!returnable)
            core::print("Command error: Arguments is empty!\n", core::PrintColors::red);
        return "Command error: Arguments is empty!";
    }
    std::string checkResult = core::checkUserPermissionsForCommand(who);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    size_t curDisk = core::pseudoFS()->getCurDiskId();
    int code;
    FolderData f = core::pseudoFS()->getFolderData(args.at(0), curDisk, code);
    if (code == core::PseudoFSCodes::ALREADY_EXISTS) {
        fixCreateSamePFSObject(args.at(0), curDisk, f, code);
    }

    checkResult = core::checkFileCodeForPFSCommand(code);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    checkResult = core::checkUserPermissionsForPFSCommand(who, f);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    code = core::pseudoFS()->moveFolder(args.at(0), curDisk, args.at(1), curDisk);
    if (code == core::PseudoFSCodes::ALREADY_EXISTS)
        fixCreateSamePFSObject(args.at(0), curDisk, f, code);

    core::pseudoFS()->savePFS();
    return "";
}

std::string core::commands::CORE_COMMAND_showFolderData(core::User* who, const std::vector<std::string>& args, bool returnable) {
     if (args.at(0).empty() || args[1].empty()) {
        if (!returnable)
            core::print("Command error: Arguments is empty!\n", core::PrintColors::red);
        return "Command error: Arguments is empty!";
    }
    std::string checkResult = core::checkUserPermissionsForCommand(who);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    size_t curDisk = core::pseudoFS()->getCurDiskId();
    int code;
    std::string where = (args.at(0)._Starts_with("./") ? args.at(0) : core::pseudoFS()->getCurrentPath() + args.at(0));
    const FolderData targetFolder = core::pseudoFS()->getFolderData(where, curDisk, code);
    checkResult = core::checkFileCodeForPFSCommand(code);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    std::string folderOwnerUsername = (targetFolder.owner ? targetFolder.owner->getUsername() : (targetFolder.system ? "[ SYSTEM ]" : "[ NONE ]"));
        
    if (!returnable) {
        core::print("Folder name: " + targetFolder.name + "\n", core::PrintColors::aqua);
        core::print("Folder ID: " + std::to_string(targetFolder.id) + "\n", core::PrintColors::aqua);
        core::print("Folder owner: " + folderOwnerUsername + "\n");
    }
    else {
        std::string result = "Folder name: " + targetFolder.name + "\n";
        result += "Folder ID: " + std::to_string(targetFolder.id) + "\n";
        result += "Folder owner: " + folderOwnerUsername + "\n";
        return result;
    }
    return "";
}

std::string core::commands::CORE_COMMAND_setNewFolderOwner(core::User* who, const std::vector<std::string>& args, bool returnable) {
     if (args.at(0).empty() || args[1].empty()) {
        if (!returnable)
            core::print("Command error: Arguments is empty!\n", core::PrintColors::red);
        return "Command error: Arguments is empty!";
    }
    std::string checkResult = core::checkUserPermissionsForCommand(who);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    size_t curDisk = core::pseudoFS()->getCurDiskId();
    int code;
    std::string where = (args.at(0)._Starts_with("./") ? args.at(0) : core::pseudoFS()->getCurrentPath() + args.at(0));
    const FolderData targetFolder = core::pseudoFS()->getFolderData(where, curDisk, code);
    checkResult = core::checkFileCodeForPFSCommand(code);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    checkResult = core::checkUserPermissionsForPFSCommand(who, targetFolder);
    COMMAND_ERROR_OUTPUT(checkResult, returnable)

    core::pseudoFS()->setFolderAtt(args.at(0), curDisk, "owner", (args.at(1) != "[ NONE ]" ? &core::userManager()->getUser(args.at(1)) : nullptr));
    core::pseudoFS()->savePFS();
    return "";
}

std::string core::commands::CORE_COMMAND_dir(core::User*, const std::vector<std::string>& args, bool returnable) {
    bool showHidden = (args.empty() ? false : args.at(0) == "h");
    int fCount = 0;
    int code;
    size_t curDisk = core::pseudoFS()->getCurDiskId();
    const std::vector<FolderData*> folders = (core::pseudoFS()->getCurrentPath() == "./" ? core::pseudoFS()->getNRFS()->getRoot()->getFolders() : core::pseudoFS()->getFolderData(core::pseudoFS()->getCurrentPath(), curDisk, code).folders);
    const std::vector<FileData*> files = (core::pseudoFS()->getCurrentPath() == "./" ? core::pseudoFS()->getNRFS()->getRoot()->getFiles() : core::pseudoFS()->getFolderData(core::pseudoFS()->getCurrentPath(), curDisk, code).files);
    
    std::string output;
    for (core::FolderData* folder : folders) {
        if (folder == nullptr) continue;
        if (folder->hidden && showHidden) {
            if (!returnable) {
                core::print(folder->name + "    " + (fCount >= 5 ? "\n" : ""), core::PrintColors::light_aqua);
            } else {
                output += folder->name + "    " + (fCount >= 5 ? "\n" : "");
            }
            if (fCount >= 5) fCount = 0;
            else ++fCount;
        }
        else if (!folder->hidden) {
            if (!returnable) {
                std::cout << folder->name + "    " + (fCount >= 5 ? "\n" : "");
            } else {
                output += folder->name + "    " + (fCount >= 5 ? "\n" : "");
            }
            if (fCount >= 5) fCount = 0;
            else ++fCount;
        }
    }
    for (core::FileData* file : files) {
        if (file == nullptr) continue;
        if (file->hidden && showHidden) {
            if (!returnable) {
                core::print(file->name + "    " + (fCount >= 5 ? "\n" : ""), core::PrintColors::light_aqua);
            } else {
                output += file->name + "    " + (fCount >= 5 ? "\n" : "");
            }
            if (fCount >= 5) fCount = 0;
            else ++fCount;
        }
        else if (!file->hidden) {
            if (!returnable) {
                std::cout << file->name + "    " + (fCount >= 5 ? "\n" : "");
            } else {
                output += file->name + "    " + (fCount >= 5 ? "\n" : "");
            }
            if (fCount >= 5) fCount = 0;
            else ++fCount;
        }
    }
    if (!returnable) {
        std::cout << '\n';
    } else {
        return output + "\n";
    }
    return "";
}

std::string core::commands::CORE_COMMAND_tree(core::User*, const std::vector<std::string>& args, bool returnable) {
    size_t curDisk = core::pseudoFS()->getCurDiskId();
    if (args.empty()) {
        if (!returnable) {
            core::pseudoFS()->showTree(false, curDisk, false);
        } else {
            std::string str;
            core::pseudoFS()->showTree(false, curDisk, false, "./", str);
            return str;
        }
    }
    else {
        if (!returnable) {
            core::pseudoFS()->showTree(
                (args.size() > 1 ? (args.at(1).find("h") != std::string::npos) : false),
                curDisk,
                (args.size() > 1 ? (args.at(1).find("f") != std::string::npos) : false),
                args.at(0)
            );
        } else {
            std::string str;
            core::pseudoFS()->showTree(
                (args.size() > 1 ? (args.at(1).find("h") != std::string::npos) : false),
                curDisk,
                (args.size() > 1 ? (args.at(1).find("f") != std::string::npos) : false),
                args.at(0),
                str
            );
            return str;
        }
    }
    return "";
}

std::string core::commands::CORE_COMMAND_showAll(core::User*, const std::vector<std::string>& args, bool returnable) {
    size_t curDisk = core::pseudoFS()->getCurDiskId();
    if (args.empty()) {
        if (!returnable) {
            core::pseudoFS()->printAll(false, curDisk);
        } else {
            std::string str;
            core::pseudoFS()->printAll(false, curDisk, "./", str);
            return str;
        }
    }
    else {
        if (!returnable) {
            core::pseudoFS()->printAll(
                (args.size() > 1 ? args.at(1).find("h") != std::string::npos : false),
                curDisk,
                args.at(0)
            );
        } else {
            std::string str;
            core::pseudoFS()->printAll(
                (args.size() > 1 ? args.at(1).find("h") != std::string::npos : false),
                curDisk,
                args.at(0),
                str
            );
            return str;
        }
    }
    return "";
}

std::string core::commands::CORE_COMMAND_searchFile(core::User*, const std::vector<std::string>& args, bool returnable) {
    if (!returnable && args.empty()) {
        core::print("Error: Args is empty!", core::PrintColors::red);
        return "";
    } else if (returnable && args.empty()) {
        return "Error: Agrs is empty!";
    }
    std::string result = "";
    std::string path = (args.size() > 1 ? args.at(1) : core::pseudoFS()->getCurrentPath());
    bool fullSearch = args.size() < 2;
    std::vector<std::string> parsedPath = core::Utils::split(path, '/');
    for (FileData* file : core::pseudoFS()->getNRFS()->getRoot()->getFiles()) {
        if (file->name.find(args.at(0)) < LONG_LONG_MAX) {
            if (!returnable) {
                std::cout << "./" + file->name << '\n';
            } else {
                result += "./" + file->name + "\n";
            }
        }
    }

    for (FolderData* folder : core::pseudoFS()->getNRFS()->getRoot()->getFolders()) {
        if (fullSearch) {
            if (!returnable) {
                core::commands::CORE_COMMAND_searchFileHelper(*folder, args.at(0), {}, "./" + folder->name);
            } else {
                result += core::commands::CORE_COMMAND_searchFileHelper(*folder, args.at(0), {}, "./" + folder->name, true);
            }
        }
        else {
            parsedPath.erase(parsedPath.begin());
            for (FolderData* anotherFolder : core::pseudoFS()->getNRFS()->getRoot()->getFolders()) {
                if (parsedPath.at(0) == anotherFolder->name) {
                    if (!returnable) {
                        core::commands::CORE_COMMAND_searchFileHelper(*anotherFolder, args.at(0), parsedPath, "./" + anotherFolder->name);
                    } else {
                        result += core::commands::CORE_COMMAND_searchFileHelper(*anotherFolder, args.at(0), parsedPath, "./" + anotherFolder->name, true);
                    }
                }
            }
        }
    }
    return result;
}

std::string core::commands::CORE_COMMAND_searchFileHelper(const core::FolderData& curFolder, const std::string& what, std::vector<std::string> path, std::string stringPath, bool returnable) {
    bool fullSearch = path.empty();
    if (curFolder.files.empty() && curFolder.folders.empty()) {
        if (!returnable) {
            core::print("Error: ", core::PrintColors::red);
            return "";
        } else {
            return "Error";
        }
    }
    
    std::string output = "";
    for (core::FileData* file : curFolder.files) {
        if (file->name.find(what) < LONG_LONG_MAX) {
            if (!returnable) {
                std::cout << stringPath + "/" + file->name << '\n';
            } else {
                output += stringPath + "/" + file->name + "\n";
            }
        }
    }

    for (core::FolderData* folder : curFolder.folders) {
        if (fullSearch) {
            if (!returnable) {
                core::commands::CORE_COMMAND_searchFileHelper(*folder, what, {}, stringPath + "/" + folder->name);
            } else {
                output += core::commands::CORE_COMMAND_searchFileHelper(*folder, what, {}, stringPath + "/" + folder->name);
            }
        }
        else {
            path.erase(path.begin());
            for (FolderData* anotherFolder : core::pseudoFS()->getNRFS()->getRoot()->getFolders()) {
                if (path.at(0) == folder->name) {
                    if (!returnable) {
                        core::commands::CORE_COMMAND_searchFileHelper(*anotherFolder, what, path, stringPath + "./" + anotherFolder->name);
                    } else {
                        output += core::commands::CORE_COMMAND_searchFileHelper(*anotherFolder, what, path, stringPath + "./" + anotherFolder->name);
                    }
                }
            }
        }
    }
    return output;
}

std::string core::commands::CORE_COMMAND_printDiskSize(core::User*, bool returnable) {
    size_t curDisk = core::pseudoFS()->getCurDiskId();
    int filesSize = core::pseudoFS()->getNRFS()->getDisks().at(curDisk)->getFilesSize();
    int foldersSize = core::pseudoFS()->getNRFS()->getDisks().at(curDisk)->getFoldersSize();
    int diskSize = core::pseudoFS()->getNRFS()->getDisks().at(curDisk)->getDiskSize();
    if (!returnable) {
        core::print(DEFAULT_PRINT_COLOR,
            "Files: ", filesSize, "\n",
            "Folders: ", foldersSize, "\n",
            "All (files & folders): ", diskSize
        );
    } else {
        std::string result = "Files: " + std::to_string(filesSize) + "\n";
        result += "Folders: " + std::to_string(foldersSize) + "\n";
        result += "All (files & folders): " + std::to_string(diskSize) + "\n";
        return result;
    }
    return "";
}

std::string core::commands::CORE_COMMAND_whereIm(core::User* /*who*/, bool returnable) {
    std::vector<std::string> parsedPath = core::Utils::split(core::pseudoFS()->getCurrentPath(), '/');
    if (!returnable) {
        if (parsedPath.size() > 1)
            core::print("You're in the folder: " + parsedPath.back() + "\n", core::PrintColors::aqua);
        core::print("Full path: " + core::pseudoFS()->getCurrentPath() + "\n", core::PrintColors::aqua);
        core::print(core::PrintColors::aqua,
        "Disk: ",
        core::pseudoFS()->getNRFS()->getDisks().at(core::pseudoFS()->getCurDiskId())->getLetter(), "\n"
        );
    } else {
        std::string result = "You're in the folder: " + parsedPath.back() + "\n";
        result += "Full path: " + core::pseudoFS()->getCurrentPath() + "\n";
        size_t curDisk = core::pseudoFS()->getCurDiskId();
        char diskLetter = core::pseudoFS()->getNRFS()->getDisks().at(curDisk)->getLetter();
        result += "Disk: " + core::Utils::valueToString(diskLetter) + "\n";
        return result;
    }
    return "";
}


//   -------------- Users "Manager" ---------------

std::string core::commands::CORE_COMMAND_setPassword(core::User* who, const std::vector<std::string>& args, bool returnable) {
    if (core::userManager()->havePassword(who->getUsername())) {
        if (!who->truePassword(args[0])) {
            if (!returnable) {
                print("Wrong password!\n", core::PrintColors::red);
                return "";
            } else {
                return "Wrong password!";
            }
        }
    }
    who->editPassword((core::userManager()->havePassword(who->getUsername()) ? args[1] : args[0]));
    core::userManager()->saveUserData(who->getUsername());
    return "";
}

std::string core::commands::CORE_COMMAND_editDisplayName(core::User* who, const std::vector<std::string>& args, bool) {
    who->editDisplayName(args.at(0));
    core::userManager()->saveUserData(who->getUsername());
    return "";
}

std::string core::commands::CORE_COMMAND_createUser(core::User*, const std::vector<std::string>& args, bool returnable) {
    std::string message;
    if (!core::userManager()->userExist(args.at(0))) {
        if (!returnable) {
            core::print("Error: The user doesn't exist!\n", core::PrintColors::red);
            return "";
        } else {
            return "Error: The user doesn't exist!";
        }
    }
    else if (args.at(1).empty()) {
        if (!returnable) {
            core::print("COMMAND WARNING: A user will be created with \"User\" permissions\n", core::PrintColors::yellow);
        } else {
            message += "COMMAND WARNING: A user will be created with \"User\" permissions!\n";
        }
    }
    else {
        long long int pos = 1;
        for (const char& letter : args.at(0)) {
            if (ispunct(letter) || letter == ',' || letter == '.') {
                if (!returnable) {
                    core::print("COMMAND ERROR: Invalid character found (position " + std::to_string(pos) + ")\n", core::PrintColors::red);
                    return "";
                } else {
                    return "COMMAND ERROR: Invalid character found (position " + std::to_string(pos) + ")";
                }
            }
            pos++;
        }
        if (!core::Utils::stringIsNumbers(args.at(1))) {
            if (!returnable) {
                core::print("COMMAND ERROR\n", core::PrintColors::red);
                return "";
            } else {
                return "Command error: ???";
            }
        }
    }
    core::userManager()->addUser(args.at(0), static_cast<Permissions>((!args.at(1).empty() ? stoi(args.at(1)) : 0)));
    return message;
}

std::string core::commands::CORE_COMMAND_deleteUser(core::User*, const std::vector<std::string>& args, bool returnable) {
    if (!core::userManager()->userExist(args.at(0))) {
        if (!returnable) {
            core::print("COMMAND ERROR: The user doesn't exist!\n", core::PrintColors::red);
            return "";
        } else {
            return "COMMAND ERROR: The user doesn't exist!";
        }
    }
    else {
        long long int pos = 1;
        for (const char& letter : args.at(0)) {
            if (ispunct(letter) || letter == ',' || letter == '.') {
                if (!returnable) {
                    core::print("COMMAND ERROR: Invalid character found (position " + std::to_string(pos) + ")\n", core::PrintColors::red);
                    return "";
                } else {
                    return "COMMAND ERROR: Invalid character found (position " + std::to_string(pos) + ")";
                }
            }
            pos++;
        }
    }
    core::userManager()->deleteUser(args.at(0));
    return "";
};

std::string core::commands::CORE_COMMAND_renameUser(core::User*, const std::vector<std::string>& args, bool returnable) {
    if (!core::userManager()->userExist(args.at(0))) {
        if (!returnable) {
            core::print("COMMAND ERROR: The user doesn't exist!\n", core::PrintColors::red);
            return "";
        } else {
            return "COMMAND ERROR: The user doesn't exist!";
        }
    }
    else {
        long long int pos = 1;
        for (const char& letter : args.at(0)) {
            if (ispunct(letter) || letter == ',' || letter == '.') {
                if (!returnable) {
                    core::print("COMMAND ERROR: Invalid character found (position " + std::to_string(pos) + ")\n", core::PrintColors::red);
                    return "";
                } else {
                    return "COMMAND ERROR: Invalid character found (position " + std::to_string(pos) + ")";
                }
            }
            pos++;
        }
        pos = 1;
        for (const char& letter : args.at(1)) {
            if (ispunct(letter) || letter == ',' || letter == '.') {
                if (!returnable) {
                    core::print("COMMAND ERROR: Invalid character found (position " + std::to_string(pos) + ")\n", core::PrintColors::red);
                    return "";
                } else {
                    return "COMMAND ERROR: Invalid character found (position " + std::to_string(pos) + ")";
                }
            }
            pos++;
        }
    }
    core::userManager()->renameUser(args.at(0), args.at(1));
    return "";
}

std::string core::commands::CORE_COMMAND_setPermissionsUser(core::User*, const std::vector<std::string>& args, bool returnable) {
    if (!core::userManager()->userExist(args.at(0))) {
        if (!returnable) {
            core::print("COMMAND ERROR: The user doesn't exist!\n", core::PrintColors::red);
            return "";
        } else {
            return "COMMAND ERROR: The user doesn't exist!";
        }
    }
    else {
        long long int pos = 1;
        for (const char& letter : args.at(0)) {
            if (ispunct(letter) || letter == ',' || letter == '.') {
                if (!returnable) {
                    core::print("COMMAND ERROR: Invalid character found (position " + std::to_string(pos) + ")\n", core::PrintColors::red);
                    return "";
                } else {
                    return "COMMAND ERROR: Invalid character found (position " + std::to_string(pos) + ")";
                }
            }
            pos++;
        }
        if (!core::Utils::stringIsNumbers(args.at(1))) {
            if (!returnable) {
                core::print("COMMAND ERROR\n", core::PrintColors::red);
                return "";
            } else {
                return "Command error: ???";
            }
        }
    }
    core::userManager()->changePermissionsUser(args.at(0), static_cast<Permissions>(stoi(args[1])));
    return "";
}

std::string core::commands::CORE_COMMAND_addLocalVar(core::User* who, const std::vector<std::string>& args, bool returnable) {
    if (args.empty() || args.size() < 3) {
        if (!returnable)
            core::print("Error: Not enough arguments.", core::PrintColors::red);
        return "Error: Not enough arguments.";
    }

    if (args.at(1) != "func") {
        int code;
        size_t curDisk = core::pseudoFS()->getCurDiskId();
        who->addVar(
            args.at(0),
            (args.at(1) == "command" ? core::VariableType::COMMAND : core::VariableType::JS_CODE),
            (args.at(1) == "command" ? args.at(2) : core::pseudoFS()->getFileData(args.at(2), curDisk, code).content)
        );
        if (core::userManager()->userExist(who->getUsername()))
            core::userManager()->saveUserData(who->getUsername());
    }
    else {
        if (!returnable)
            core::print("Not ready.", core::PrintColors::yellow);
        return "Not ready.";
    }
    return "";
}

std::string core::commands::CORE_COMMAND_renameLocalVar(core::User* who, const std::vector<std::string>& args, bool returnable) {
    if (!returnable && (args.empty() || args.size() < 2)) {
        core::print("Error: Not enough arguments.", core::PrintColors::red);
        return "";
    } else if (returnable && (args.empty() || args.size() < 2)) {
        return "Error: Not enough arguments.";
    }

    who->renameVar(args.at(0), args.at(1));
    if (core::userManager()->userExist(who->getUsername()))
        core::userManager()->saveUserData(who->getUsername());
    return "";
}

//   -------------- Users ---------------

std::string core::commands::CORE_COMMAND_whoim(core::User* who, bool returnable) {
    if (!returnable) {
        std::cout << "Username: " << who->getUsername() << '\n';
        std::cout << "Display Name: " << who->getDisplayName() << '\n';
        std::cout << "Permissions: " << permissionsS(who->getPermissions()) << '\n';
    } else {
        std::string result = "Username: " + who->getUsername() + "\n";
        result += "Display Name: " + who->getDisplayName() + "\n";
        result += "Permissions: " + permissionsS(who->getPermissions()) + "\n";
        return result;
    }
    return "";
}

std::string core::commands::CORE_COMMAND_infoUser(core::User*, const std::vector<std::string>& args, bool returnable) {
    if (!core::userManager()->userExist(args.at(0))) {
        if (!returnable) {
            core::print("COMMAND ERROR: The user doesn't exist!\n", core::PrintColors::red);
            return "";
        } else {
            return "COMMAND ERROR: The user doesn't exist!";
        }
    }
    if (!returnable) {
        std::cout << "Username: " << args.at(0) << '\n';
        std::cout << "Display Name: " << core::userManager()->getUserMap()[args.at(0)] << '\n';
        std::cout << "Permissions: " << permissionsS(core::userManager()->getPermissionsMap()[args.at(0)]) << '\n';
    } else {
        std::string result = "Username: " + args.at(0) + "\n";
        result += "Display Name: " + core::userManager()->getUserMap()[args.at(0)] + "\n";
        result += "Permissions: " + permissionsS(core::userManager()->getPermissionsMap()[args.at(0)]) + "\n";
        return result;
    }
    return "";
}

std::string core::commands::CORE_COMMAND_allInfoUsers(core::User*, bool returnable) {
    if (!returnable) {
        std::cout << "  - [ All Users Info ] -  " << '\n';
        for (auto& user : core::userManager()->getUserMap()) {
            std::cout << " - " + user.first + " | " + user.second << '\n';
            std::cout << "Language: " << core::userManager()->getLanguageMap()[user.first] << '\n';
            std::cout << "Permissions: " << permissionsS(core::userManager()->getPermissionsMap()[user.first]) << "\n\n";
        }
    } else {
        std::string result = "  - [ All Users Info ] -  \n";
        for (auto& user : core::userManager()->getUserMap()) {
            result += " - " + user.first + " | " + user.second + "\n";
            result += "Language: " + core::userManager()->getLanguageMap()[user.first] + "\n";
            result += "Permissions: " + permissionsS(core::userManager()->getPermissionsMap()[user.first]) + "\n";
        }
        return result;
    }
    return "";
}

std::string core::commands::CORE_COMMAND_logout(core::User*, bool returnable) {
    if (!returnable) {
        std::string choice;
        print("Are you sure you want to log out of your current user account? (Y/N): ", core::PrintColors::yellow);
        std::cin >> choice;
        while (true) {
            if (choice == "Y") { core::userManager()->userLogout(); break; }
            else if (choice == "N") { break; }
            else { 
                print("Error. Are you sure you want to log out of the current user account? (Y/N): ", core::PrintColors::red);
                std::cin >> choice;
            }
        }
    }
    return "";
}

//   -------------- Other ---------------

std::string core::commands::CORE_COMMAND_allSystemVars(core::User*, bool returnable) {
    if (!returnable) {
        for (auto var : core::systemVariablesManager()->getAllVars()) {
            core::print(var, core::PrintColors::light_yellow);
            core::print();
        }
    } else {
        std::string result;
        for (auto var : core::systemVariablesManager()->getAllVars()) {
            result += core::Utils::valueToString(var) + "\n";
        }
        return result;
    }
    return "";
}

std::string core::commands::CORE_COMMAND_allLocalVars(core::User* who, bool returnable) {
    if (!returnable) {
        for (auto var : who->getAllVars()) {
            core::print(var, core::PrintColors::light_aqua);
            core::print();
        }
    } else {
        std::string result;
        for (auto var : who->getAllVars()) {
            result += core::Utils::valueToString(var) + "\n";
        }
        return result;
    }
    return "";
}

std::string core::checkUserPermissionsForCommand(core::User* who) {
    if (!who)
        return "User is not initialised! (who == NULL)";
    else if (who->getPermissions() == core::Permissions::ghost)
        return "Insufficient permissions for this operation: the current user permission level is lower than normal";
    return "";
}

std::string core::checkUserPermissionsForPFSCommand(core::User* who, const core::FileData& target) {
    if (target.link)
        return core::checkUserPermissionsForPFSCommand(who, *(target.link));
    else if (target.system && !target.owner && who->getPermissions() < core::Permissions::admin)
        return "Insufficient rights to operate on a system file: administrator rights required";
    else if (target.owner && target.owner != who && who->getPermissions() < core::Permissions::admin)
        return "Insufficient rights to operate on this file: administrator rights required";
    return "";
}

std::string core::checkUserPermissionsForPFSCommand(core::User* who, const core::FolderData& target) {
    if (target.link)
        return core::checkUserPermissionsForPFSCommand(who, *(target.link));
    else if (target.system && !target.owner && who->getPermissions() < core::Permissions::admin)
        return "Insufficient rights to work with the system folder: administrator rights required";
    else if (target.owner && target.owner != who && who->getPermissions() < core::Permissions::admin)
        return "Insufficient rights to operate on this folder: administrator rights required";
    return "";
}

std::string core::checkFileCodeForPFSCommand(int code) {
    switch (code)
    {
    case core::PseudoFSCodes::NOT_FOUND:            return "File not found!";
    case core::PseudoFSCodes::ALREADY_EXISTS:       return "This file already exists!";
    case core::PseudoFSCodes::PATH_IS_EMPTY:        return "The file operation was not completed - PATH IS EMPTY!";
    case core::PseudoFSCodes::BAD_PATH:             return "Incorrect folder path. Check if the path is correct.";
    case core::PseudoFSCodes::OK:                   return "";
    default:                                        return "Unknown Error";
    }
    return "";
}

std::string core::checkFolderCodeForPFSCommand(int code) {
    switch (code)
    {
    case core::PseudoFSCodes::NOT_FOUND:            return "Folder not found!";
    case core::PseudoFSCodes::ALREADY_EXISTS:       return "This folder already exists!";
    case core::PseudoFSCodes::PATH_IS_EMPTY:        return "The operation with the folder was not completed - PATH IS EMPTY!";
    case core::PseudoFSCodes::BAD_PATH:             return "Incorrect folder path. Check if the path is correct.";
    case core::PseudoFSCodes::OK:                   return "";
    default:                                        return "Unknown Error";
    }
    return "";
}

void core::fixCreateSamePFSObject(const std::string& path, size_t curDisk, const core::FileData& target, int& code) {
    int temp = 2;
    std::string fileName = core::pseudoFS()->getFileFromPath(path);
    std::string partPath = path.substr(0, path.length() - fileName.length());
    while (code == core::PseudoFSCodes::ALREADY_EXISTS) {
        std::string newFileName =
            fileName.substr(
                0,
                fileName.find_last_of(".")
            ) + "(" + std::to_string(temp++) + ")" +
            fileName.substr(
                fileName.find_last_of("."),
               fileName.length()
            );
        core::FileData* targetPtr = new core::FileData(target);
        targetPtr->name = newFileName;
        code = core::pseudoFS()->createFile(path + newFileName, curDisk, targetPtr);
    }
    core::pseudoFS()->newLastPFSObjectID();
}

void core::fixCreateSamePFSObject(const std::string& path, size_t curDisk, const core::FolderData& target, int& code) {
    int temp = 2;
    std::string folderName = core::pseudoFS()->getFolderFromPath(path);
    std::string partPath = path.substr(0, path.length() - folderName.length());
    while (code == core::PseudoFSCodes::ALREADY_EXISTS) {
        std::string newFolderName =
            folderName.substr(
                0,
                folderName.find_last_of(".")
            ) + "(" + std::to_string(temp++) + ")" +
            folderName.substr(
                folderName.find_last_of("."),
               folderName.length()
            );
        core::FolderData* targetPtr = new core::FolderData(target);
        targetPtr->name = newFolderName;
        code = core::pseudoFS()->createFolder(path + newFolderName, curDisk, targetPtr);
    }
}