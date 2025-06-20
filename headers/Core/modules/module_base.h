#include <string>
#include <memory>
#include <map>

namespace core {
	struct ModuleMetadata;
	class HandlerCommands;
	class PseudoFS;
	class UserManager;

class ModuleBase {
	public:
		explicit ModuleBase(ModuleMetadata* meta, HandlerCommands* hc, PseudoFS* pfs, UserManager* um);
		virtual ~ModuleBase() = 0;
		ModuleBase(const ModuleBase &) = delete;
		ModuleBase(ModuleBase &&) = delete;
		ModuleBase &operator=(const ModuleBase &) = delete;
		ModuleBase &operator=(ModuleBase &&) = delete;

		virtual bool runRequest(const std::string &request) = 0;
		virtual void onLoad() = 0;
		virtual void onUnload() = 0;
		virtual void onWrongRequest() = 0;
	private:
		HandlerCommands* hcPtr;
		PseudoFS* pfsPtr;
		UserManager* umPtr;
		std::string path;
		std::map<std::string, std::string> currentLanguage;
		std::unique_ptr<ModuleMetadata> metadata;
};

};