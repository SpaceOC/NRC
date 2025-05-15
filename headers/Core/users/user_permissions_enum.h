#ifndef NRC_BASE_USERS_PERMISSIONS_ENUM_CLASS_H_
#define NRC_BASE_USERS_PERMISSIONS_ENUM_CLASS_H_
#include <string>
#include <vector>

namespace core {
	enum UserPermissions {
		GHOST = -1,
		USER,
		ADMIN,
		ROOT
	};

	// Returns a name depending on the argument.
	std::string userPermissionsS(const core::UserPermissions& permissions);
}
#endif