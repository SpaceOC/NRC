#include <string>
#include <vector>
#include "Core/base/print.h"
#include "Core/base/filesystem/pseudo_fs.h"

void addFolderAndFiles() {
    core::PseudoFS::createFolder("./yourPseudoOS");
    core::PseudoFS::createFile("./yourPseudoOS/info.txt");
    core::PseudoFS::setFileAtt("./yourPseudoOS/info.txt", "content", "Hello :D");
}