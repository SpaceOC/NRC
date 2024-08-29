#include "Core/base/users/permissions_enum_class.h"

std::string core::permissionsS(core::permissionsEC permissions) {
    std::vector<std::string> temp = {"Ghost", "User", "Admin"};
    return temp[1 + static_cast<int>(permissions)];
}