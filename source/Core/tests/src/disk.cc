#include <gtest/gtest.h>
#include "Core/users/user.h"
#include "Core/filesystem/nrfs.h"
#include "Core/filesystem/pseudo_fs.h"

using namespace core;

TEST(PFS, Folders) {
	// Expect two strings not to be equal.
	//EXPECT_STRNE("hello", "world");
	// Expect equality.
	//EXPECT_EQ(7 * 6, 42);
	PseudoFS pseudoFilesystem;
	pseudoFilesystem.init();
	//pseudoFilesystem.postInit();
	pseudoFilesystem.createFolder("./first", 0);
	int code = 0;
	auto folder = pseudoFilesystem.getFolderData("./first", 0, code);
	EXPECT_EQ(code, PseudoFSCodes::OK);
	EXPECT_EQ(folder.name, "first");

	code = pseudoFilesystem.renameFolder("./first", 0, "second");
	EXPECT_EQ(code, PseudoFSCodes::OK);
	folder = pseudoFilesystem.getFolderData("./second", 0, code);
	EXPECT_EQ(folder.name, "second");

	code = pseudoFilesystem.createFolder("./second/second_layer_folder", 0);
	EXPECT_EQ(code, PseudoFSCodes::OK);
	folder = pseudoFilesystem.getFolderData("./second/second_layer_folder", 0, code);
	EXPECT_EQ(folder.name, "second_layer_folder");

	pseudoFilesystem.createFolder("./hii", 0);
	code = pseudoFilesystem.deleteFolder("./hii", 0);
	EXPECT_EQ(code, PseudoFSCodes::OK);

	pseudoFilesystem.createFolder("./second/f", 0);
	code = pseudoFilesystem.deleteFolder("./second/f", 0);
	EXPECT_EQ(code, PseudoFSCodes::OK);

	code = pseudoFilesystem.createFolder("./second/second_layer_folder/f", 0);
	EXPECT_EQ(code, PseudoFSCodes::OK);
	folder = pseudoFilesystem.getFolderData("./second/second_layer_folder/f", 0, code);
	EXPECT_EQ(folder.name, "f");
	code = pseudoFilesystem.deleteFolder("./second/second_layer_folder/f", 0);
	EXPECT_EQ(code, PseudoFSCodes::OK);

	pseudoFilesystem.printAll(true, 0);
}

TEST(PFS, Files) {
	// Expect two strings not to be equal.
	//EXPECT_STRNE("hello", "world");
	// Expect equality.
	//EXPECT_EQ(7 * 6, 42);
	PseudoFS pseudoFilesystem;
	pseudoFilesystem.init();
	pseudoFilesystem.createFile("./h.txt", 0);
	int code = 0;
	auto file = pseudoFilesystem.getFileData("./h.txt", 0, code);
	EXPECT_EQ(code, PseudoFSCodes::OK);
	EXPECT_EQ(file.name, "h.txt");
}

TEST(PFS, NewDisk) {
	PseudoFS pseudoFilesystem;
	pseudoFilesystem.init();
	pseudoFilesystem.getNRFS()->createDisk('D');
	EXPECT_EQ(pseudoFilesystem.getNRFS()->getDisks().size(), 2);

	int code = 0;

	code = pseudoFilesystem.createFile("./hello_from_diskC.txt", 0);
	EXPECT_EQ(code, PseudoFSCodes::OK);
	auto file = pseudoFilesystem.getFileData("./hello_from_diskC.txt", 0, code);
	EXPECT_EQ(file.name, "hello_from_diskC.txt");

	pseudoFilesystem.getFileData("./hello_from_diskD.txt", 0, code);
	EXPECT_TRUE(code == PseudoFSCodes::NOT_FOUND);

	pseudoFilesystem.createFile("./hello_from_diskD.txt", 1);
	file = pseudoFilesystem.getFileData("./hello_from_diskD.txt", 1, code);
	EXPECT_EQ(code, PseudoFSCodes::OK);
	EXPECT_EQ(file.name, "hello_from_diskD.txt");
}