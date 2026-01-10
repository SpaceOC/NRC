#ifndef NRC_BASE_COMMAND_COMAND_PARSER_H_
#define NRC_BASE_COMMAND_COMAND_PARSER_H_

#include <utility>
#include <vector>
#include <string>
#include <map>

namespace core {
	struct CommandObject;
	class User;

	class CommandParser {
	public:
		CommandParser();
		virtual ~CommandParser();
		[[nodiscard]] virtual std::vector<CommandObject> parse(const std::string& raw, bool parseVars = false, core::User* who = nullptr);
		[[nodiscard]] static std::map<std::string, std::string> getMapArgsFromVector(const std::vector<std::string>& args, std::string def = "--");
	private:
		[[nodiscard]] virtual CommandObject* parseSingleCommand(const std::string& raw, size_t startIndex, bool parseVars, core::User* who);
		[[nodiscard]] virtual std::string getStringInQ(const std::string& original, size_t startIndex, size_t& end);
		[[nodiscard]] virtual bool setReturnableOnSingleCommand(CommandObject* target, const std::string& raw, size_t startIndex);

		[[nodiscard]] bool isCommandSeparator(const std::string& original, size_t startIndex);
	};
}

#endif