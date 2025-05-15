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

void core_experimental::addPseudoFileSystemFunctions(mjs::interpreter& i, core::User* user, mjs::gc_heap& gc) {
	auto global = i.global();

	put_native_function(global, global, "nrc_getFileData", [&i, &gc](const value&, const std::vector<value>& args) {
		if (args.size() < 2 || args[0].type() != value_type::string || args[1].type() != value_type::number) {
			throw native_error_exception{native_error_type::eval, i.global()->stack_trace(), "path must be a string (and curDisk must be number)"};
		}

		int code;
		size_t curDisk = args.at(1).number_value();
		std::string path(args.at(0).string_value().view().begin(), args.at(0).string_value().view().end());
		core::FileData file = core::pseudoFS()->getFileData(path, curDisk, code);

		if (file.name.empty()) {
			throw native_error_exception{native_error_type::eval, i.global()->stack_trace(), "file is null"};
		}

		gc_heap_ptr a = gc.allocate_and_construct<object>(sizeof(object), string(gc, "FileData"), nullptr);
		object_ptr fileObj(a);
		fileObj->put(string(gc, "name"), getCString(file.name, gc), property_attribute::read_only);
		fileObj->put(string(gc, "content"), getCString(file.content, gc), property_attribute::read_only);
		fileObj->put(string(gc, "id"), value(static_cast<double>(file.id)), property_attribute::read_only);
		fileObj->put(string(gc, "owner"), objectUserData(gc, file.owner), property_attribute::read_only);
		fileObj->put(string(gc, "system"), value(file.system), property_attribute::read_only);
		fileObj->put(string(gc, "hidden"), value(file.hidden), property_attribute::read_only);
		fileObj->put(string(gc, "link_path"), getCString(file.linkPath, gc), property_attribute::read_only);
		return value(fileObj);
	}, 1);
	
	put_native_function(global, global, "nrc_getFolderData", [&i, &gc](const value&, const std::vector<value>& args) {
		if (args.size() < 2 || args[0].type() != value_type::string || args[1].type() != value_type::number) {
			throw native_error_exception{native_error_type::eval, i.global()->stack_trace(), "path must be a string (and curDisk must be number)"};
		}

		int code;
		size_t curDisk = args.at(1).number_value();
		std::string path(args.at(0).string_value().view().begin(), args.at(0).string_value().view().end());
		core::FolderData folder = core::pseudoFS()->getFolderData(path, curDisk, code);

		if (folder.name.empty()) {
			throw native_error_exception{native_error_type::eval, i.global()->stack_trace(), "file is null"};
		}

		gc_heap_ptr a = gc.allocate_and_construct<object>(sizeof(object), string(gc, "FolderData"), nullptr);
		object_ptr folderData(a);
		folderData->put(string(gc, "name"), getCString(folder.name, gc), property_attribute::read_only);
		folderData->put(string(gc, "id"), value(static_cast<double>(folder.id)), property_attribute::read_only);
		folderData->put(string(gc, "owner"), objectUserData(gc, folder.owner), property_attribute::read_only);
		folderData->put(string(gc, "system"), value(folder.system), property_attribute::read_only);
		folderData->put(string(gc, "hidden"), value(folder.hidden), property_attribute::read_only);
		folderData->put(string(gc, "link_path"), getCString(folder.linkPath, gc), property_attribute::read_only);
		return value(folderData);
	}, 1);

	put_native_function(global, global, "nrc_createFile", [&i](const value&, const std::vector<value>& args) {
		if (args.size() < 2 || args[0].type() != value_type::string || args[1].type() != value_type::number) {
			throw native_error_exception{native_error_type::eval, i.global()->stack_trace(), "path must be a string (and curDisk must be number)"};
		}

		size_t curDisk = args.at(1).number_value();
		int code;
		std::string path(args.at(0).string_value().view().begin(), args.at(0).string_value().view().end());
		if (!core::pseudoFS()->isFile(path)) {
			throw native_error_exception{native_error_type::eval, i.global()->stack_trace(), "Bad path to folder or missing file name."};
		}
		if (!core::pseudoFS()->getFileData(path, curDisk, code).name.empty()) {
			throw native_error_exception{native_error_type::eval, i.global()->stack_trace(), "The file already exists!"};
		}
		core::pseudoFS()->createFile(path, curDisk);
		core::pseudoFS()->getNRFS()->saveData();
		return value::undefined;
	}, 1);

	put_native_function(global, global, "nrc_createFolder", [&i](const value&, const std::vector<value>& args) {
		if (args.size() < 2 || args[0].type() != value_type::string || args[1].type() != value_type::number) {
			throw native_error_exception{native_error_type::eval, i.global()->stack_trace(), "path must be a string (and curDisk must be number)"};
		}

		size_t curDisk = args.at(1).number_value();

		std::string path(args.at(0).string_value().view().begin(), args.at(0).string_value().view().end());
		if (core::pseudoFS()->isFile(path)) {
			throw native_error_exception{native_error_type::eval, i.global()->stack_trace(), "Bad path"};
		}
		if (core::pseudoFS()->folderExists(path, curDisk)) {
			throw native_error_exception{native_error_type::eval, i.global()->stack_trace(), "The folder already exists!"};
		}
		core::pseudoFS()->createFolder(path, curDisk);
		core::pseudoFS()->getNRFS()->saveData();
		return value::undefined;
	}, 1);

	put_native_function(global, global, "nrc_delete", [&i](const value&, const std::vector<value>& args) {
		if (args.size() < 2 || args[0].type() != value_type::string || args[1].type() != value_type::number) {
			throw native_error_exception{native_error_type::eval, i.global()->stack_trace(), "path must be a string (and curDisk must be number)"};
		}

		int code;
		size_t curDisk = args.at(1).number_value();
		std::string path(args.at(0).string_value().view().begin(), args.at(0).string_value().view().end());
		if (core::pseudoFS()->isFile(path) && !core::pseudoFS()->getFileData(path, curDisk, code).name.empty()) {
			core::pseudoFS()->deleteFile(path, curDisk);
		}
		else if (core::pseudoFS()->folderExists(path, curDisk)) {
			core::pseudoFS()->deleteFolder(path, curDisk);
		}
		core::pseudoFS()->getNRFS()->saveData();
		return value::undefined;
	}, 1);

	put_native_function(global, global, "nrc_rename", [&i](const value&, const std::vector<value>& args) {
		if (args.size() < 3 || args[0].type() != value_type::string || args[1].type() != value_type::string || args[2].type() != value_type::number) {
			throw native_error_exception{native_error_type::eval, i.global()->stack_trace(), "path/newName must be string!"};
		}

		int code;
		size_t curDisk = args.at(2).number_value();
		std::string path(args.at(0).string_value().view().begin(), args.at(0).string_value().view().end());
		std::string newName(args.at(1).string_value().view().begin(), args.at(1).string_value().view().end());
		if (core::pseudoFS()->isFile(path) && !core::pseudoFS()->getFileData(path, curDisk, code).name.empty()) {
			core::pseudoFS()->renameFile(path, curDisk, newName);
		}
		else if (core::pseudoFS()->folderExists(path, curDisk)) {
			core::pseudoFS()->renameFolder(path, curDisk, newName);
		}
		core::pseudoFS()->getNRFS()->saveData();
		return value::undefined;
	}, 2);

	put_native_function(global, global, "nrc_move", [&i](const value&, const std::vector<value>& args) {
		if (args.size() < 2 || args[0].type() != value_type::string || args[1].type() != value_type::string || args[2].type() != value_type::number) {
			throw native_error_exception{native_error_type::eval, i.global()->stack_trace(), "path/newPath must be string!"};
		}

		int code;
		size_t curDisk = args.at(2).number_value();
		std::string path(args.at(0).string_value().view().begin(), args.at(0).string_value().view().end());
		std::string newPath(args.at(1).string_value().view().begin(), args.at(1).string_value().view().end());
		if (core::pseudoFS()->isFile(path) && !core::pseudoFS()->getFileData(path, curDisk, code).name.empty() && core::pseudoFS()->getFileData(newPath, curDisk, code).name.empty()) {
			core::pseudoFS()->moveFile(path, curDisk, newPath, curDisk);
		}
		else if (core::pseudoFS()->folderExists(path, curDisk) && !core::pseudoFS()->folderExists(newPath, curDisk)) {
			core::pseudoFS()->moveFolder(path, curDisk, newPath, curDisk);
		}
		else {
			throw native_error_exception{native_error_type::eval, i.global()->stack_trace(), "The folder/file in that path already exists!"};
		}
		core::pseudoFS()->getNRFS()->saveData();
		return value::undefined;
	}, 2);

	put_native_function(global, global, "nrc_setFAttribute", [&i, &user](const value&, const std::vector<value>& args) {
		if (args.size() < 3 || args[0].type() != value_type::string ||
			args[1].type() != value_type::string ||
			(args[2].type() != value_type::string && args[2].type() != value_type::boolean) ||
			args[3].type() != value_type::number) {
			throw native_error_exception{native_error_type::eval, i.global()->stack_trace(), (args.size() == 3 && (args[2].type() != value_type::string && args[2].type() != value_type::boolean) ? "attribute must be string or boolean!" : "path/newPath must be string!")};
		}

		int code;
		size_t curDisk = args.at(3).number_value();
		std::string path(args.at(0).string_value().view().begin(), args.at(0).string_value().view().end());
		std::string type(args.at(1).string_value().view().begin(), args.at(1).string_value().view().end());

		if (user->getPermissions() < core::UserPermissions::ADMIN) {
			if (core::pseudoFS()->isFile(path)) {
				core::FileData file = core::pseudoFS()->getFileData(path, curDisk, code);
				if (code != core::PseudoFSCodes::OK) {
					throw native_error_exception{native_error_type::eval, i.global()->stack_trace(), core::pseudoFSCodesS(code)};
				}

				if (file.owner != user || file.system) {
					throw native_error_exception{native_error_type::eval, i.global()->stack_trace(), "Not enough permissions."};
				}
			}
			else {
				core::FolderData folder = core::pseudoFS()->getFolderData(path, curDisk, code);
				if (code != core::PseudoFSCodes::OK) {
					throw native_error_exception{native_error_type::eval, i.global()->stack_trace(), core::pseudoFSCodesS(code)};
				}

				if (folder.owner != user || folder.system) {
					throw native_error_exception{native_error_type::eval, i.global()->stack_trace(), "Not enough permissions."};
				}
			}
		}

		if (type != "content" && type != "hidden" && type != "system" && type != "owner") {
			throw native_error_exception{native_error_type::eval, i.global()->stack_trace(), "This attribute does not exist."};
		}

		std::any attribute;

		if (args[2].type() == value_type::boolean)
			attribute = args.at(2).boolean_value();
		else
			attribute = std::string(args.at(2).string_value().view().begin(), args.at(2).string_value().view().end());

		if (core::pseudoFS()->isFile(path) && !core::pseudoFS()->getFileData(path, curDisk, code).name.empty()) {
			core::pseudoFS()->setFileAtt(
				path,
				curDisk,
				type,
				(
					type == "owner" ?
					(&core::userManager()->getUser(std::any_cast<std::string>(attribute))) :
					attribute
				)
			);
		}
		else if (core::pseudoFS()->folderExists(path, curDisk)) {
			if (type == "content") {
				throw native_error_exception{native_error_type::eval, i.global()->stack_trace(), "This attribute does not exist."};
			}
			core::pseudoFS()->setFolderAtt(
				path,
				curDisk,
				type,
				(
					type == "owner" ?
					(&core::userManager()->getUser(std::any_cast<std::string>(attribute))) :
					attribute
				)
			);
		}
		core::pseudoFS()->getNRFS()->saveData();
		return value::undefined;
	}, 3);
}