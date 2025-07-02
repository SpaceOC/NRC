#include <nlohmann/json.hpp>

#include "Core/modules/module_base.h"
#include "Core/modules/module_metadata.h"
#include "Core/command/handler_commands.h"
#include "Core/filesystem/pseudo_fs.h"
#include "Core/users/user_manager.h"
#include "Core/users/user.h"
#include "Core/utils/language_util.h"
#include "Core/utils/other_util.h"

core::ModuleBase::ModuleBase(ModuleMetadata* meta, HandlerCommands* hc, PseudoFS* pfs, UserManager* um) : hcPtr(hc), pfsPtr(pfs), umPtr(um), metadata(meta) {
	path = "modules/" + meta->name;

	std::string lc = um ? language_util::getLC(um->currentUserData().getLanguage()) : "en_US";
	std::string rawTranslateData = core::other_util::getFileContent(path + "/langs/" + lc + ".json");
	nlohmann::json td = nlohmann::json::parse(rawTranslateData);
	currentLanguage = td.get<std::map<std::string, std::string>>();
}