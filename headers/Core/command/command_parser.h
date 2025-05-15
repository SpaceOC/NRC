#ifndef NRC_BASE_COMMAND_COMAND_PARSER_H_
#define NRC_BASE_COMMAND_COMAND_PARSER_H_

#include <utility>
#include <vector>
#include <string>

namespace core {
	struct CommandObject;
	class User;

	class CommandParser {
	public:
		CommandParser();
		virtual ~CommandParser();
		[[nodiscard]] virtual std::vector<CommandObject> parse(const std::string& raw, bool parseVars = false, core::User* who = nullptr);
	private:
		[[nodiscard]] virtual CommandObject* __parseSingleCommand(const std::string& raw, size_t startIndex, bool parseVars, core::User* who);
		virtual std::pair<std::string, std::string> __getMapArgs(const std::string& targetText);
		[[nodiscard]] virtual std::string __getStringInQ(const std::string& original, size_t startIndex, size_t& end);
		[[nodiscard]] virtual bool __setReturnableOnSingleCommand(CommandObject* target, const std::string& raw, size_t startIndex);

		bool __isCommandSeparator(const std::string& original, size_t startIndex);
	};
}

#endif