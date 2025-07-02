#include "Core/print/print.h"
#include "Core/users/user_permissions_enum.h"
#include "Core/experimental/event_manager.h"

core::structDataEvents::UserAddEvent newUserEvent(core::structDataEvents::UserAddEvent data) {
	core::print("New user!\n");
	core::print("Username - " + data.username + "\n");
	return data;
}

core::structDataEvents::UserChangeEvent changeUserEvent(core::structDataEvents::UserChangeEvent data) {
	core::print("Old username - " + data.oldUsername + "\n");
	core::print("New username - " + data.username + "\n");
	core::print("Old permissions - " + core::userPermissionsS(data.oldPermissions) + "\n");
	core::print("New permissions - " + core::userPermissionsS(data.permissions) + "\n");
	return data;
}

core::structDataEvents::UserDeleteEvent deleteUserEvent(core::structDataEvents::UserDeleteEvent data) {
	core::print("Bye...\n");
	core::print("Username - " + data.username + "\n");
	return data;
}

core::structDataEvents::NRCShutdownEvent goodbyeNRC(core::structDataEvents::NRCShutdownEvent data) {
	core::print("Username - " + data.username + "\n");
	core::print("Permissions - " + core::userPermissionsS(data.permissions) + "\n");
	core::print("Vector pos - " + std::to_string(data.id) + "\n");
	return data;
}

void addEvents() {
	core::EventManager::addEvent(newUserEvent);
	core::EventManager::addEvent(changeUserEvent);
	core::EventManager::addEvent(deleteUserEvent);
	core::EventManager::addEvent(goodbyeNRC);
}