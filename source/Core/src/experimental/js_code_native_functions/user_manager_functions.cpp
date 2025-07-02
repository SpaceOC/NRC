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

void core_experimental::addUserManagerFunctions(mjs::interpreter& i, mjs::gc_heap& gc) {
	auto global = i.global();

	put_native_function(global, global, "nrc_getAllUsers", [&i, &gc](const value&, const std::vector<value>&) {
		std::vector<value> users;
		for (const auto& user : core::userManager()->getUserMap())
			users.push_back(getCString(user.first, gc));
		object_ptr vectorObj = make_array(i.global(), users);
		return value(vectorObj);
	}, 0);

	put_native_function(global, global, "nrc_getUserData", [&i, &gc](const value&, const std::vector<value>& args) {
		if (args.size() != 1 || args[0].type() != value_type::string) {
			throw native_error_exception{native_error_type::eval, i.global()->stack_trace(), "username must be a string"};
		}


		std::string username(args.at(0).string_value().view().begin(), args.at(0).string_value().view().end());
		if (!core::userManager()->userExist(username)) {
			throw native_error_exception{native_error_type::eval, i.global()->stack_trace(), "The user doesn't exist"};
		}

		return value(objectUserData(gc, &core::userManager()->getUser(username)));
	}, 1);
}