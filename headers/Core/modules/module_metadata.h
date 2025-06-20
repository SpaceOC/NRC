#include <string>
#include <vector>
#include <map>
#include "other/versionc.h"

namespace core {

struct ModuleMetadata {
	std::string name;
	std::string description;
	std::vector<std::string> targets;
	VersionC uses;
	std::map<std::string, std::string> layout;
	std::vector<std::string> translations;
};

};