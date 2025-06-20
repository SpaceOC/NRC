#include "Core/filesystem/nrfs.h"
#include <iostream>
#include "Core/print/print.h"
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include "Core/other/utils.h"

core::FileData* createFileFromJSON(const nlohmann::json& fileJson) {
	core::FileData* result;

	std::string fileName = fileJson.at(0).get<std::string>();
	std::string fileContent = fileJson.at(1).get<std::string>();
	time_t fileTimeCreate = fileJson.at(2).get<time_t>();
	time_t fileTimeEdit = fileJson.at(3).get<time_t>();
	bool isSystemFile = fileJson.at(4).get<bool>();
	bool isHiddenFile = fileJson.at(5).get<bool>();
	std::string fileLinkPath = fileJson.at(6).get<std::string>();
	std::string fileOwnerUsername = fileJson.at(7).get<std::string>();

	result = new core::FileData{
		fileName,
		fileContent,
		fileTimeCreate,
		fileTimeEdit,
		isSystemFile,
		isHiddenFile,
		nullptr,
		fileLinkPath,
		nullptr,
		fileOwnerUsername
	};

	return result;
}

core::FolderData* createFolderFromJSON(const nlohmann::json& folderJson) {
	core::FolderData* result;

	std::string folderName = folderJson.at(0).get<std::string>();
	time_t folderTimeCreate = folderJson.at(1).get<time_t>();
	time_t folderTimeEdit = folderJson.at(2).get<time_t>();
	bool isSystemFolder = folderJson.at(3).get<bool>();
	bool isHiddenFolder = folderJson.at(4).get<bool>();
	std::string folderLinkPath = folderJson.at(5).get<std::string>();
	std::string folderOwnerUsername = folderJson.at(6).get<std::string>();

	std::vector<std::shared_ptr<core::FileData>> files;
	std::vector<std::shared_ptr<core::FolderData>> folders;
	if (folderJson.contains("objects") && !folderJson["objects"].is_null()) {
		for (const auto& j : folderJson["objects"]) {
			std::string jsonType = j["type"].get<std::string>();
			if (jsonType == "file") {
				core::FileData* f = createFileFromJSON(j["data"]);

				std::shared_ptr<core::FileData> fileInSharedPtr(std::move(f));

				files.push_back(std::move(fileInSharedPtr));
			}
			else if (jsonType == "folder") {
				core::FolderData* f = createFolderFromJSON(j["data"]);

				std::shared_ptr<core::FolderData> folderInSharedPtr(std::move(f));

				folders.push_back(std::move(folderInSharedPtr));
			}
		}
	}

	result = new core::FolderData{
		folderName,
		folderTimeCreate,
		folderTimeEdit,
		std::move(files),
		std::move(folders),
		isSystemFolder,
		isHiddenFolder,
		nullptr,
		folderLinkPath,
		nullptr,
		folderOwnerUsername
	};

	return result;
}

nlohmann::json core::FileData::buildJSON() {
	nlohmann::json j;

	j["type"] = "file";
	j["data"] = {};
	j["data"].push_back(this->name);
	j["data"].push_back(this->content);
	j["data"].push_back(this->timeCreate);
	j["data"].push_back(this->timeEdit);
	j["data"].push_back(this->system);
	j["data"].push_back(this->hidden);
	j["data"].push_back(this->linkPath);
	j["data"].push_back((this->owner ? this->owner->getUsername() : ""));

	return j;
}

nlohmann::json core::FolderData::buildJSON() {
	nlohmann::json j;

	j["type"] = "folder";
	j["data"] = {};
	j["data"].push_back(this->name);
	j["data"].push_back(this->timeCreate);
	j["data"].push_back(this->timeEdit);
	j["data"].push_back(this->system);
	j["data"].push_back(this->hidden);
	j["data"].push_back(this->linkPath);
	j["data"].push_back((this->owner ? this->owner->getUsername() : ""));

	j["objects"] = {};
	for (const auto& folder : this->folders) {
		nlohmann::json folderJson = folder->buildJSON();
		j["objects"].push_back(folderJson);
	}

	for (const auto& file : this->files) {
		nlohmann::json fileJson = file->buildJSON();
		j["objects"].push_back(fileJson);
	}

	return j;
}

nlohmann::json core::NRFSDisk::buildJSON() {
	nlohmann::json j;

	std::string letterStr = "";
	letterStr.push_back(this->letter);

	j["letter"] = letterStr;
	j["name"] = this->name;

	j["objects"] = {};
	for (const auto& folder : this->folders) {
		nlohmann::json folderJson = folder->buildJSON();
		j["objects"].push_back(folderJson);
	}

	for (const auto& file : this->files) {
		nlohmann::json fileJson = file->buildJSON();
		j["objects"].push_back(fileJson);
	}

	return j;
}

void core::NRFSDisk::loadData(const nlohmann::json& j) {
	this->letter = j["letter"].get<std::string>()[0];
	this->name = j["name"].get<std::string>();

	if (j.contains("objects") && !j["objects"].is_null()) {
		for (const auto& fileJson : j["objects"]) {
			std::string jsonType = fileJson["type"].get<std::string>();
			if (jsonType != "file") continue;
			
			core::FileData* f = createFileFromJSON(fileJson["data"]);

			std::shared_ptr<core::FileData> fileInSharedPtr(std::move(f));

			this->files.push_back(std::move(fileInSharedPtr));
		}

		for (const auto& folderJson : j["objects"]) {
			std::string jsonType = folderJson["type"].get<std::string>();
			if (jsonType != "folder") continue;
			
			core::FolderData* f = createFolderFromJSON(folderJson["data"]);

			std::shared_ptr<core::FolderData> folderInSharedPtr(std::move(f));

			this->folders.push_back(std::move(folderInSharedPtr));
		}
	}
}

void core::NRFS::saveData() {
	nlohmann::json j;
	
	for (const auto& disk : this->disks) {
		nlohmann::json diskJson = disk->buildJSON();
		j.push_back(diskJson);
	}

	std::ofstream pfsData("Data/PFS-Data.json");
	//pfsData << j.dump(1);
	pfsData << j;
	pfsData.close();
}

void core::NRFS::loadData() {
	if (!std::filesystem::exists("Data/PFS-Data.json")) {
		std::ofstream pfsData("Data/PFS-Data.json");
		pfsData.close();
		this->root = new NRFSDisk();
		this->root->letter = 'C';
		this->root->update();

		std::shared_ptr<core::NRFSDisk> rootInSharedPtr(std::move(root));
		this->disks.push_back(std::move(rootInSharedPtr));

		isLoadedFromFile = false;
		return;
	}

	std::string data = core::Utils::getFileContent("Data/PFS-Data.json");
	nlohmann::json j = nlohmann::json::parse(data);
	if (j.is_array()) {
		for (const auto& diskJson : j) {
			NRFSDisk* disk = new NRFSDisk();
			disk->loadData(diskJson);

			if (disk->letter == 'C') {
				this->root = std::move(disk);
			}
			
			std::shared_ptr<core::NRFSDisk> diskInSharedPtr((disk->letter == 'C' ? disk : std::move(disk)));
			this->disks.push_back(std::move(diskInSharedPtr));
		}
	}

	isLoadedFromFile = true;
}

int core::NRFSDisk::updateHelper(const std::vector<std::shared_ptr<FolderData>>& folders) {
	int temp = 0;
	for (const std::shared_ptr<FolderData>& folder : folders) {
		foldersSize++;
		filesSize += folder->files.size() - (folder->files.size() == 1 ? 0 : 1);
		temp += folder->files.size() - (folder->files.size() == 1 ? 0 : 1);
		temp += updateHelper(folder->folders);
	}
	return temp;
}

void core::NRFSDisk::update() {
	filesSize = 0;
	foldersSize = 0;
	diskSize = 0;
	if (folders.empty() && files.empty()) return;
	if (files.size() > 0) filesSize += files.size() - (files.size() == 1 ? 0 : 1);
	this->diskSize = filesSize;
	this->diskSize += this->updateHelper(folders);
}

core::NRFS::NRFS() {
	/*
	this->root = new NRFSDisk();
	this->root->letter = 'C';
	this->root->update();
	this->disks.push_back(root);
	*/
	loadData();
}

core::NRFSDisk* core::NRFS::getRoot() {
	return root;
}

void core::NRFS::createDisk(char c) {
	for (auto d : disks) {
		if (d->letter == c)
			return;
	}
	std::shared_ptr<core::NRFSDisk> disk(new NRFSDisk());
	disk->letter = c;
	disk->update();
	disks.push_back(std::move(disk));
};

void core::NRFS::renameDisk(char c, const std::string& name) {
	for (auto d : disks) {
		if (d->letter == c) {
			d->name = name;
			break;
		}
	}
}

void core::NRFS::deleteDisk(char c) {
	auto it = disks.begin();
	for (auto d : disks) {
		if (d->letter == c) {
			disks.erase(it);
			break;
		}
		else
			++it;
	}
}

size_t core::NRFS::getDiskFromLetter(char c) {
	if (disks.size() < 2) return 1001;
	for (size_t i = 0; i < disks.size(); i++) {
		if (disks[i]->letter == c)
			return i;
	}
	return 1001;
}
