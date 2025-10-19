#ifndef NRC_DISABLE_EXPERIMENTAL_FEATURES
#include "Core/experimental/event_manager.h"
#include "Core/print/print.h"

void core::experimental::EventManager::eventsStart(int eventId, std::any event) {
	if (!enableEvents) return;

	if (eventId == USER_ADD_EVENT) {
		core::experimental::structDataEvents::UserAddEvent ev = std::any_cast<core::experimental::structDataEvents::UserAddEvent>(event);
		for (auto func : userAddEvents) {
			func(ev);
		}
	}
	else if (eventId == USER_DELETE_EVENT) {
		core::experimental::structDataEvents::UserDeleteEvent ev = std::any_cast<core::experimental::structDataEvents::UserDeleteEvent>(event);
		for (auto func : userDeleteEvents) {
			func(ev);
		}
	}
	else if (eventId == USER_CHANGE_EVENT) {
		core::experimental::structDataEvents::UserChangeEvent ev = std::any_cast<core::experimental::structDataEvents::UserChangeEvent>(event);
		for (auto func : userChangeEvents) {
			func(ev);
		}
	}
	else if (eventId == NRC_SHUTDOWN_EVENT) {
		core::experimental::structDataEvents::NRCShutdownEvent ev = std::any_cast<core::experimental::structDataEvents::NRCShutdownEvent>(event);
		for (auto func : NRCShutdownEvents) {
			func(ev);
		}
	}
	else if (eventId == PFS_INIT_EVENT) {
		core::experimental::structDataEvents::PFSInit ev = std::any_cast<core::experimental::structDataEvents::PFSInit>(event);
		for (auto func : pseudoFSInitEvents) {
			func(ev);
		}
	}
	else if (eventId == PFS_POST_INIT_EVENT) {
		core::experimental::structDataEvents::PFSPostInit ev = std::any_cast<core::experimental::structDataEvents::PFSPostInit>(event);
		for (auto func : pseudoFSPostInitEvents) {
			func(ev);
		}
	}
}

void core::experimental::EventManager::addEvent(std::function<core::experimental::structDataEvents::UserAddEvent(core::experimental::structDataEvents::UserAddEvent)> func) {
	userAddEvents.push_back(func);
}

void core::experimental::EventManager::addEvent(std::function<core::experimental::structDataEvents::UserChangeEvent(core::experimental::structDataEvents::UserChangeEvent)> func) {
	userChangeEvents.push_back(func);
}

void core::experimental::EventManager::addEvent(std::function<core::experimental::structDataEvents::UserDeleteEvent(core::experimental::structDataEvents::UserDeleteEvent)> func) {
	userDeleteEvents.push_back(func);
}

void core::experimental::EventManager::addEvent(std::function<core::experimental::structDataEvents::NRCShutdownEvent(core::experimental::structDataEvents::NRCShutdownEvent)> func) {
	NRCShutdownEvents.push_back(func);
}

void core::experimental::EventManager::addEvent(std::function<core::experimental::structDataEvents::PFSInit(core::experimental::structDataEvents::PFSInit)> func){
	pseudoFSInitEvents.push_back(func);
}

void core::experimental::EventManager::addEvent(std::function<core::experimental::structDataEvents::PFSPostInit(core::experimental::structDataEvents::PFSPostInit)> func) {
	pseudoFSPostInitEvents.push_back(func);
}
#endif