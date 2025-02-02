#include "Core/base/users/permissions_enum_class.h"

std::string core::permissionsS(core::Permissions permissions) {
    std::vector<std::string> temp = {"Ghost", "User", "Admin", "Root"};
    return temp[1 + permissions];
}