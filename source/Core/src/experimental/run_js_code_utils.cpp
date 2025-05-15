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
#include "Core/filesystem/pseudo_fs.h"
#include "Core/filesystem/nrfs.h"

#include "Core/experimental/run_js_code_utils.h"

#include "ThirdParty/mjs/value.h"
#include "ThirdParty/mjs/parser.h"
#include "ThirdParty/mjs/interpreter.h"
#include "ThirdParty/mjs/printer.h"
#include "ThirdParty/mjs/platform.h"
#include "ThirdParty/mjs/char_conversions.h"
#include "ThirdParty/mjs/function_object.h"
#include "ThirdParty/mjs/array_object.h"

mjs::value core_experimental::getCString(std::string str, mjs::gc_heap& gc) {
	return mjs::value(mjs::string(gc, str));
}

mjs::value core_experimental::objectUserData(mjs::gc_heap& gc, core::User* targetUser) {
	if (!targetUser)
		return mjs::value::null;
	std::cout << "cho?\n";
	mjs::gc_heap_ptr a = gc.allocate_and_construct<mjs::object>(sizeof(mjs::object), mjs::string(gc, "User"), nullptr);
	mjs::object_ptr userObj(a);
	userObj->put(mjs::string(gc, "name"), getCString(targetUser->getUsername(), gc), mjs::property_attribute::read_only);
	userObj->put(mjs::string(gc, "display_name"), getCString(targetUser->getDisplayName(), gc), mjs::property_attribute::read_only);
	userObj->put(mjs::string(gc, "permissions"), mjs::value(static_cast<double>(targetUser->getPermissions())), mjs::property_attribute::read_only);
	std::cout << "a ni cho\n";
	return mjs::value(userObj);
}