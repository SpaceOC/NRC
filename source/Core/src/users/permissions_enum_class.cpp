#include "Core/users/user_permissions_enum.h"

std::string core::userPermissionsS(const core::UserPermissions& permissions) {
	std::vector<std::string> temp = {"Ghost", "User", "Admin", "Root"};
	return temp[1 + permissions];
}