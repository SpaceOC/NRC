#include "Core/print/print.h"
#include "Core/users/user_permissions_enum.h"
#include "Core/experimental/event_manager.h"

core::experimental::structDataEvents::UserAddEvent newUserEvent(core::experimental::structDataEvents::UserAddEvent data) {
	core::print("New user!\n");
	core::print("Username - " + data.username + "\n");
	return data;
}

core::experimental::structDataEvents::UserChangeEvent changeUserEvent(core::experimental::structDataEvents::UserChangeEvent data) {
	core::print("Old username - " + data.oldUsername + "\n");
	core::print("New username - " + data.username + "\n");
	core::print("Old permissions - " + core::userPermissionsS(data.oldPermissions) + "\n");
	core::print("New permissions - " + core::userPermissionsS(data.permissions) + "\n");
	return data;
}

core::experimental::structDataEvents::UserDeleteEvent deleteUserEvent(core::experimental::structDataEvents::UserDeleteEvent data) {
	core::print("Bye...\n");
	core::print("Username - " + data.username + "\n");
	return data;
}

core::experimental::structDataEvents::NRCShutdownEvent goodbyeNRC(core::experimental::structDataEvents::NRCShutdownEvent data) {
	core::print("Username - " + data.username + "\n");
	core::print("Permissions - " + core::userPermissionsS(data.permissions) + "\n");
	core::print("Vector pos - " + std::to_string(data.id) + "\n");
	return data;
}

void addEvents() {
	core::experimental::EventManager::addEvent(newUserEvent);
	core::experimental::EventManager::addEvent(changeUserEvent);
	core::experimental::EventManager::addEvent(deleteUserEvent);
	core::experimental::EventManager::addEvent(goodbyeNRC);
}