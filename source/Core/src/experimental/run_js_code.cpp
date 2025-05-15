#include <sstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <chrono>
#include <fstream>
#include <streambuf>
#include <cstring>
#include <filesystem>
#include <any>

#include "Core/CORE_info.h"
#include "Core/other/utils.h"
#include "Core/print/print.h"
#include "Core/command/handler_commands.h"
#include "Core/users/user_manager.h"
#include "Core/users/user.h"
#include "Core/users/user_permissions_enum.h"

#include "Core/experimental/run_js_code.h"
#include "Core/experimental/run_js_code_functions.h"

#include "ThirdParty/mjs/value.h"
#include "ThirdParty/mjs/parser.h"
#include "ThirdParty/mjs/interpreter.h"
#include "ThirdParty/mjs/printer.h"
#include "ThirdParty/mjs/platform.h"
#include "ThirdParty/mjs/char_conversions.h"
#include "ThirdParty/mjs/function_object.h"

using namespace mjs;

constexpr uint32_t deafult_heap_size = 1<<24;
std::wstring base_dir;

std::shared_ptr<source_file> read_utf8_file(version ver, const std::wstring_view filename) {
	const auto u8fname = unicode::utf16_to_utf8(filename);
#ifdef _MSC_VER
	std::ifstream in(std::wstring{filename});
#else
	std::ifstream in(u8fname);
#endif
	if (!in) throw std::runtime_error("Could not open \"" + u8fname + "\"");
	return std::make_shared<source_file>(
		filename,
		unicode::utf8_to_utf16(std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>())),
		ver
		);
}

std::shared_ptr<source_file> make_source(const std::wstring_view s, version ver) {
	return std::make_shared<source_file>(std::wstring(L"inline code"), std::wstring(s), ver);
}

value load_file(interpreter& i, const std::wstring_view path) {   
#if 0
	std::wcout << "Loading " << path << "\n";
#endif
	auto& global = *i.global();
	std::unique_ptr<block_statement> bs;
	try {
		bs = parse(read_utf8_file(global.language_version(), base_dir + std::wstring{path}), global.strict_mode() ? parse_mode::strict : parse_mode::non_strict);
	} catch (const std::exception& e) {
		throw native_error_exception{native_error_type::syntax, global.stack_trace(), e.what()};
	}
	return i.eval(*bs);
}

void core_experimental::runCode(const std::string& code, core::User* user) {
	try {
		gc_heap heap{deafult_heap_size};
		interpreter i{heap, version::latest};
		std::string str;
		core_experimental::addUserManagerFunctions(i, heap);
		core_experimental::addPseudoFileSystemFunctions(i, user, heap);
		//core_experimental::addSimpleFileSystemFunctions(i, user, heap);
		core_experimental::addOtherFunctions(i, user, heap, str);
		try {
			const value res = i.eval(*parse(make_source(std::wstring(code.begin(), code.end()), version::latest)));
		}
		catch (const std::exception& e) {
			std::cout << e.what() << "\n";
		}
	}
	catch(const std::exception& e) {
		std::cout << e.what() << "\n";
	}
}

void core_experimental::runCode(const std::string& code, core::User* user, std::string& str) {
	try {
		gc_heap heap{deafult_heap_size};
		interpreter i{heap, version::latest};
		core_experimental::addUserManagerFunctions(i, heap);
		core_experimental::addPseudoFileSystemFunctions(i, user, heap);
		//core_experimental::addSimpleFileSystemFunctions(i, user, heap);
		core_experimental::addOtherFunctions(i, user, heap, str);
		try {
			const value res = i.eval(*parse(make_source(std::wstring(code.begin(), code.end()), version::latest)));
		}
		catch (const std::exception& e) {
			std::cout << e.what() << "\n";
		}
	}
	catch(const std::exception& e) {
		std::cout << e.what() << "\n";
	}
}

void core_experimental::runCode(const std::string& code, const core::UserPermissions& permissions) {
	core::User tempUser = core::User("UNKNOWN_TEMP_USER_FOR_RUNNING_COMMANDS_OR_JS_CODE", permissions);
	core_experimental::runCode(code, &tempUser);
}

void core_experimental::runCode(const std::string& code, const core::UserPermissions& permissions, std::string& str) {
	core::User tempUser = core::User("UNKNOWN_TEMP_USER_FOR_RUNNING_COMMANDS_OR_JS_CODE", permissions);
	core_experimental::runCode(code, &tempUser, str);
}