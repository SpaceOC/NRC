#include <string>
#include <vector>
#include "Core/base/print.h"
#include "Core/base/filesystem/pseudo_fs.h"

void addFolderAndFiles() {
    core::PseudoFS FS;
    FS.createFolder("./yourPseudoOS");
    FS.createFile("./yourPseudoOS/info.txt");
    FS.setFileAtt("./yourPseudoOS/info.txt", "content", "Hello :D");
}