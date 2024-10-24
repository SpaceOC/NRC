#include "Core/base/users/permissions_enum_class.h"

std::string core::permissionsS(core::Permissions permissions) {
    std::vector<std::string> temp = {"Ghost", "User", "Admin"};
    return temp[1 + permissions];
}