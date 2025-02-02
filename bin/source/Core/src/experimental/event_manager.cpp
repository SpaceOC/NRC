#include "Core/experimental/event_manager.h"
#include "Core/base/print.h"

void core::EventManager::eventsStart(int eventId, std::any event) {
    if (!enableEvents) return;

    if (eventId == USER_ADD_EVENT) {
        core::structDataEvents::UserAddEvent ev = std::any_cast<core::structDataEvents::UserAddEvent>(event);
        for (auto func : userAddEvents) {
            func(ev);
        }
    }
    else if (eventId == USER_DELETE_EVENT) {
        core::structDataEvents::UserDeleteEvent ev = std::any_cast<core::structDataEvents::UserDeleteEvent>(event);
        for (auto func : userDeleteEvents) {
            func(ev);
        }
    }
    else if (eventId == USER_CHANGE_EVENT) {
        core::structDataEvents::UserChangeEvent ev = std::any_cast<core::structDataEvents::UserChangeEvent>(event);
        for (auto func : userChangeEvents) {
            func(ev);
        }
    }
    else if (eventId == NRC_SHUTDOWN_EVENT) {
        core::structDataEvents::NRCShutdownEvent ev = std::any_cast<core::structDataEvents::NRCShutdownEvent>(event);
        for (auto func : NRCShutdownEvents) {
            func(ev);
        }
    }
    else if (eventId == PFS_INIT_EVENT) {
        core::structDataEvents::PFSInit ev = std::any_cast<core::structDataEvents::PFSInit>(event);
        for (auto func : pseudoFSInitEvents) {
            func(ev);
        }
    }
    else if (eventId == PFS_POST_INIT_EVENT) {
        core::structDataEvents::PFSPostInit ev = std::any_cast<core::structDataEvents::PFSPostInit>(event);
        for (auto func : pseudoFSPostInitEvents) {
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

void core::EventManager::addEvent(std::function<core::structDataEvents::PFSInit(core::structDataEvents::PFSInit)> func){
    pseudoFSInitEvents.push_back(func);
}

void core::EventManager::addEvent(std::function<core::structDataEvents::PFSPostInit(core::structDataEvents::PFSPostInit)> func) {
    pseudoFSPostInitEvents.push_back(func);
}