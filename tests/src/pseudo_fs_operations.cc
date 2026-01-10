#include <gtest/gtest.h>
#include "Core/users/user.h"
#include "Core/filesystem/nrfs.h"
#include "Core/filesystem/pseudo_fs.h"

using namespace core;

TEST(PFS_Operations, Folders) {
	PseudoFS pseudoFilesystem;
	pseudoFilesystem.init();
	//pseudoFilesystem.postInit();
	pseudoFilesystem.createFolder("./test", 0);
	int code = 0;
	auto folder = pseudoFilesystem.getFolderData("./test", 0, code);
	EXPECT_EQ(code, PseudoFSCodes::OK);
	EXPECT_EQ(folder.name, "test");

    pseudoFilesystem.createFolder("./test2", 0);
    pseudoFilesystem.createFolder("./test3", 0);
    pseudoFilesystem.createFolder("./test4", 0);

	code = pseudoFilesystem.renameFolder("./test", 0, "coolFolder");
    EXPECT_EQ(code, PseudoFSCodes::OK);
    folder = pseudoFilesystem.getFolderData("./coolFolder", 0, code);
    EXPECT_EQ(code, PseudoFSCodes::OK);
    EXPECT_EQ(folder.name, "coolFolder");

	code = pseudoFilesystem.setFolderAtt("./coolFolder", 0, "system", true);
    EXPECT_EQ(code, PseudoFSCodes::OK);
    folder = pseudoFilesystem.getFolderData("./coolFolder", 0, code);
    EXPECT_EQ(code, PseudoFSCodes::OK);
    EXPECT_EQ(folder.system, true);

    code = pseudoFilesystem.moveFolder("./coolFolder", 0, "./test2/", 0);
    EXPECT_EQ(code, PseudoFSCodes::OK);
    folder = pseudoFilesystem.getFolderData("./coolFolder", 0, code);
    EXPECT_EQ(code, PseudoFSCodes::NOT_FOUND);
    folder = pseudoFilesystem.getFolderData("./test2/coolFolder", 0, code);
    EXPECT_EQ(code, PseudoFSCodes::OK);

    code = pseudoFilesystem.moveFolder("./test4", 0, "./test2/coolFolder/", 0);
    EXPECT_EQ(code, PseudoFSCodes::OK);
    folder = pseudoFilesystem.getFolderData("./test4", 0, code);
    EXPECT_EQ(code, PseudoFSCodes::NOT_FOUND);
    folder = pseudoFilesystem.getFolderData("./test2/coolFolder/test4", 0, code);
    EXPECT_EQ(code, PseudoFSCodes::OK);

    code = pseudoFilesystem.deleteFolder("./test3", 0);
    EXPECT_EQ(code, PseudoFSCodes::OK);
    folder = pseudoFilesystem.getFolderData("./test3", 0, code);
    EXPECT_EQ(code, PseudoFSCodes::NOT_FOUND);

	pseudoFilesystem.createFolder("./test2/coolFolder/test2025", 0);
	folder = pseudoFilesystem.getFolderData("./test2/coolFolder/test2025", 0, code);
	EXPECT_EQ(code, PseudoFSCodes::OK);
	EXPECT_EQ(folder.name, "test2025");

    code = pseudoFilesystem.deleteFolder("./test2/coolFolder/test4", 0);
    EXPECT_EQ(code, PseudoFSCodes::OK);
    folder = pseudoFilesystem.getFolderData("./test2/coolFolder/test4", 0, code);
    EXPECT_EQ(code, PseudoFSCodes::NOT_FOUND);

    code = pseudoFilesystem.deleteFolder("./test2/coolFolder/test2025", 0);
    EXPECT_EQ(code, PseudoFSCodes::OK);
    folder = pseudoFilesystem.getFolderData("./test2/coolFolder/test2025", 0, code);
    EXPECT_EQ(code, PseudoFSCodes::NOT_FOUND);

    code = pseudoFilesystem.deleteFolder("./test2/coolFolder", 0);
    EXPECT_EQ(code, PseudoFSCodes::OK);
    folder = pseudoFilesystem.getFolderData("./test2/coolFolder", 0, code);
    EXPECT_EQ(code, PseudoFSCodes::NOT_FOUND);

    code = pseudoFilesystem.deleteFolder("./test2", 0);
    EXPECT_EQ(code, PseudoFSCodes::OK);
    folder = pseudoFilesystem.getFolderData("./test2", 0, code);
    EXPECT_EQ(code, PseudoFSCodes::NOT_FOUND);

	pseudoFilesystem.printAll(true, 0);
}

TEST(PFS_Operations, File) {
	PseudoFS pseudoFilesystem;
	pseudoFilesystem.init();
	//pseudoFilesystem.postInit();
	pseudoFilesystem.createFile("./test.txt", 0);
	int code = 0;
	auto file = pseudoFilesystem.getFileData("./test.txt", 0, code);
	EXPECT_EQ(code, PseudoFSCodes::OK);
	EXPECT_EQ(file.name, "test.txt");

	code = pseudoFilesystem.renameFile("./test.txt", 0, "test2.txt");
    EXPECT_EQ(code, PseudoFSCodes::OK);
    file = pseudoFilesystem.getFileData("./test2.txt", 0, code);
    EXPECT_EQ(code, PseudoFSCodes::OK);
    EXPECT_EQ(file.name, "test2.txt");

	code = pseudoFilesystem.setFileAtt("./test2.txt", 0, "system", true);
    EXPECT_EQ(code, PseudoFSCodes::OK);
    file = pseudoFilesystem.getFileData("./test2.txt", 0, code);
    EXPECT_EQ(code, PseudoFSCodes::OK);
    EXPECT_EQ(file.system, true);

    pseudoFilesystem.createFolder("./test2", 0);
    pseudoFilesystem.createFolder("./test3", 0);
    pseudoFilesystem.createFolder("./test4", 0);

    code = pseudoFilesystem.moveFile("./test2.txt", 0, "./test2", 0);
    EXPECT_EQ(code, PseudoFSCodes::OK);
    file = pseudoFilesystem.getFileData("./test2.txt", 0, code);
    EXPECT_EQ(code, PseudoFSCodes::NOT_FOUND);

    pseudoFilesystem.moveFolder("./test3", 0, "./test2", 0);
    pseudoFilesystem.moveFolder("./test4", 0, "./test2/test3", 0);

    pseudoFilesystem.createFile("./test2/test2025.txt", 0);
	file = pseudoFilesystem.getFileData("./test2/test2025.txt", 0, code);
	EXPECT_EQ(code, PseudoFSCodes::OK);
	EXPECT_EQ(file.name, "test2025.txt");

    pseudoFilesystem.createFile("./test2/test3/test2026.txt", 0);
	file = pseudoFilesystem.getFileData("./test2/test3/test2026.txt", 0, code);
	EXPECT_EQ(code, PseudoFSCodes::OK);
	EXPECT_EQ(file.name, "test2026.txt");

    code = pseudoFilesystem.moveFile("./test2/test2.txt", 0, "./test2/test3/test4", 0);
    EXPECT_EQ(code, PseudoFSCodes::OK);
    file = pseudoFilesystem.getFileData("./test2/test2.txt", 0, code);
    EXPECT_EQ(code, PseudoFSCodes::NOT_FOUND);

    code = pseudoFilesystem.deleteFile("/test2/test3/test2026.txt", 0);
    EXPECT_EQ(code, PseudoFSCodes::OK);
	file = pseudoFilesystem.getFileData("./test2/test3/test2026.txt", 0, code);
	EXPECT_EQ(code, PseudoFSCodes::NOT_FOUND);

    code = pseudoFilesystem.deleteFile("/test2/test2025.txt", 0);
    EXPECT_EQ(code, PseudoFSCodes::OK);
	file = pseudoFilesystem.getFileData("./test2/test2025.txt", 0, code);
	EXPECT_EQ(code, PseudoFSCodes::NOT_FOUND);

    code = pseudoFilesystem.deleteFile("./test2/test3/test4/test2.txt", 0);
    EXPECT_EQ(code, PseudoFSCodes::OK);
	file = pseudoFilesystem.getFileData("./test2/test3/test4/test2.txt", 0, code);
	EXPECT_EQ(code, PseudoFSCodes::NOT_FOUND);

	pseudoFilesystem.printAll(true, 0);
}

TEST(PFS_Operations, ChangePath) {
	PseudoFS pseudoFilesystem;
	pseudoFilesystem.init();

	pseudoFilesystem.createFolder("./test", 0);
	int code = 0;
	auto folder = pseudoFilesystem.getFolderData("./test", 0, code);
	EXPECT_EQ(code, PseudoFSCodes::OK);
	EXPECT_EQ(folder.name, "test");

    pseudoFilesystem.createFolder("./test/test2", 0);
    pseudoFilesystem.createFolder("./test/test2/test3", 0);
    pseudoFilesystem.createFolder("./test4", 0);
    
    code = pseudoFilesystem.changePath("./test/test2/test3", 0);
    EXPECT_EQ(code, PseudoFSCodes::OK);
    EXPECT_EQ(pseudoFilesystem.currentPath(), "./test/test2/test3");

    code = pseudoFilesystem.changePath("./test/test2", 0);
    EXPECT_EQ(code, PseudoFSCodes::OK);
    EXPECT_EQ(pseudoFilesystem.currentPath(), "./test/test2");

    code = pseudoFilesystem.changePath("./test", 0);
    EXPECT_EQ(code, PseudoFSCodes::OK);
    EXPECT_EQ(pseudoFilesystem.currentPath(), "./test");

    code = pseudoFilesystem.changePath("./", 0);
    EXPECT_EQ(code, PseudoFSCodes::OK);
    EXPECT_EQ(pseudoFilesystem.currentPath(), "./");
}

TEST(PFS_Operations, ChangeDirectory) {
	PseudoFS pseudoFilesystem;
	pseudoFilesystem.init();

	pseudoFilesystem.createFolder("./test", 0);
	int code = 0;
	auto folder = pseudoFilesystem.getFolderData("./test", 0, code);
	EXPECT_EQ(code, PseudoFSCodes::OK);
	EXPECT_EQ(folder.name, "test");

    pseudoFilesystem.createFolder("./test/test2", 0);
    pseudoFilesystem.createFolder("./test/test2/test3", 0);
    pseudoFilesystem.createFolder("./test4", 0);
    
    code = pseudoFilesystem.changeDirectory("test/test2/test3", 0);
    EXPECT_EQ(code, PseudoFSCodes::OK);
    EXPECT_EQ(pseudoFilesystem.currentPath(), "./test/test2/test3/");

    code = pseudoFilesystem.changeDirectory("..", 0);
    EXPECT_EQ(code, PseudoFSCodes::OK);
    EXPECT_EQ(pseudoFilesystem.currentPath(), "./test/test2/");

    code = pseudoFilesystem.changeDirectory("test3", 0);
    EXPECT_EQ(code, PseudoFSCodes::OK);
    EXPECT_EQ(pseudoFilesystem.currentPath(), "./test/test2/test3/");

    code = pseudoFilesystem.changeDirectory("..", 0);
    EXPECT_EQ(code, PseudoFSCodes::OK);
    EXPECT_EQ(pseudoFilesystem.currentPath(), "./test/test2/");

    code = pseudoFilesystem.changeDirectory("..", 0);
    EXPECT_EQ(code, PseudoFSCodes::OK);
    EXPECT_EQ(pseudoFilesystem.currentPath(), "./test/");

    code = pseudoFilesystem.changeDirectory("test2", 0);
    EXPECT_EQ(code, PseudoFSCodes::OK);
    EXPECT_EQ(pseudoFilesystem.currentPath(), "./test/test2/");

    code = pseudoFilesystem.changeDirectory("..", 0);
    EXPECT_EQ(code, PseudoFSCodes::OK);
    EXPECT_EQ(pseudoFilesystem.currentPath(), "./test/");

    code = pseudoFilesystem.changeDirectory("..", 0);
    EXPECT_EQ(code, PseudoFSCodes::OK);
    EXPECT_EQ(pseudoFilesystem.currentPath(), "./");

    code = pseudoFilesystem.changeDirectory("..", 0);
    EXPECT_EQ(code, PseudoFSCodes::SMALL_SIZE);
    EXPECT_EQ(pseudoFilesystem.currentPath(), "./");
}