#pragma once
#ifndef PERMISSIONS_ENUM_CLASS
#define PERMISSIONS_ENUM_CLASS
#include <string>
#include <vector>

enum class permissionsEC {
    ghost = -1,
    user,
    admin
};

std::string permissionsS(permissionsEC permissions);
#endif