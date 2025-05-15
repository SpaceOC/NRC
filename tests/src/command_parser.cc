#include <gtest/gtest.h>
#include "Core/command/handler_commands.h"
#include "Core/command/command_parser.h"
#include "Core/command/command_structs.h"
#include "Core/users/user.h"
#include "Core/filesystem/nrfs.h"
#include "Core/filesystem/pseudo_fs.h"

using namespace core;

TEST(CommandParser, NoSpacesInCommand) {
	CommandObject obj;
	CommandParser parser;

	auto parsed = parser.parse("help");
	ASSERT_EQ(parsed.size(), 1);

	obj = parsed[0];
	EXPECT_EQ(obj.name, "help");
	EXPECT_EQ(obj.args.size(), 0);
}

TEST(CommandParser, NameInQuotes1) {
	CommandObject obj;
	CommandParser parser;

	auto parsed = parser.parse("\"help\"");
	ASSERT_EQ(parsed.size(), 1);

	obj = parsed[0];
	EXPECT_EQ(obj.name, "help");
	EXPECT_EQ(obj.args.size(), 0);
}

TEST(CommandParser, NameInQuotes2) {
	CommandObject obj;
	CommandParser parser;

	auto parsed = parser.parse("\"one two  three   |\"");
	ASSERT_EQ(parsed.size(), 1);

	obj = parsed[0];
	EXPECT_EQ(obj.name, "one two  three   |");
	EXPECT_EQ(obj.args.size(), 0);
}

TEST(CommandParser, NameInQuotes3) {
	CommandObject obj;
	CommandParser parser;

	auto parsed = parser.parse("\"hi, \\\"! | My name: \\\" a  b  c   d\\\"\"");
	ASSERT_EQ(parsed.size(), 1);

	obj = parsed[0];
	EXPECT_EQ(obj.name, "hi, \\\"! | My name: \\\" a  b  c   d\\\"");
	EXPECT_EQ(obj.args.size(), 0);
}

TEST(CommandParser, WithOneSingleSpaceInCommand) {
	CommandObject obj;
	CommandParser parser;

	auto parsed = parser.parse("help whoim");
	ASSERT_EQ(parsed.size(), 1);

	obj = parsed[0];
	EXPECT_EQ(obj.name, "help");
	EXPECT_EQ(obj.args.size(), 1);
}

TEST(CommandParser, WithReturnable1) {
	CommandObject obj;
	CommandParser parser;

	auto parsed = parser.parse("help whoim -> hello.txt");
	ASSERT_EQ(parsed.size(), 1);

	obj = parsed[0];
	EXPECT_EQ(obj.name, "help");
	EXPECT_EQ(obj.args.size(), 1);
	EXPECT_EQ(obj.returnable, true);
	EXPECT_EQ(obj.whereOutput, "hello.txt");
}

TEST(CommandParser, WithReturnable2) {
	CommandObject obj;
	CommandParser parser;

	auto parsed = parser.parse("help whoim -> \"./users/ New Test    User/output.bin\"");
	ASSERT_EQ(parsed.size(), 1);

	obj = parsed[0];
	EXPECT_EQ(obj.name, "help");
	EXPECT_EQ(obj.args.size(), 1);
	EXPECT_EQ(obj.returnable, true);
	EXPECT_EQ(obj.whereOutput, "./users/ New Test    User/output.bin");
}

TEST(CommandParser, WithSpacesInCommand) {
	CommandObject obj;
	CommandParser parser;

	auto parsed = parser.parse("create_file ./da net.txt SpaceOC");
	ASSERT_EQ(parsed.size(), 1);

	obj = parsed[0];
	EXPECT_EQ(obj.name, "create_file");
	EXPECT_EQ(obj.args.size(), 3);
}

TEST(CommandParser, WithSingleQuote) {
	CommandObject obj;
	CommandParser parser;

	auto parsed = parser.parse("create_file \"./yo ur/ path/  to/fol d er\"");
	ASSERT_EQ(parsed.size(), 1);

	obj = parsed[0];
	EXPECT_EQ(obj.name, "create_file");
	EXPECT_EQ(obj.args.size(), 1);

	if (obj.args.size() == 1) {
		std::string arg = obj.args[0];
		EXPECT_EQ(arg, "./yo ur/ path/  to/fol d er");
	}
}

TEST(CommandParser, WithQuotes) {
	CommandObject obj;
	CommandParser parser;

	auto parsed = parser.parse("echo \"./yo ur/ path/  to/fol d er\" \"Hello NRC! Hello gtest! Hello hello   ---    h e l l o  hi :)\" \"another quote yea...\"");
	ASSERT_EQ(parsed.size(), 1);

	obj = parsed[0];
	EXPECT_EQ(obj.name, "echo");
	EXPECT_EQ(obj.args.size(), 3);

	if (obj.args.size() == 3) {
		EXPECT_EQ(obj.args[0], "./yo ur/ path/  to/fol d er");
		EXPECT_EQ(obj.args[1], "Hello NRC! Hello gtest! Hello hello   ---    h e l l o  hi :)");
		EXPECT_EQ(obj.args[2], "another quote yea...");
	}
}

TEST(CommandParser, WithSingleQuoteAndSpace) {
	CommandObject obj;
	CommandParser parser;

	auto parsed = parser.parse("create_file \"./yo ur/ path/  to/fol d er\" hello_world.txt");
	ASSERT_EQ(parsed.size(), 1);

	obj = parsed[0];
	EXPECT_EQ(obj.name, "create_file");
	EXPECT_EQ(obj.args.size(), 2);

	if (obj.args.size() == 2) {
		EXPECT_EQ(obj.args[0], "./yo ur/ path/  to/fol d er");
		EXPECT_EQ(obj.args[1], "hello_world.txt");
	}
}

TEST(CommandParser, WithQuoteAndSpaces1) {
	CommandObject obj;
	CommandParser parser;

	auto parsed = parser.parse("create \"./yo ur/ path/  to/fol d er\" hello_world.txt \"Hello NRC! Hello gtest! Hello hello   ---    h e l l o  hi :)\" true true \"OS / Users\"");
	ASSERT_EQ(parsed.size(), 1);

	obj = parsed[0];
	EXPECT_EQ(obj.name, "create");
	EXPECT_EQ(obj.args.size(), 6);

	if (obj.args.size() == 6) {
		EXPECT_EQ(obj.args[0], "./yo ur/ path/  to/fol d er");
		EXPECT_EQ(obj.args[1], "hello_world.txt");
		EXPECT_EQ(obj.args[2], "Hello NRC! Hello gtest! Hello hello   ---    h e l l o  hi :)");
		EXPECT_EQ(obj.args[3], "true");
		EXPECT_EQ(obj.args[4], "true");
		EXPECT_EQ(obj.args[5], "OS / Users");
	}
}

TEST(CommandParser, WithQuoteAndSpaces2) {
	CommandObject obj;
	CommandParser parser;

	auto parsed = parser.parse("create \"./yo ur/ path/  to/fol d er\" hello_world.txt \"Hello \\\"NRC\\\"! Hello gtest! Hello hello   ---    \\\"h\\\" \\\"e\\\" \\\"l\\\" \\\"l\\\" \\\"o\\\"  hi :)\" true true \"\\\"OS\\\" / \\\"Users\\\"\"");
	ASSERT_EQ(parsed.size(), 1);

	obj = parsed[0];
	EXPECT_EQ(obj.name, "create");
	EXPECT_EQ(obj.args.size(), 6);

	if (obj.args.size() == 6) {
		EXPECT_EQ(obj.args[0], "./yo ur/ path/  to/fol d er");
		EXPECT_EQ(obj.args[1], "hello_world.txt");
		EXPECT_EQ(obj.args[2], "Hello \\\"NRC\\\"! Hello gtest! Hello hello   ---    \\\"h\\\" \\\"e\\\" \\\"l\\\" \\\"l\\\" \\\"o\\\"  hi :)");
		EXPECT_EQ(obj.args[3], "true");
		EXPECT_EQ(obj.args[4], "true");
		EXPECT_EQ(obj.args[5], "\\\"OS\\\" / \\\"Users\\\"");
	}
}

TEST(CommandParser, PreFinalTest) {
	CommandObject obj;
	CommandParser parser;

	auto parsed = parser.parse("create \"./yo ur/ path/  to/fol d er\" hello_world.txt &/ help whoim &/ delete_user \"Guest g  |\" &/ another_command \"./yo ur/ path/  to/fol d er\" hello_world.txt \"Hello \\\"NRC\\\"! Hello gtest! Hello hello   ---    \\\"h\\\" \\\"e\\\" \\\"l\\\" \\\"l\\\" \\\"o\\\"  hi :)\" true true \"\\\"OS\\\" / \\\"Users\\\"\"");
	ASSERT_EQ(parsed.size(), 4);

	obj = parsed[0];
	EXPECT_EQ(obj.name, "create");
	EXPECT_EQ(obj.args.size(), 2);

	if (obj.args.size() == 2) {
		EXPECT_EQ(obj.args[0], "./yo ur/ path/  to/fol d er");
		EXPECT_EQ(obj.args[1], "hello_world.txt");
	}

	obj = parsed[1];
	EXPECT_EQ(obj.name, "help");
	EXPECT_EQ(obj.args[0], "whoim");

	obj = parsed[2];
	EXPECT_EQ(obj.name, "delete_user");
	EXPECT_EQ(obj.args[0], "Guest g  |");

	obj = parsed[3];
	EXPECT_EQ(obj.name, "another_command");
	EXPECT_EQ(obj.args.size(), 6);

	if (obj.args.size() == 6) {
		EXPECT_EQ(obj.args[0], "./yo ur/ path/  to/fol d er");
		EXPECT_EQ(obj.args[1], "hello_world.txt");
		EXPECT_EQ(obj.args[2], "Hello \\\"NRC\\\"! Hello gtest! Hello hello   ---    \\\"h\\\" \\\"e\\\" \\\"l\\\" \\\"l\\\" \\\"o\\\"  hi :)");
		EXPECT_EQ(obj.args[3], "true");
		EXPECT_EQ(obj.args[4], "true");
		EXPECT_EQ(obj.args[5], "\\\"OS\\\" / \\\"Users\\\"");
	}
}

TEST(CommandParser, FinalTest) {
	CommandObject obj;
	CommandParser parser;

	auto parsed = parser.parse("create \"./yo ur/ path/  to/fol d er\" hello_world.txt -> output1.txt &/ help whoim -> \"very cool text.exe\" &/ delete_user \"Guest g  |\" &/ another_command \"./yo ur/ path/  to/fol d er\" hello_world.txt \"Hello \\\"NRC\\\"! Hello gtest! Hello hello   ---    \\\"h\\\" \\\"e\\\" \\\"l\\\" \\\"l\\\" \\\"o\\\"  hi :)\" true true \"\\\"OS\\\" / \\\"Users\\\"\" -> \"./ w h y /  u /reading/this/a.bin\"");
	ASSERT_EQ(parsed.size(), 4);

	obj = parsed[0];
	EXPECT_EQ(obj.name, "create");
	EXPECT_EQ(obj.args.size(), 2);

	if (obj.args.size() == 2) {
		EXPECT_EQ(obj.args[0], "./yo ur/ path/  to/fol d er");
		EXPECT_EQ(obj.args[1], "hello_world.txt");
	}

	EXPECT_EQ(obj.returnable, true);
	EXPECT_EQ(obj.whereOutput, "output1.txt");

	obj = parsed[1];
	EXPECT_EQ(obj.name, "help");
	EXPECT_EQ(obj.args[0], "whoim");
	EXPECT_EQ(obj.returnable, true);
	EXPECT_EQ(obj.whereOutput, "very cool text.exe");

	obj = parsed[2];
	EXPECT_EQ(obj.name, "delete_user");
	EXPECT_EQ(obj.args[0], "Guest g  |");

	obj = parsed[3];
	EXPECT_EQ(obj.name, "another_command");
	EXPECT_EQ(obj.args.size(), 6);

	if (obj.args.size() == 6) {
		EXPECT_EQ(obj.args[0], "./yo ur/ path/  to/fol d er");
		EXPECT_EQ(obj.args[1], "hello_world.txt");
		EXPECT_EQ(obj.args[2], "Hello \\\"NRC\\\"! Hello gtest! Hello hello   ---    \\\"h\\\" \\\"e\\\" \\\"l\\\" \\\"l\\\" \\\"o\\\"  hi :)");
		EXPECT_EQ(obj.args[3], "true");
		EXPECT_EQ(obj.args[4], "true");
		EXPECT_EQ(obj.args[5], "\\\"OS\\\" / \\\"Users\\\"");
	}

	EXPECT_EQ(obj.returnable, true);
	EXPECT_EQ(obj.whereOutput, "./ w h y /  u /reading/this/a.bin");
}