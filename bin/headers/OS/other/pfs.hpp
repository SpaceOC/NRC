#include <string>
#include <vector>
#include "Core/base/print.h"
#include "Core/base/filesystem/pseudo_fs.h"

void addFolderAndFiles() {
    core::pseudoFS()->createFolder("./yourPseudoOS", 0);
    core::pseudoFS()->createFile("./yourPseudoOS/info.txt", 0);
    core::pseudoFS()->setFileAtt("./yourPseudoOS/info.txt", 0, "content", "Hello");
}