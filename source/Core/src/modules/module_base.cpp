#include <nlohmann/json.hpp>

#include "Core/modules/module_base.h"
#include "Core/modules/module_metadata.h"
#include "Core/command/handler_commands.h"
#include "Core/filesystem/pseudo_fs.h"
#include "Core/users/user_manager.h"
#include "Core/users/user.h"
#include "Core/other/utils.h"

core::ModuleBase::ModuleBase(ModuleMetadata* meta, HandlerCommands* hc, PseudoFS* pfs, UserManager* um) : metadata(meta), hcPtr(hc), pfsPtr(pfs), umPtr(um) {
	path = "modules/" + meta->name;

	// TODO: Добавить функцию getLCFromLanguageName в Utils (наверное)
	std::string lc = (um ? um->currentUserData().getLanguage() : "en-US");
	std::string rawTranslateData = core::Utils::getFileContent(path + "/langs/" + lc + ".json");
	nlohmann::json td = nlohmann::json::parse(rawTranslateData);
	currentLanguage = td.get<std::map<std::string, std::string>>();
}