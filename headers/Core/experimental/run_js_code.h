// is not finished!
#ifndef NRC_EXPERIMENTAL_RUN_JS_CODE_H_
#define NRC_EXPERIMENTAL_RUN_JS_CODE_H_

#include <string>
#include "Core/users/user_permissions_enum.h"

namespace core {
	class User;
}

namespace core_experimental {
	void runCode(const std::string& code, core::User* user);
	void runCode(const std::string& code, const core::UserPermissions& permissions);
	void runCode(const std::string& code, core::User* user, std::string& str);
	void runCode(const std::string& code, const core::UserPermissions& permissions, std::string& str);
}

#endif