#include <nlohmann/json.hpp>
#include "Core/modules/module_base.h"
#include "Core/modules/module_metadata.h"
#include "Core/users/user_manager.h"
#include "Core/users/user.h"
#include "Core/utils/language_util.h"
#include "Core/utils/other_util.h"
#include "Core/main.h"

core::ModuleBase::ModuleBase(ModuleMetadata* meta, main* core) : path("modules/" + meta->name), metadata(meta), core(core){
	std::string lc = language_util::getLC(userManager()->currentUserData().getLanguage());
	std::string rawTranslateData = core::other_util::getFileContent(path + "/langs/" + lc + ".json");
	nlohmann::json td = nlohmann::json::parse(rawTranslateData);
	currentLanguage = td.get<std::map<std::string, std::string>>();
}

void* core::ModuleBase::requestPtrClass(const std::string& request) {
	return core->getRequiredClassPtr(request, this);
}

core::ModuleMetadata core::ModuleBase::getMetadata() {
	return *metadata;
}