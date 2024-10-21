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

#include "Core/experimental/event_manager.h"
#include "Core/base/print.h"

void core::EventManager::eventsStart(std::string name, std::any event) {
    if (!enableEvents) return;
    if (name == USER_ADD_EVENT) {
        core::structDataEvents::UserAddEvent ev = std::any_cast<core::structDataEvents::UserAddEvent>(event);
        for (auto func : userAddEvents) {
            func(ev);
        }
    }
    else if (name == USER_DELETE_EVENT) {
        core::structDataEvents::UserDeleteEvent ev = std::any_cast<core::structDataEvents::UserDeleteEvent>(event);
        for (auto func : userDeleteEvents) {
            func(ev);
        }
    }
    else if (name == USER_CHANGE_EVENT) {
        core::structDataEvents::UserChangeEvent ev = std::any_cast<core::structDataEvents::UserChangeEvent>(event);
        for (auto func : userChangeEvents) {
            func(ev);
        }
    }
    else if (name == NRC_SHUTDOWN_EVENT) {
        core::structDataEvents::NRCShutdownEvent ev = std::any_cast<core::structDataEvents::NRCShutdownEvent>(event);
        for (auto func : NRCShutdownEvents) {
            func(ev);
        }
    }
}

void core::EventManager::addEvent(std::function<core::structDataEvents::UserAddEvent(core::structDataEvents::UserAddEvent)> func) {
    userAddEvents.push_back(func);
}

void core::EventManager::addEvent(std::function<core::structDataEvents::UserChangeEvent(core::structDataEvents::UserChangeEvent)> func) {
    userChangeEvents.push_back(func);
}

void core::EventManager::addEvent(std::function<core::structDataEvents::UserDeleteEvent(core::structDataEvents::UserDeleteEvent)> func) {
    userDeleteEvents.push_back(func);
}

void core::EventManager::addEvent(std::function<core::structDataEvents::NRCShutdownEvent(core::structDataEvents::NRCShutdownEvent)> func) {
    NRCShutdownEvents.push_back(func);
}