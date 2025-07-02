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
#include "Core/print/print.h"
#include "Core/command/handler_commands.h"
#include "Core/command/command_structs.h"
#include "Core/command/command_parser.h"
#include "Core/users/user_manager.h"
#include "Core/filesystem/pseudo_fs.h"
#include "Core/filesystem/nrfs.h"

#include "Core/experimental/run_js_code_functions.h"
#include "Core/experimental/run_js_code_utils.h"

#include "ThirdParty/mjs/value.h"
#include "ThirdParty/mjs/parser.h"
#include "ThirdParty/mjs/interpreter.h"
#include "ThirdParty/mjs/printer.h"
#include "ThirdParty/mjs/platform.h"
#include "ThirdParty/mjs/char_conversions.h"
#include "ThirdParty/mjs/function_object.h"
#include "ThirdParty/mjs/array_object.h"

using namespace mjs;

void core_experimental::addOtherFunctions(mjs::interpreter& i, core::User* user, mjs::gc_heap& gc, std::string& str) {
	auto global = i.global();

	/*
	put_native_function(global, global, "load", [&i](const value&, const std::vector<value>& args) {
		if (args.size() != 1 || args[0].type() != value_type::string) {
			throw native_error_exception{native_error_type::eval, i.global()->stack_trace(), "path must be a string"};
		}
		return load_file(i, args[0].string_value().view());
	}, 1);
	*/
	
	put_native_function(global, global, "nrc_getCoreVersion", [&gc](const value&, const std::vector<value>&) {
		return getCString(CORE_VERSION, gc);
	}, 0);

	put_native_function(global, global, "nrc_getCoreVersionType", [&gc](const value&, const std::vector<value>&) {
		return getCString(CORE_VERSION_TYPE, gc);
	}, 0);

	put_native_function(global, global, "print", [&i](const value&, const std::vector<value>& args) {
		if (args.size() != 1) {
			throw native_error_exception{native_error_type::eval, i.global()->stack_trace(), "message must be a string/boolean/number/object/null/undefined/reference"};
		}
		if (args.at(0).type() != value_type::object) {
			std::wstring wstr = debug_string(args.at(0));
			std::string output(wstr.begin(), wstr.end());
			core::print(output, core::PrintColors::aqua);
		}
		core::print();
		return value::undefined;
	}, 1);

	put_native_function(global, global, "nrc_executeCommand", [&i, &user](const value&, const std::vector<value>& args) {
		if (args.size() < 1 || args[0].type() != value_type::string) {
			throw native_error_exception{native_error_type::eval, i.global()->stack_trace(), "command must be string!"};
		}

		std::string rawCommand(args.at(0).string_value().view().begin(), args.at(0).string_value().view().end());
		auto commands = core::handlerCommands()->getParser()->parse(rawCommand);
		for (auto command : commands) {
			core::handlerCommands()->sendCommand(user, command);
		}
		return value::undefined;
	}, 1);

	put_native_function(global, global, "nrc_executeCommandAndReturn", [&i, &gc, &user](const value&, const std::vector<value>& args) {
		if (args.size() < 1 || args[0].type() != value_type::string) {
			throw native_error_exception{native_error_type::eval, i.global()->stack_trace(), "command must be string!"};
		}

		std::string rawCommand(args.at(0).string_value().view().begin(), args.at(0).string_value().view().end());
		auto commands = core::handlerCommands()->getParser()->parse(rawCommand);
		std::string output;
		for (auto command : commands) {
			std::string str;
			core::handlerCommands()->sendCommand(user, command, str);
			output += str + "\n";
		}
		return getCString(output, gc);
	}, 1);

	put_native_function(global, global, "nrc_strReturn", [&i, &str](const value&, const std::vector<value>& args) {
		if (args.size() < 1 || args[0].type() != value_type::string) {
			throw native_error_exception{native_error_type::eval, i.global()->stack_trace(), "str must be string!"};
		}

		std::string r(args.at(0).string_value().view().begin(), args.at(0).string_value().view().end());
		str = r;
		return value::undefined;
	}, 1);
}