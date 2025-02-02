#ifndef NRC_BASE_USERS_PERMISSIONS_ENUM_CLASS_H_
#define NRC_BASE_USERS_PERMISSIONS_ENUM_CLASS_H_
#include <string>
#include <vector>

namespace core {
    enum Permissions {
        ghost = -1,
        user,
        admin,
        root
    };

    // Returns a name depending on the argument.
    std::string permissionsS(core::Permissions permissions);
}
#endif