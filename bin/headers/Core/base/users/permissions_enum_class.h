// permissions enum class header
// by SpaceOC
#pragma once
#ifndef NRC_BASE_USERS_PERMISSIONS_ENUM_CLASS_H_
#define NRC_BASE_USERS_PERMISSIONS_ENUM_CLASS_H_
#include <string>
#include <vector>

enum class permissionsEC {
    ghost = -1,
    user,
    admin
};

std::string permissionsS(permissionsEC permissions);
#endif