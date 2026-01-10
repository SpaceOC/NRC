//#include <iostream>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <memory>

#include "Core/filesystem/pseudo_fs.h"
#include "Core/filesystem/nrfs.h"
#include "Core/utils/string_util.h"
#include "Core/print/print.h"
#include "Core/users/user_manager.h"
#include "Core/users/user_permissions_enum.h"

std::string core::pseudoFSCodesS(int t) {
	switch(t) {
		case PseudoFSCodes::OK:
			return "Ok";
			break;
		case PseudoFSCodes::NOT_FOUND:
			return "Not found";
			break;
		case PseudoFSCodes::ALREADY_EXISTS:
			return "Already exists";
			break;
		case PseudoFSCodes::PATH_IS_EMPTY:
			return "Path is empty";
			break;
		case PseudoFSCodes::THIS_LINK:
			return "This is link";
			break;
		case PseudoFSCodes::BAD_PATH:
			return "Bad path";
			break;
		case PseudoFSCodes::JSON_PARSING_FAILED:
			return "JSON parsing failed";
			break;
		case PseudoFSCodes::IS_EMPTY:
			return "Is empty";
			break;
		case PseudoFSCodes::SMALL_SIZE:
			return "Small size";
			break;
	}
	return "Unknown error";
}

core::PseudoFS PFS;
core::PseudoFS* core::pseudoFS() {
	return &PFS;
}

bool core::PseudoFS::isFile(const std::string& path) {
	if (path.find(".") >= LONG_LONG_MAX) return false;
	std::string file = path.substr(path.find("."), path.length());
	return (file.find(".") == 0);
}

int core::PseudoFS::createFolderHelper(std::vector<std::string> path, FolderData* currentFolder, FolderData* oneFolderData) {
	if (path.empty())
		return core::PseudoFSCodes::PATH_IS_EMPTY;

	std::string currentFolderName = path.at(0);
	path.erase(path.begin());

	for (auto& folder : currentFolder->folders) {
		if (folder->name == currentFolderName)
			return createFolderHelper(path, folder.get(), oneFolderData);
	}

	if (path.size() < 2) {
		bool folderExists = std::any_of(currentFolder->folders.begin(), currentFolder->folders.end(), [&](std::shared_ptr<FolderData> f) {
			if (!oneFolderData) return false;
			return f->name == oneFolderData->name;
		});
		
		if (folderExists) return core::PseudoFSCodes::ALREADY_EXISTS;

		auto newFolder = !oneFolderData
			? std::make_shared<core::FolderData>(
			core::FolderData{
				currentFolderName,
				std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()),
				std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()),
				{}, {}, false, false, nullptr, ""
			}
			)
		: std::make_shared<core::FolderData>(
			oneFolderData->name,
			oneFolderData->createTime,
			oneFolderData->lastEditTime,
			oneFolderData->files,
			oneFolderData->folders,
			oneFolderData->system,
			oneFolderData->hidden,
			oneFolderData->realFolderPointer,
			oneFolderData->realFolderPath,
			oneFolderData->owner,
			oneFolderData->ownerUsername,
			oneFolderData->parent
		);
		
		currentFolder->folders.push_back(std::move(newFolder));
		//_nrfs->disks[_currentDiskId]->update();
		return core::PseudoFSCodes::OK;
	}
	return core::PseudoFSCodes::NOT_FOUND;
}

int core::PseudoFS::setFolderAttHelper(std::vector<std::string> path, FolderData* currentFolder, const std::string& what, const std::any& newAtt) {
	if (path.empty())
		return core::PseudoFSCodes::PATH_IS_EMPTY;
	else if (path.size() == 1) {
		for (auto& folder : currentFolder->folders) {
			if (folder->name == path.at(0)) {
				if (what == "hidden") folder->hidden = std::any_cast<bool>(newAtt);
				else if (what == "system") folder->system = std::any_cast<bool>(newAtt);
				else if (what == "owner") folder->owner = std::any_cast<User*>(newAtt);
				folder->lastEditTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
				//_nrfs->disks[_currentDiskId]->update();
				return core::PseudoFSCodes::OK;
			}
		}
	}
	else {
		for (auto& folder : currentFolder->folders) {
			if (folder->name == path.at(0)) {
				path.erase(path.begin());
				return setFolderAttHelper(path, folder.get(), what, newAtt);
			}
		}
	}
	return core::PseudoFSCodes::NOT_FOUND;
}

int core::PseudoFS::renameFolderHelper(std::vector<std::string> path, FolderData* currentFolder, const std::string& newName) {
	if (path.empty())
		return core::PseudoFSCodes::PATH_IS_EMPTY;
	else if (path.size() == 1) {
		bool newFolderExists = std::any_of(currentFolder->folders.begin(), currentFolder->folders.end(), [&](std::shared_ptr<FolderData> f) { return f->name == newName; });
		if (!newFolderExists) {
			for (auto& folder : currentFolder->folders) {
				if (folder->name == path.at(0)) {
					folder->name = newName;
					folder->lastEditTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
					return core::PseudoFSCodes::OK;
				}
			}
		}
		else
			return core::PseudoFSCodes::ALREADY_EXISTS;
	}
	else {
		for (auto& folder : currentFolder->folders) {
			if (folder->name == path.at(0) && path.size() > 1) {
				path.erase(path.begin());
				return renameFolderHelper(path, folder.get(), newName);
			}
		}
	}
	return core::PseudoFSCodes::NOT_FOUND;
}

int core::PseudoFS::deleteFolderHelper(std::vector<std::string> path, FolderData* currentFolder) {
	if (path.empty())
		return core::PseudoFSCodes::PATH_IS_EMPTY;
	else if (path.size() == 1) {
		for (auto it = currentFolder->folders.begin(); it != currentFolder->folders.end(); ) {
			if ((*it)->name == path.at(0)) {
				(*it).reset();
				(*it) = nullptr;
				it = currentFolder->folders.erase(it);
				return core::PseudoFSCodes::OK;
			}
			else
				it++;
		}
	}
	else {
		for (auto& folder : currentFolder->folders) {
			if (folder->name == path.at(0)) {
				path.erase(path.begin());
				return deleteFolderHelper(path, folder.get());
			}
		}
	}
	return core::PseudoFSCodes::NOT_FOUND;
}

int core::PseudoFS::moveFolderHelper(std::vector<std::string> path, size_t diskId, FolderData* currentFolder, const core::FolderData& oldFolderData, const std::string& oldPath) {
	if (path.empty()) {
		bool exists = std::any_of(currentFolder->folders.begin(), currentFolder->folders.end(), [&](std::shared_ptr<FolderData> f) { return f->name == oldFolderData.name; });
		if (exists) {
			return core::PseudoFSCodes::ALREADY_EXISTS;
		}

		auto newFolder = std::make_shared<core::FolderData>(oldFolderData);
		currentFolder->folders.push_back(std::move(newFolder));
		_nrfs->disks[diskId]->update();
		return core::PseudoFSCodes::OK;
	}
	else {
		for (auto& folder : currentFolder->folders) {
			if (folder->name == path.at(0)) {
				path.erase(path.begin());
				return moveFolderHelper(path, diskId, folder.get(), oldFolderData, oldPath);
			}
		}
	}
	return core::PseudoFSCodes::NOT_FOUND;
}

core::FolderData core::PseudoFS::getFolderDataHelper(std::vector<std::string> path, size_t diskId, FolderData* currentFolder, int& code) {
	if (path.empty()) {
		code = core::PseudoFSCodes::PATH_IS_EMPTY;
		return {};
	}
	else if (path.size() == 1) {
		for (std::shared_ptr<FolderData> anotherFolder : currentFolder->folders) {
			if (anotherFolder->name == path.at(0)) {
				if (anotherFolder->realFolderPointer!= nullptr && !anotherFolder->realFolderPath.empty())
					return getFolderData(anotherFolder->realFolderPath, diskId, code);
				else {
					code = core::PseudoFSCodes::OK;
					return *anotherFolder.get();
				}
			}
		}
	}
	else {
		for (auto& folder : currentFolder->folders) {
			if (folder->name == path.at(0) && path.size() > 1) {
				path.erase(path.begin());
				return getFolderDataHelper(path, diskId, folder.get(), code);
			}
		}
	}
	code = core::PseudoFSCodes::NOT_FOUND;
	return {};
}

int core::PseudoFS::createFileHelper(std::vector<std::string> path, FolderData* currentFolder, FileData* oneFileData) {
	if (path.size() == 1 && isFile(path.at(0))) {
		auto newFile = !oneFileData
			? std::make_shared<core::FileData>(
			core::FileData{
				path.at(0), 
				"",
				std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()),
				std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()),
				false,
				false,
				nullptr,
				""
			}
			)
		: std::make_shared<core::FileData>(
			oneFileData->name,
			oneFileData->content,
			oneFileData->createTime,
			oneFileData->lastEditTime,
			oneFileData->system,
			oneFileData->hidden,
			oneFileData->realFilePointer,
			oneFileData->realFilePath,
			oneFileData->owner,
			oneFileData->ownerUsername
		);

		currentFolder->files.push_back(std::move(newFile));

		//_nrfs->disks[_currentDiskId]->update();
		return core::PseudoFSCodes::OK;
	}
	else {
		for (auto& folder : currentFolder->folders) {
			if (folder->name == path.at(0)) {
				path.erase(path.begin());
				/*
				if (!path.empty()) {
					return createFileHelper(path, folder, oneFileData);
				}
				else {
					if (oneFileData.name.empty()) {
						folderfiles.push_back({folder->name, "", tempId++, std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()), std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()), false, false, nullptr, ""});
					}
					else
						folder->files.push_back(new FileData(oneFileData));
				}
				_nrfs->disks[_currentDiskId]->update();
				return core::PseudoFSCodes::OK;
				*/
				return createFileHelper(path, folder.get(), oneFileData);
			}
		}
	}
	return core::PseudoFSCodes::NOT_FOUND;
}

int core::PseudoFS::renameFileHelper(std::vector<std::string> path, FolderData* currentFolder, const std::string& newName) {
	if (path.empty())
		return core::PseudoFSCodes::PATH_IS_EMPTY;
	else if (path.size() == 1) {
		for (auto& file : currentFolder->files) {
			bool newFileExists = std::any_of(currentFolder->files.begin(), currentFolder->files.end(), [&](std::shared_ptr<FileData> f) { return f->name == newName; });
			if (file->name == path.at(0) && !newFileExists) {
				file->name = newName;
				file->lastEditTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
				//_nrfs->disks[_currentDiskId]->update();
				return core::PseudoFSCodes::OK;
			}
			else if (newFileExists) {
				return core::PseudoFSCodes::ALREADY_EXISTS;
			}
		}
	}
	else {
		for (auto& folder : currentFolder->folders) {
			if (folder->name == path.at(0) && path.size() > 1) {
				path.erase(path.begin());
				return renameFileHelper(path, folder.get(), newName);
			}
		}
	}
	return core::PseudoFSCodes::NOT_FOUND;
}

int core::PseudoFS::deleteFileHelper(std::vector<std::string> path, FolderData* currentFolder) {
	if (path.empty())
		return core::PseudoFSCodes::PATH_IS_EMPTY;
	else if (path.size() == 1 && isFile(path.at(0))) {
		auto it = currentFolder->files.begin();
		for (auto& file : currentFolder->files) {
			if (file->name == path.at(0)) {
				it = currentFolder->files.erase(it);
				return core::PseudoFSCodes::OK;
			}
			it++;
		}
		//_nrfs->disks[_currentDiskId]->update();
	}
	else {
		for (auto& folder : currentFolder->folders) {
			if (folder->name == path.at(0) && !isFile(path.at(0))) {
				path.erase(path.begin());
				return deleteFileHelper(path, folder.get());
			}
		}
	}
	return core::PseudoFSCodes::NOT_FOUND;
}

int core::PseudoFS::moveFileHelper(std::vector<std::string> path, size_t diskId, FolderData* currentFolder, const core::FileData& oldFileData, const std::string& oldPath) {
	if (path.empty()) {
		bool exists = std::any_of(currentFolder->files.begin(), currentFolder->files.end(), [&](std::shared_ptr<FileData> f) { return f->name == oldFileData.name; });

		if (exists)
			return core::PseudoFSCodes::ALREADY_EXISTS;
			
		auto newFile = std::make_shared<core::FileData>(oldFileData);

		currentFolder->files.push_back(std::move(newFile));
		deleteFile(oldPath, diskId);
		_nrfs->disks[_currentDiskId]->update();
		return core::PseudoFSCodes::OK;
	}
	else {
		for (auto& folder : currentFolder->folders) {
			if (folder->name == path.at(0)) {
				path.erase(path.begin());
				return moveFileHelper(path, diskId, folder.get(), oldFileData, oldPath);
			}
		}
	}
	return core::PseudoFSCodes::NOT_FOUND;
}

int core::PseudoFS::setFileAttHelper(std::vector<std::string> path, FolderData* currentFolder, const std::string& what, const std::any& newAtt) {
	if (path.empty())
		return core::PseudoFSCodes::PATH_IS_EMPTY;
	else if (path.size() == 1) {
		for (auto& file : currentFolder->files) {
			if (file->name == path.at(0)) {
				if (what == "content") file->content = newAtt.type() != typeid(std::string) ?
								static_cast<std::string>(std::any_cast<const char*>(newAtt)) :
								std::any_cast<std::string>(newAtt);
				else if (what == "hidden") file->hidden = std::any_cast<bool>(newAtt);
				else if (what == "system") file->system = std::any_cast<bool>(newAtt);
				else if (what == "owner") file->owner = std::any_cast<User*>(newAtt);
				file->lastEditTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
				//_nrfs->disks[_currentDiskId]->update();
				return core::PseudoFSCodes::OK;
			}
		}
	}
	else {
		for (auto& folder : currentFolder->folders) {
			if (folder->name == path.at(0)) {
				path.erase(path.begin());
				return setFileAttHelper(path, folder.get(), what, newAtt);
			}
		}
	}
	return core::PseudoFSCodes::NOT_FOUND;
}

core::FileData core::PseudoFS::getFileDataHelper(std::vector<std::string> path, size_t diskId, FolderData* currentFolder, int& code) {
	if (path.empty()) {
		code = core::PseudoFSCodes::PATH_IS_EMPTY;
		return {};
	}
	else if (path.size() == 1) {
		for (auto& file : currentFolder->files) {
			if (file->name == path.at(0)) {
				if (file->realFilePointer!= nullptr && !file->realFilePath.empty())
					return getFileData(file->realFilePath, diskId, code);
				else {
					code = core::PseudoFSCodes::OK;
					return *file.get();
				}
			}
		}
	}
	else {
		for (auto& folder : currentFolder->folders) {
			if (folder->name == path.at(0)) {
				path.erase(path.begin());
				return getFileDataHelper(path, diskId, folder.get(), code);
			}
		}
	}
	code = core::PseudoFSCodes::NOT_FOUND;
	return {};
}

int core::PseudoFS::createFolder(const std::string& path, size_t diskId, FolderData* oneFolderData) {
	std::vector<std::string> parsedPath = core::string_util::split(path, '/');
	if (parsedPath.size() == 2) {
		bool folderExists = _nrfs->disks[diskId]->folders.empty() ? false : std::any_of(_nrfs->disks[diskId]->folders.begin(), _nrfs->disks[diskId]->folders.end(),
			[&](std::shared_ptr<FolderData> f) { return f->name == parsedPath.at(1);}
		);
		if (folderExists)
			return core::PseudoFSCodes::ALREADY_EXISTS;

		auto newFolder = !oneFolderData
			? std::make_shared<core::FolderData>(
				core::FolderData{
					parsedPath.at(1),
					std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()),
					std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()),
					{},
					{},
					false,
					false,
					nullptr,
					"",
					nullptr
				}
			)
		: std::make_shared<core::FolderData>(
			oneFolderData->name,
			oneFolderData->createTime,
			oneFolderData->lastEditTime,
			oneFolderData->files,
			oneFolderData->folders,
			oneFolderData->system,
			oneFolderData->hidden,
			oneFolderData->realFolderPointer,
			oneFolderData->realFolderPath,
			oneFolderData->owner,
			oneFolderData->ownerUsername,
			oneFolderData->parent
		);

		_nrfs->disks[diskId]->folders.push_back(std::move(newFolder));
	}
	else {
		parsedPath.erase(parsedPath.begin());
		for (auto& folder : _nrfs->disks[diskId]->folders) {
			if (parsedPath.at(0) == folder->name) {
				parsedPath.erase(parsedPath.begin());
				int code = createFolderHelper(parsedPath, folder.get(), oneFolderData);
				_nrfs->disks[diskId]->update();
				return code;
			}
		}
		return core::PseudoFSCodes::NOT_FOUND;
	}
	return core::PseudoFSCodes::OK;
}

int core::PseudoFS::setFolderAtt(const std::string& path, size_t diskId, const std::string& what, std::any newAtt) {
	std::vector<std::string> parsedPath = core::string_util::split(path, '/');
	if (parsedPath.size() == 2) {
		bool folderExists = std::any_of(_nrfs->disks[diskId]->folders.begin(), _nrfs->disks[diskId]->folders.end(),
			[&](std::shared_ptr<FolderData> f) { return f->name == parsedPath.at(1); }
		);
		if (!folderExists)
			return core::PseudoFSCodes::NOT_FOUND;

		for (auto& folder : _nrfs->disks[diskId]->folders) {
			if (folder->name == parsedPath.at(1)) {
				if (what == "hidden") folder->hidden = std::any_cast<bool>(newAtt);
				else if (what == "system") folder->system = std::any_cast<bool>(newAtt);
				else if (what == "owner") folder->owner = std::any_cast<User*>(newAtt);
				folder->lastEditTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
				break;
			}
		}
	}
	else {
		parsedPath.erase(parsedPath.begin());
		for (auto& folder : _nrfs->disks[diskId]->folders) {
			if (parsedPath.at(0) == folder->name) {
				parsedPath.erase(parsedPath.begin());
				int code = setFolderAttHelper(parsedPath, folder.get(), what, newAtt);
				return code;
			}
		}
		return core::PseudoFSCodes::NOT_FOUND;
	}
	return core::PseudoFSCodes::OK;
}

int core::PseudoFS::renameFolder(const std::string& path, size_t diskId, std::string newName) {
	std::vector<std::string> parsedPath = core::string_util::split(path, '/');
	if (parsedPath.size() == 2) {
		bool folderExists = std::any_of(_nrfs->disks[diskId]->folders.begin(), _nrfs->disks[diskId]->folders.end(),
			[&](std::shared_ptr<FolderData> f) { return f->name == parsedPath.at(1); }
		);
		if (!folderExists)
			return core::PseudoFSCodes::NOT_FOUND;

		bool newFolderExists = std::any_of(_nrfs->disks[diskId]->folders.begin(), _nrfs->disks[diskId]->folders.end(),
			[&](std::shared_ptr<FolderData> f) { return f->name == newName; }
		);
		if (newFolderExists)
				return core::PseudoFSCodes::ALREADY_EXISTS;
		
		for (auto& folder : _nrfs->disks[diskId]->folders) {
			if (folder->name == parsedPath.at(1)) {
				folder->name = newName;
				folder->lastEditTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
				break;
			}
		}
	}
	else {
		parsedPath.erase(parsedPath.begin());
		for (auto& folder : _nrfs->disks[diskId]->folders) {
			if (parsedPath.at(0) == folder->name) {
				parsedPath.erase(parsedPath.begin());
				int code = renameFolderHelper(parsedPath, folder.get(), newName);
				_nrfs->disks[diskId]->update();
				return code;
			}
		}
		return core::PseudoFSCodes::NOT_FOUND;
	}
	return core::PseudoFSCodes::OK;
}

int core::PseudoFS::deleteFolder(const std::string& path, size_t diskId) {
	std::vector<std::string> parsedPath = core::string_util::split(path, '/');
	if (parsedPath.size() == 2) {
		bool folderExists = std::any_of(_nrfs->disks[diskId]->folders.begin(), _nrfs->disks[diskId]->folders.end(), [&](std::shared_ptr<FolderData> f) { return f->name == parsedPath.at(1); });
		if (!folderExists)
			return core::PseudoFSCodes::NOT_FOUND;

		auto it = _nrfs->disks[diskId]->folders.begin();
		for (auto& folder : _nrfs->disks[diskId]->folders) {
			if (folder->name == parsedPath.at(1)) {
				folder.reset();
				_nrfs->disks[diskId]->folders.erase(it);
				_nrfs->disks[diskId]->update();
				break;
			}
			it++;
		}
	}
	else {
		parsedPath.erase(parsedPath.begin());
		for (auto& folder : _nrfs->disks[diskId]->folders) {
			if (parsedPath.at(0) == folder->name) {
				parsedPath.erase(parsedPath.begin());
				int code = deleteFolderHelper(parsedPath, folder.get());
				_nrfs->disks[diskId]->update();
				return code;
			}
		}
		return core::PseudoFSCodes::NOT_FOUND;
	}
	return core::PseudoFSCodes::OK;
}

int core::PseudoFS::moveFolder(const std::string& path, size_t diskId, const std::string& newPath, size_t anotherDiskId) {
	std::vector<std::string> parsedPath = core::string_util::split(newPath, '/');
	int code = 0;
	const FolderData oldFolderData = getFolderData(path, diskId, code);
	if (code != 1)
		return code;
	if (parsedPath.size() == 1) {
		bool newFolderExists = std::any_of(_nrfs->disks[anotherDiskId]->folders.begin(), _nrfs->disks[anotherDiskId]->folders.end(), [&](std::shared_ptr<FolderData> f) { return f->name == oldFolderData.name; });
		if (newFolderExists)
			return core::PseudoFSCodes::ALREADY_EXISTS;

		auto newFolder = std::make_shared<core::FolderData>(oldFolderData);

		_nrfs->disks[anotherDiskId]->folders.push_back(std::move(newFolder));
		deleteFolder(path, diskId);
		_nrfs->disks[anotherDiskId]->update();
	}
	else {
		parsedPath.erase(parsedPath.begin());
		for (auto& folder : _nrfs->disks[anotherDiskId]->folders) {
			if (parsedPath.at(0) == folder->name) {
				parsedPath.erase(parsedPath.begin());
				int code = moveFolderHelper(parsedPath, anotherDiskId, folder.get(), oldFolderData, path);
				if (code) {
					deleteFolder(path, diskId);
					_nrfs->disks[diskId]->update();
				}
				return code;
			}
		}
		return core::PseudoFSCodes::NOT_FOUND;
	}
	return core::PseudoFSCodes::OK;
}

core::FolderData core::PseudoFS::getFolderData(const std::string& path, size_t diskId, int& code) {
	std::vector<std::string> parsedPath = core::string_util::split(path, '/');
	if (parsedPath.size() == 2) {
		if (_nrfs->disks[diskId]->folders.empty()) {
			code = core::PseudoFSCodes::NOT_FOUND;
			return {};
		}

		for (auto folder : _nrfs->disks[diskId]->folders) {
			if (folder->name == parsedPath.at(1)) {
				if (folder->realFolderPointer != nullptr && !folder->realFolderPath.empty())
					return getFolderData(folder->realFolderPath, diskId, code);
				else {
					code = core::PseudoFSCodes::OK;
					return *folder.get();
				}
			}
		}
	}
	else {
		parsedPath.erase(parsedPath.begin());

		for (auto& folder : _nrfs->disks[diskId]->folders) {
			if (parsedPath.at(0) == folder->name) {
				parsedPath.erase(parsedPath.begin());
				return getFolderDataHelper(parsedPath, diskId, folder.get(), code);
			}
		}
	}
	code = core::PseudoFSCodes::NOT_FOUND;
	return {};
}

int core::PseudoFS::createFile(const std::string& path, size_t diskId, FileData* oneFileData) {
	std::vector<std::string> parsedPath = core::string_util::split(path, '/');

	if (parsedPath.size() == 2) {
		bool fileExists = _nrfs->disks[diskId]->files.empty() ? false : std::any_of(_nrfs->disks[diskId]->files.begin(), _nrfs->disks[diskId]->files.end(),
			[&](std::shared_ptr<FileData> f) { return f->name == parsedPath.at(1); }
		);
		if (fileExists)
			return core::PseudoFSCodes::ALREADY_EXISTS;

		auto newFile = !oneFileData
			? std::make_shared<core::FileData>(
				parsedPath.at(1), 
				"",
				std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()),
				std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()),
				false,
				false,
				nullptr,
				""
			)
			: std::make_shared<core::FileData>(
				oneFileData->name,
				oneFileData->content,
				oneFileData->createTime,
				oneFileData->lastEditTime,
				oneFileData->system,
				oneFileData->hidden,
				oneFileData->realFilePointer,
				oneFileData->realFilePath,
				oneFileData->owner,
				oneFileData->ownerUsername
			);

		_nrfs->disks[diskId]->files.push_back(std::move(newFile));
	}
	else {
		parsedPath.erase(parsedPath.begin());
		for (auto& folder : _nrfs->disks[diskId]->folders) {
			if (parsedPath.at(0) == folder->name) {
				parsedPath.erase(parsedPath.begin());
				return createFileHelper(parsedPath, folder.get(), oneFileData);
			}
		}
		return core::PseudoFSCodes::NOT_FOUND;
	}
	return core::PseudoFSCodes::OK;
}

int core::PseudoFS::renameFile(const std::string& path, size_t diskId, std::string newName) {
	std::vector<std::string> parsedPath = core::string_util::split(path, '/');
	if (parsedPath.size() == 2) {
		bool fileExists = std::any_of(_nrfs->disks[diskId]->files.begin(), _nrfs->disks[diskId]->files.end(),
			[&](std::shared_ptr<FileData> f) { return f->name == parsedPath.at(1); }
		);
		if (!fileExists)
			return core::PseudoFSCodes::NOT_FOUND;

		bool newFileExists = std::any_of(_nrfs->disks[diskId]->files.begin(), _nrfs->disks[diskId]->files.end(),
			[&](std::shared_ptr<FileData> f) { return f->name == newName; }
		);
		if (newFileExists)
				return core::PseudoFSCodes::ALREADY_EXISTS;

		for (auto& file : _nrfs->disks[diskId]->files) {
			if (file->name == parsedPath.at(1)) {
				file->name = newName;
				file->lastEditTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
				break;
			}
		}
	}
	else {
		parsedPath.erase(parsedPath.begin());
		for (auto& folder : _nrfs->disks[diskId]->folders) {
			if (parsedPath.at(0) == folder->name) {
				parsedPath.erase(parsedPath.begin());
				return renameFileHelper(parsedPath, folder.get(), newName);
			}
		}
		return core::PseudoFSCodes::NOT_FOUND;
	}
	return core::PseudoFSCodes::OK;
}

int core::PseudoFS::deleteFile(const std::string& path, size_t diskId) {
	std::vector<std::string> parsedPath = core::string_util::split(path, '/');
	if (parsedPath.size() == 2) {
		bool fileExists = std::any_of(_nrfs->disks[diskId]->files.begin(), _nrfs->disks[diskId]->files.end(),
			[&](std::shared_ptr<FileData> f) { return f->name == parsedPath.at(1); }
		);
		if (!fileExists)
			return core::PseudoFSCodes::NOT_FOUND;    

		auto it = _nrfs->disks[diskId]->files.begin();
		for (auto& file : _nrfs->disks[diskId]->files) {
			if (file->name == parsedPath.at(1)) {
				it = _nrfs->disks[diskId]->files.erase(it);
				break;
			}
			it++;
		}
		_nrfs->disks[diskId]->update();
	}
	else {
		parsedPath.erase(parsedPath.begin()); 
		for (auto& folder : _nrfs->disks[diskId]->folders) {
			if (parsedPath.at(0) == folder->name) {
				parsedPath.erase(parsedPath.begin());
				return deleteFileHelper(parsedPath, folder.get());
			}
		}
		return core::PseudoFSCodes::NOT_FOUND;
	}
	return core::PseudoFSCodes::OK;
}

int core::PseudoFS::moveFile(const std::string& path, size_t diskId, const std::string& newPath, size_t anotherDiskId) {
	try {
		std::vector<std::string> parsedPath = core::string_util::split(newPath, '/');
		int code = 0;
		const FileData& oldFileData = getFileData(path, diskId, code);
		if (code != 1)
			return code;
		if (parsedPath.size() == 1) {
			bool newFileExists = std::any_of(_nrfs->disks[anotherDiskId]->files.begin(), _nrfs->disks[anotherDiskId]->files.end(),
				[&](std::shared_ptr<FileData> f) { return f->name == oldFileData.name; }
			);
			if (newFileExists)
				return core::PseudoFSCodes::ALREADY_EXISTS;

			auto newFile = std::make_shared<core::FileData>(
				oldFileData.name,
				oldFileData.content,
				oldFileData.createTime,
				oldFileData.lastEditTime,
				oldFileData.system,
				oldFileData.hidden,
				oldFileData.realFilePointer,
				oldFileData.realFilePath,
				oldFileData.owner,
				oldFileData.ownerUsername
			);

			_nrfs->disks[anotherDiskId]->files.push_back(std::move(newFile));
			deleteFile(path, diskId);
			_nrfs->disks[anotherDiskId]->update();
		}
		else {
			parsedPath.erase(parsedPath.begin());

			for (auto& folder : _nrfs->disks[diskId]->folders) {
				if (parsedPath.at(0) == folder->name) {
					parsedPath.erase(parsedPath.begin());
					return moveFileHelper(parsedPath, diskId, folder.get(), oldFileData, path);
				}
			}
			return core::PseudoFSCodes::NOT_FOUND;
		}
	}
	catch(const std::exception& e) {
		trace(core::PrintColors::red,
			"moveFile Error: ", e.what(), "\n"
			"target file: ", getFileFromPath(path), "\n",
			"path: " , path, "\n"
			"new path: ", newPath
		);
		#ifdef NRC_WEB
		outputLog += "moveFile Error: " + std::string(e.what()) + "\n" +
			"target file: " + getFileFromPath(path) + "\n" +
			"path: " + path + "\n" +
			"new path: " + newPath + "\n";
		#endif
		return core::PseudoFSCodes::UNKNOWN_ERROR;
	}
	return core::PseudoFSCodes::OK;
}

int core::PseudoFS::setFileAtt(const std::string& path, size_t diskId, const std::string& what, std::any newAtt) {
	assert(!(newAtt.type() != typeid(std::string) && newAtt.type() != typeid(const char*) && newAtt.type() != typeid(bool) && newAtt.type() != typeid(User)));
	std::vector<std::string> parsedPath = core::string_util::split(path, '/');
	if (parsedPath.size() == 2) {
		bool fileExists = std::any_of(_nrfs->disks[diskId]->files.begin(), _nrfs->disks[diskId]->files.end(),
			[&](std::shared_ptr<FileData> f) { return f->name == parsedPath.at(1); }
		);
		if (!fileExists)
			return core::PseudoFSCodes::NOT_FOUND;

		for (auto& file : _nrfs->disks[diskId]->files) {
			if (file->name == parsedPath.at(1)) {
				if (what == "content") file->content = newAtt.type() != typeid(std::string) ?
								static_cast<std::string>(std::any_cast<const char*>(newAtt)) :
								std::any_cast<std::string>(newAtt);
				else if (what == "hidden") file->hidden = std::any_cast<bool>(newAtt);
				else if (what == "system") file->system = std::any_cast<bool>(newAtt);
				else if (what == "owner") file->owner = std::any_cast<User*>(newAtt);
				file->lastEditTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
				_nrfs->disks[diskId]->update();
				break;
			}
		}
	}
	else {
		parsedPath.erase(parsedPath.begin());
		for (auto& folder : _nrfs->disks[diskId]->folders) {
			if (parsedPath.at(0) == folder->name) {
				parsedPath.erase(parsedPath.begin());
				return setFileAttHelper(parsedPath, folder.get(), what, newAtt);
			}
		}
		return core::PseudoFSCodes::NOT_FOUND;
	}
	return core::PseudoFSCodes::OK;
}

core::FileData core::PseudoFS::getFileData(const std::string& path, size_t diskId, int& code) {
	std::vector<std::string> parsedPath = core::string_util::split(path, '/');
	if (parsedPath.size() == 2) {
		for (auto& file : _nrfs->disks[diskId]->files) {
			if (file->name == parsedPath.at(1)) {
				if (file->realFilePointer != nullptr && !file->realFilePath.empty())
					return getFileData(file->realFilePath, diskId, code);
				else {
					code = core::PseudoFSCodes::OK;
					return *file.get();
				}
			}
		}
		code = core::PseudoFSCodes::NOT_FOUND;
	}
	else {
		parsedPath.erase(parsedPath.begin());
		for (auto& folder : _nrfs->disks[diskId]->folders) {
			if (parsedPath.at(0) == folder->name) {
				parsedPath.erase(parsedPath.begin());
				return getFileDataHelper(parsedPath, diskId, folder.get(), code);
			}
		}
		code = core::PseudoFSCodes::NOT_FOUND;
	}
	return {};
}

void core::PseudoFS::printAllHelper(const std::vector<std::shared_ptr<FolderData>> &folders, const std::string &path, bool includeHidden) {
	for (auto& folder : folders) {
		const auto folderInSharedPtr = folder;

		std::string newPath = path + folderInSharedPtr->name + "/";

		if (!folderInSharedPtr->folders.empty()) {
			bool showFolder = includeHidden && folderInSharedPtr->hidden;
			if (!showFolder) showFolder = !folderInSharedPtr->hidden;
			
			if (showFolder) {
				printAllHelper(folderInSharedPtr->folders, newPath, includeHidden);
			}
		}

		if (!folderInSharedPtr->files.empty()) {
			for (auto& file : folderInSharedPtr->files) {
				const auto fileInSharedPtr = file;

				bool showFile = includeHidden && fileInSharedPtr->hidden;
				if (!showFile) showFile = !fileInSharedPtr->hidden;

				if (showFile) {
					std::cout << newPath << fileInSharedPtr->name << "\n";
				}
			}
		}

		if (folderInSharedPtr->folders.empty() && folderInSharedPtr->files.empty()) {
			bool showFolder = includeHidden && folderInSharedPtr->hidden;
			if (!showFolder) showFolder = !folderInSharedPtr->hidden;

			if (showFolder) {
				std::cout << newPath << '\n';
			}
		}
	}
}

void core::PseudoFS::printAllHelper(const std::vector<std::shared_ptr<FolderData>> &folders, const std::string &path, bool includeHidden, std::string& str) {
	std::string output;
	for (auto& folder : folders) {
		const auto folderInSharedPtr = folder;

		std::string newPath = path + folderInSharedPtr->name + "/";

		if (!folderInSharedPtr->folders.empty()) {
			bool showFolder = includeHidden && folderInSharedPtr->hidden;
			if (!showFolder) showFolder = !folderInSharedPtr->hidden;
			
			if (showFolder) {
				std::string temp;
				printAllHelper(folderInSharedPtr->folders, newPath, includeHidden, temp);
				output += temp + "\n";
			}
		}

		if (!folderInSharedPtr->files.empty()) {
			for (auto& file : folderInSharedPtr->files) {
				const auto fileInSharedPtr = file;

				bool showFile = includeHidden && fileInSharedPtr->hidden;
				if (!showFile) showFile = !fileInSharedPtr->hidden;

				if (showFile) {
					output += newPath + fileInSharedPtr->name + "\n";
				}
			}
		}

		if (folderInSharedPtr->folders.empty() && folderInSharedPtr->files.empty()) {
			bool showFolder = includeHidden && folderInSharedPtr->hidden;
			if (!showFolder) showFolder = !folderInSharedPtr->hidden;

			if (showFolder) {
				output += newPath + "\n";
			}
		}
	}
	str = output;
}

void core::PseudoFS::printAll(bool includeHidden, size_t diskId, const std::string& startPath) {
	std::vector<std::string> parsedPath = core::string_util::split(startPath, '/');
	if (parsedPath.size() < 2) {
		parsedPath = {};
		for (auto& folder : _nrfs->disks[diskId]->folders) {
			const auto folderInSharedPtr = folder;

			std::string newPath = "./" + folderInSharedPtr->name + "/";
			
			if (!folderInSharedPtr->folders.empty()) {
				bool showFolder = includeHidden && folderInSharedPtr->hidden;
				if (!showFolder) showFolder = !folderInSharedPtr->hidden;
				if (showFolder) {
					printAllHelper(folderInSharedPtr->folders, newPath, includeHidden);
				}
			}

			if (!folderInSharedPtr->files.empty()) {
				for (auto& file : folderInSharedPtr->files) {
					const auto fileInSharedPtr = file;

					bool showFile = includeHidden && fileInSharedPtr->hidden;
					if (!showFile) showFile = !fileInSharedPtr->hidden;

					if (showFile) {
						std::cout << newPath << fileInSharedPtr->name << "\n";
					}
				}
			}

			if (folderInSharedPtr->folders.empty() && folderInSharedPtr->files.empty()) {
				bool showFolder = includeHidden && folderInSharedPtr->hidden;
				if (!showFolder) showFolder = !folderInSharedPtr->hidden;

				if (showFolder) {
					std::cout << newPath << '\n';
				}
			}
		}

		for (auto& file : _nrfs->disks[diskId]->files) {
			const auto fileInSharedPtr = file;

			bool showFile = includeHidden && fileInSharedPtr->hidden;
			if (!showFile) showFile = !fileInSharedPtr->hidden;

			if (showFile) {
				std::cout << "./" << fileInSharedPtr->name << '\n';
			}
		}
	}
	else {
		parsedPath = {};
		int code = 0;
		printAllHelper(getFolderData(startPath, diskId, code).folders, startPath, includeHidden);
	}
}

void core::PseudoFS::printAll(bool includeHidden, size_t diskId, const std::string& startPath, std::string& str) {
	std::string result;
	std::vector<std::string> parsedPath = core::string_util::split(startPath, '/');
	if (parsedPath.size() < 2) {
		parsedPath = {};
		for (auto& folder : _nrfs->disks[diskId]->folders) {
			const auto folderInSharedPtr = folder;

			std::string newPath = "./" + folderInSharedPtr->name + "/";
			
			if (!folderInSharedPtr->folders.empty()) {
				bool showFolder = includeHidden && folderInSharedPtr->hidden;
				if (!showFolder) showFolder = !folderInSharedPtr->hidden;
				if (showFolder) {
					std::string temp;
					printAllHelper(folderInSharedPtr->folders, newPath, includeHidden, temp);
					result += temp + "\n";
				}
			}

			if (!folderInSharedPtr->files.empty()) {
				for (auto& file : folderInSharedPtr->files) {
					const auto fileInSharedPtr = file;

					bool showFile = includeHidden && fileInSharedPtr->hidden;
					if (!showFile) showFile = !fileInSharedPtr->hidden;

					if (showFile) {
						result += newPath + fileInSharedPtr->name + "\n";
					}
				}
			}

			if (folderInSharedPtr->folders.empty() && folderInSharedPtr->files.empty()) {
				bool showFolder = includeHidden && folderInSharedPtr->hidden;
				if (!showFolder) showFolder = !folderInSharedPtr->hidden;

				if (showFolder) {
					result += newPath + "\n";
				}
			}
		}

		for (auto& file : _nrfs->disks[diskId]->files) {
			const auto fileInSharedPtr = file;

			bool showFile = includeHidden && fileInSharedPtr->hidden;
			if (!showFile) showFile = !fileInSharedPtr->hidden;

			if (showFile) {
				result += "./" + fileInSharedPtr->name + "\n";
			}
		}
	}
	else {
		parsedPath = {};
		int code = 0;
		std::string temp;
		printAllHelper(getFolderData(startPath, diskId, code).folders, startPath, includeHidden, temp);
		result += temp;
	}

	str = result;
}

void core::PseudoFS::showTreeHelper(const FolderData &curFolder, bool includeHidden, bool showFiles, int level) {
	for (auto& folder : curFolder.folders) {
		const auto folderInSharedPtr = folder;
		bool showFolder = includeHidden && folderInSharedPtr->hidden;
		if (!showFolder) showFolder = !folderInSharedPtr->hidden;

		if (folderInSharedPtr->folders.empty() && showFolder) {
			std::cout << std::string(level * 2, ' ') << folderInSharedPtr->name << '\n';
		}
		else if (!folderInSharedPtr->folders.empty() && showFolder) {
			std::cout << std::string(level * 2, ' ') << folderInSharedPtr->name << '\n';
			showTreeHelper(*folderInSharedPtr.get(), includeHidden, showFiles, level + 1);
		}

		if (showFiles && !folderInSharedPtr->files.empty()) {
			for (auto& file : folderInSharedPtr->files) {
				const auto fileInSharedPtr = file;

				bool showFile = includeHidden && fileInSharedPtr->hidden;
				if (!showFile) showFile = !fileInSharedPtr->hidden;

				if (showFile) {
					std::cout << std::string((level + 1) * 2, ' ') << fileInSharedPtr->name << '\n';
				}
			}
		}
	}
	if (showFiles) {
		for (auto& file : curFolder.files) {
			auto fileInSharedPtr = file;

			bool showFile = includeHidden && fileInSharedPtr->hidden;
			if (!showFile) showFile = !fileInSharedPtr->hidden;

			if (showFile) {
				std::cout << std::string(level * 2, ' ') << fileInSharedPtr->name << '\n';
			}
		}
	}
}

void core::PseudoFS::showTreeHelper(const FolderData &curFolder, bool includeHidden, bool showFiles, int level, std::string& str) {
	std::string output;
	for (auto& folder : curFolder.folders) {
		const auto folderInSharedPtr = folder;

		bool showFolder = includeHidden && folderInSharedPtr->hidden;
		if (!showFolder) showFolder = !folderInSharedPtr->hidden;

		if (folderInSharedPtr->folders.empty() && showFolder) {
			output += std::string(level * 2, ' ') + folderInSharedPtr->name + "\n";
		}
		else if (!folderInSharedPtr->folders.empty() && showFolder) {
			output += std::string(level * 2, ' ') + folderInSharedPtr->name + "\n";
			std::string temp;
			showTreeHelper(*folderInSharedPtr.get(), includeHidden, showFiles, level + 1, temp);
			output += temp;
		}

		if (showFiles && !folderInSharedPtr->files.empty()) {
			for (auto& file : folderInSharedPtr->files) {
				const auto fileInSharedPtr = file;

				bool showFile = includeHidden && fileInSharedPtr->hidden;
				if (!showFile) showFile = !fileInSharedPtr->hidden;

				if (showFile) {
					output += std::string((level + 1) * 2, ' ') + fileInSharedPtr->name + "\n";
				}
			}
		}
	}
	if (showFiles) {
		for (auto& file : curFolder.files) {
			const auto fileInSharedPtr = file;

			bool showFile = includeHidden && fileInSharedPtr->hidden;
			if (!showFile) showFile = !fileInSharedPtr->hidden;

			if (showFile) {
				output += std::string(level * 2, ' ') + fileInSharedPtr->name + "\n";
			}
		}
	}
	str = output;
}

void core::PseudoFS::showTree(bool includeHidden, size_t diskId, bool showFiles, const std::string& startPath) {
	std::vector<std::string> parsedPath = core::string_util::split(startPath, '/');
	if (parsedPath.size() < 2) {
		parsedPath = {};
		int level = 0;
		for (auto& folder : _nrfs->disks[diskId]->folders) {
			const auto folderInSharedPtr = folder;

			bool showFolder = includeHidden && folderInSharedPtr->hidden;
			if (!showFolder) showFolder = !folderInSharedPtr->hidden;

			if (folderInSharedPtr->folders.empty() && showFolder) {
				std::cout << folderInSharedPtr->name << '\n';
			}
			else if (!folderInSharedPtr->folders.empty() && showFolder) {
				std::cout << folderInSharedPtr->name << '\n';
				showTreeHelper(*folderInSharedPtr.get(), includeHidden, showFiles, level + 1);
			}
		}
		if (showFiles) {
			for (auto& file : _nrfs->disks[diskId]->files) {
				const auto fileInSharedPtr = file;

				bool showFile = includeHidden && fileInSharedPtr->hidden;
				if (!showFile) showFile = !fileInSharedPtr->hidden;

				if (showFile) {
					std::cout << fileInSharedPtr->name << '\n';
				}
			}
		}
	}
	else {
		parsedPath = {};
		int code = 0;
		showTreeHelper(getFolderData(startPath, diskId, code), includeHidden, showFiles, 0);
	}
}

void core::PseudoFS::showTree(bool includeHidden, size_t diskId, bool showFiles, const std::string& startPath, std::string& str) {
	std::string result;
	std::vector<std::string> parsedPath = core::string_util::split(startPath, '/');
	if (parsedPath.size() < 2) {
		parsedPath = {};
		int level = 0;
		for (auto& folder : _nrfs->disks[diskId]->folders) {
			const auto folderInSharedPtr = folder;

			bool showFolder = includeHidden && folderInSharedPtr->hidden;
			if (!showFolder) showFolder = !folderInSharedPtr->hidden;

			if (folderInSharedPtr->folders.empty() && showFolder) {
				result += folderInSharedPtr->name + "\n";
			}
			else if (!folderInSharedPtr->folders.empty() && showFolder) {
				result += folderInSharedPtr->name + "\n";
				std::string temp;
				showTreeHelper(*folderInSharedPtr.get(), includeHidden, showFiles, level + 1, temp);
				result += temp;
			}
		}
		if (showFiles) {
			for (auto& file : _nrfs->disks[diskId]->files) {
				const auto fileInSharedPtr = file;

				bool showFile = includeHidden && fileInSharedPtr->hidden;
				if (!showFile) showFile = !fileInSharedPtr->hidden;

				if (showFile) {
					result += fileInSharedPtr->name + "\n";
				}
			}
		}
	}
	else {
		parsedPath = {};
		int code = 0;
		std::string temp;
		showTreeHelper(getFolderData(startPath, diskId, code), includeHidden, showFiles, 0, temp);
		result += temp + "\n";
	}
	str = result;
}

core::NRFS* core::PseudoFS::getNRFS() {
	return _nrfs;
}

void core::PseudoFS::init() {
	_nrfs = new NRFS();
	try {
		int code = 0;
		getFolderData("./home", 0, code);
		if (code == core::PseudoFSCodes::NOT_FOUND) {
			createFolder("./home", 0);
			setFolderAtt("./home", 0, "system", true);
			createFile("./hello.txt", 0);
			setFileAtt("./hello.txt", 0, "content", "Hello from NRC!");
			_nrfs->saveData();
		}
	}
	catch(const std::exception& e) {
		trace(core::PrintColors::red,
			"PFS Init Error: ", e.what()
		);
		#ifdef NRC_WEB
		outputLog += "PFS Init Error: " + std::string(e.what()) + "\n";
		#endif
	}
}

core::PseudoFS::~PseudoFS() {
	delete _nrfs;
}

void searchAndTransformDefaultObjectToLink(core::FolderData* curFolder, std::vector<std::string>* pathToFile, core::FileData* m) {
	if (pathToFile->size() == 1) {
		for (const auto& f : curFolder->files) {
			if (f->name == pathToFile->at(0)) {
				m->realFilePointer = f.get();
				break;
			}
		}
	}
	else {
		for (const auto& f : curFolder->folders) {
			pathToFile->erase(pathToFile->begin());

			if (f->name == pathToFile->at(0)) {
				searchAndTransformDefaultObjectToLink(f.get(), pathToFile, m);
				break;
			}
		}
	}
}

void searchAndTransformDefaultObjectToLink(core::FolderData* curFolder, std::vector<std::string>* pathToFile, core::FolderData* m) {
	bool folderExists = pathToFile->size() > 1 ? false : std::any_of(curFolder->folders.begin(), curFolder->folders.end(),
		[&](std::shared_ptr<core::FolderData> f) {
			if (f->name == pathToFile->at(0)) {
				m->realFolderPointer = f.get();
			}
			return f->name == pathToFile->at(0);
		}
	);

	if (!folderExists) {
		for (const auto& f : curFolder->folders) {
			pathToFile->erase(pathToFile->begin());

			if (f->name == pathToFile->at(0)) {
				searchAndTransformDefaultObjectToLink(f.get(), pathToFile, m);
				break;
			}
		}
	}
}

void core::PseudoFS::postInit() {
	for (size_t i = 0; i < _nrfs->disks.size(); i++) {
		for (const auto& file : _nrfs->disks[i]->files) {
			if (file->realFilePath.empty()) continue;

			std::vector<std::string> parsedPath = core::string_util::split(file->realFilePath, '/');
			
			for (const auto& folder : _nrfs->disks[i]->folders) {
				searchAndTransformDefaultObjectToLink(folder.get(), &parsedPath, file.get());
			}
		}

		for (const auto& folder : _nrfs->disks[i]->folders) {
			if (folder->realFolderPath.empty()) continue;

			std::vector<std::string> parsedPath = core::string_util::split(folder->realFolderPath, '/');
			searchAndTransformDefaultObjectToLink(folder.get(), &parsedPath, folder.get());
		}
	}
}

int core::PseudoFS::changePath(const std::string& newPath, size_t diskId) {
	if (folderExists(newPath, diskId) || newPath == "./") {
		_currentPath = newPath;
		return core::PseudoFSCodes::OK;
	}
	return core::PseudoFSCodes::NOT_FOUND;
}

int core::PseudoFS::changeDirectory(const std::string& newDirectory, size_t diskId) {
	std::vector<std::string> parsedPath = core::string_util::split(_currentPath, '/');
	if (newDirectory == ".." && parsedPath.size() > 1) {
		std::string _currentPathTemp;
		for (const std::string& pathPart : parsedPath)
			_currentPathTemp += pathPart + "/";
		_currentPath = _currentPathTemp.substr(0, _currentPathTemp.length() - (parsedPath.back().length() + 1));
		return core::PseudoFSCodes::OK;
	}
	else if (newDirectory == ".." && parsedPath.size() == 1)
		return core::PseudoFSCodes::SMALL_SIZE;
	else {
		if (folderExists(_currentPath + newDirectory, diskId)) {
			_currentPath += newDirectory + "/";
			return core::PseudoFSCodes::OK;
		}
	}
	return core::PseudoFSCodes::BAD_PATH;
}

int core::PseudoFS::changeDisk(char diskLetter) {
	size_t tempDiskId = 0;
	for (auto d : _nrfs->disks) {
		if (d->letter == diskLetter) {
			_currentDiskId = tempDiskId;
			_currentPath = "./"; // Для избежания возможных ошибок при смене текущего диска.
			break;
		}
		++tempDiskId;
	}
	return core::PseudoFSCodes::OK;
}

bool core::PseudoFS::folderExistsHelper(std::vector<std::string> path, FolderData* currentFolder) {
	if (currentFolder->folders.empty() || path.empty())
		return false;
	else {
		for (auto& folder : currentFolder->folders) {
			if (folder->name == path.at(0) && path.size() > 1) {
				path.erase(path.begin());
				return folderExistsHelper(path, folder.get());
			}
			if (path.size() == 1 && path.at(0) == folder->name) {
				return true;
			}
		}
	}
	return false;
}

bool core::PseudoFS::folderExists(const std::string& path, size_t diskId) {
	std::vector<std::string> parsedPath = core::string_util::split(path, '/');
	try {
		if (parsedPath.size() < 2)
			return false;
		if (parsedPath.size() == 2) {
			return std::any_of(_nrfs->disks[diskId]->folders.begin(), _nrfs->disks[diskId]->folders.end(), [&](std::shared_ptr<FolderData> f) { return f->name == parsedPath.at(1); });
		}
		else {
			parsedPath.erase(parsedPath.begin());
			for (auto& folder : _nrfs->disks[diskId]->folders) {
				if (parsedPath.at(0) == folder->name) {
					parsedPath.erase(parsedPath.begin());
					return folderExistsHelper(parsedPath, folder.get());
				}
			}
		}
	}
	catch(const std::exception& e) {
		trace(core::PrintColors::red,
			"folderExists Error: ", e.what(), "\n",
			"folder: ", getFolderFromPath(path),
			"path: ", path
		);
		#ifdef NRC_WEB
		outputLog += "folderExists Error: " + std::string(e.what()) + "\n" +
			"folder: " + getFolderFromPath(path) + "\n" +
			"path: " + path + "\n";
		#endif
	}
	return false;
}

bool core::PseudoFS::fileExistsHelper(std::vector<std::string> path, FolderData* currentFolder) {
	if (currentFolder->folders.empty() || path.empty())
		return false;
	else {
		for (auto& folder : currentFolder->folders) {
			if (folder->name == path.at(0) && path.size() > 1) {
				path.erase(path.begin());
				return fileExistsHelper(path, folder.get());
			}
			else if (path.size() == 1) {
				return std::any_of(currentFolder->files.begin(), currentFolder->files.end(), [&](std::shared_ptr<FileData> f) { return f->name == path.at(0); });
			}
		}
	}
	return false;
}

bool core::PseudoFS::fileExists(const std::string& path, size_t diskId) {
	std::vector<std::string> parsedPath = core::string_util::split(path, '/');
	try {
		if (parsedPath.size() < 2)
			return false;
		if (parsedPath.size() == 2) {
			return std::any_of(_nrfs->disks[diskId]->files.begin(), _nrfs->disks[diskId]->files.end(), [&](std::shared_ptr<FileData> f) { return f->name == parsedPath.at(1); });
		}
		else {
			parsedPath.erase(parsedPath.begin());
			for (auto& folder : _nrfs->disks[diskId]->folders) {
				if (parsedPath.at(0) == folder->name) {
					parsedPath.erase(parsedPath.begin());
					return fileExistsHelper(parsedPath, folder.get());
				}
			}
		}
	}
	catch(const std::exception& e) {
		trace(core::PrintColors::red,
			"fileExists Error: ", e.what(), "\n",
			"folder: ", getFolderFromPath(path), "\n",
			"target file", getFileFromPath(path), "\n", 
			"path: ", path
		);
		#ifdef NRC_WEB
		outputLog += "folderExists Error: " + std::string(e.what()) + "\n" +
			"folder: " + getFolderFromPath(path) + "\n" +
			"path: " + path + "\n";
		#endif
	}
	return false;
}

const std::string& core::PseudoFS::currentPath() {
	return _currentPath;
}

std::string core::PseudoFS::getFileFromPath(const std::string& path) {
	size_t index = path.find_last_of("/");
	if (index >= path.length() && path.find_last_of(".") < 1) return "";
	return path.substr(index + 1, path.length());
}

std::string core::PseudoFS::getFolderFromPath(const std::string& path) {
	size_t index = path.find_last_of("/");
	if (index >= path.length()) return "";
	return path.substr(index + 1, path.length());
}