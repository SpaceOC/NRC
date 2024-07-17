#include "Core/users/permissions_enum_class.h"

std::string permissionsS(permissionsEC permissions) {
    std::vector<std::string> temp = {"Ghost", "User", "Admin"};
    return temp[1 + static_cast<int>(permissions)];
}