#include "Core/base/print.h"
#include "Core/base/users/permissions_enum_class.h"
#include "Core/experimental/event_manager.h"

core::structDataEvents::UserAddEvent newUserEvent(core::structDataEvents::UserAddEvent data) {
    core::print("OMG!!! New user!!!\n");
    core::print("Username - " + data.username + "\n");
    return data;
}

core::structDataEvents::UserChangeEvent changeUserEvent(core::structDataEvents::UserChangeEvent data) {
    core::print("Old username - " + data.oldUsername + "\n");
    core::print("New username - " + data.username + "\n");
    core::print("Old permissions - " + core::permissionsS(data.oldPermissions) + "\n");
    core::print("New permissions - " + core::permissionsS(data.permissions) + "\n");
    return data;
}

core::structDataEvents::UserDeleteEvent deleteUserEvent(core::structDataEvents::UserDeleteEvent data) {
    core::print("Bye...\n");
    core::print("Username - " + data.username + "\n");
    return data;
}

core::structDataEvents::NRCShutdownEvent goodbyeNRC(core::structDataEvents::NRCShutdownEvent data) {
    core::print("Username - " + data.username + "\n");
    core::print("Permissions - " + core::permissionsS(data.permissions) + "\n");
    core::print("Vector pos - " + std::to_string(data.id) + "\n");
    return data;
}

void addEvents() {
    core::EventManager::addEvent(newUserEvent);
    core::EventManager::addEvent(changeUserEvent);
    core::EventManager::addEvent(deleteUserEvent);
    core::EventManager::addEvent(goodbyeNRC);
}