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
#ifndef NRC_EXPERIMENTAL_EVENT_MANAGER_H_
#define NRC_EXPERIMENTAL_EVENT_MANAGER_H_

#define USER_DELETE_EVENT "UDE"
#define USER_ADD_EVENT "UAE"
#define USER_CHANGE_EVENT "UCE"
#define NRC_SHUTDOWN_EVENT "NRC-S"

#include <string>
#include <map>
#include <vector>
#include <functional>
#include <any>
#include "Core/base/users/permissions_enum_class.h"

namespace core {
    namespace structDataEvents {
        // Stores data (username, display name, permissions, position in users vector) of a newly created user.
        struct UserAddEvent {
            std::string username; // Username of the user (who has just been created in the NRC)
            std::string displayName; // Displayed name of the user (who was just created in the NRC)
            Permissions permissions; // Permissions of the user (who has just been created in the NRC)
            size_t id; // ID (position in the users vector) of the user (who has just been created in the NRC)

            //bool cancelled;
        };

        // Stores some new and old data (username, display name, permissions) of the user as well as their position in the users vector.
        struct UserChangeEvent {
            std::string oldUsername; // The user's old username.
            std::string username; // The user's new username.
            std::string oldDisplayName; // The old display username.
            std::string displayName; // New displayed user name.
            Permissions oldPermissions; // Old user permissions.
            Permissions permissions; // New user permissions.
            size_t id; // ID (position in the users vector) of the user.

            //bool cancelled;
        };

        // Contains data (username, display name, permissions) about the deleted (from NRC) user.
        struct UserDeleteEvent {
            std::string username; // Username of the user.
            std::string displayName; // Displayed name of the user.
            Permissions permissions; // Permissions of the user.

            //bool cancelled;
        };

        // Contains data about the current user who started the Pseudo-OS (and NRC) shutdown process.
        struct NRCShutdownEvent {
            std::string username; // Username of the user.
            Permissions permissions; // Permissions of the user.
            size_t id; // ID (position in the users vector) of the user.
        };
    };

    class EventManager {
        private:
            static inline std::vector<std::function<core::structDataEvents::UserAddEvent(core::structDataEvents::UserAddEvent)>> userAddEvents;
            static inline std::vector<std::function<core::structDataEvents::UserDeleteEvent(core::structDataEvents::UserDeleteEvent)>> userDeleteEvents;
            static inline std::vector<std::function<core::structDataEvents::UserChangeEvent(core::structDataEvents::UserChangeEvent)>> userChangeEvents;
            static inline std::vector<std::function<core::structDataEvents::NRCShutdownEvent(core::structDataEvents::NRCShutdownEvent)>> NRCShutdownEvents;
        public:
            static inline bool enableEvents;

            // Starts all functions of a certain event.
            static void eventsStart(std::string name, std::any event);
            //auto eventStart(std::string name, std::any event, size_t id);

            static void addEvent(std::function<core::structDataEvents::UserAddEvent(core::structDataEvents::UserAddEvent)> func);
            static void addEvent(std::function<core::structDataEvents::UserChangeEvent(core::structDataEvents::UserChangeEvent)> func);
            static void addEvent(std::function<core::structDataEvents::UserDeleteEvent(core::structDataEvents::UserDeleteEvent)> func);
            static void addEvent(std::function<core::structDataEvents::NRCShutdownEvent(core::structDataEvents::NRCShutdownEvent)> func);
    };

};

#endif