/*
    Copyright (C) 2024-2024  SpaceOC

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#ifndef NRC_BASE_USERS_PERMISSIONS_ENUM_CLASS_H_
#define NRC_BASE_USERS_PERMISSIONS_ENUM_CLASS_H_
#include <string>
#include <vector>

namespace core {
    enum Permissions {
        ghost = -1,
        user,
        admin
    };

    // Returns a name depending on the argument.
    std::string permissionsS(Permissions permissions);
}
#endif