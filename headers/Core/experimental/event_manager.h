#ifndef NRC_EXPERIMENTAL_EVENT_MANAGER_H_
#define NRC_EXPERIMENTAL_EVENT_MANAGER_H_

#define USER_DELETE_EVENT 0
#define USER_ADD_EVENT 1
#define USER_CHANGE_EVENT 2
#define NRC_SHUTDOWN_EVENT 3
#define PFS_INIT_EVENT 4
#define PFS_POST_INIT_EVENT 5

#include <string>
#include <map>
#include <vector>
#include <functional>
#include <any>
#include "Core/users/user_permissions_enum.h"

namespace core::experimental {
	namespace structDataEvents {
		// Stores data (username, display name, permissions, position in users vector) of a newly created user.
		struct UserAddEvent {
			std::string username; // Username of the user (who has just been created in the NRC)
			std::string displayName; // Displayed name of the user (who was just created in the NRC)
			UserPermissions permissions; // Permissions of the user (who has just been created in the NRC)
			size_t id; // ID (position in the users vector) of the user (who has just been created in the NRC)

			//bool cancelled;
		};

		// Stores some new and old data (username, display name, permissions) of the user as well as their position in the users vector.
		struct UserChangeEvent {
			std::string oldUsername; // The user's old username.
			std::string username; // The user's new username.
			std::string oldDisplayName; // The old display username.
			std::string displayName; // New displayed user name.
			UserPermissions oldPermissions; // Old user permissions.
			UserPermissions permissions; // New user permissions.
			size_t id; // ID (position in the users vector) of the user.

			//bool cancelled;
		};

		// Contains data (username, display name, permissions) about the deleted (from NRC) user.
		struct UserDeleteEvent {
			std::string username; // Username of the user.
			std::string displayName; // Displayed name of the user.
			UserPermissions permissions; // Permissions of the user.

			//bool cancelled;
		};

		// Contains data about the current user who started the Pseudo-OS (and NRC) shutdown process.
		struct NRCShutdownEvent {
			std::string username; // Username of the user.
			UserPermissions permissions; // Permissions of the user.
			size_t id; // ID (position in the users vector) of the user.
		};

		struct PFSInit {

		};

		struct PFSPostInit {

		};
	};

	class EventManager {
		using SDUserAddEvent = core::experimental::structDataEvents::UserAddEvent;
		using SDUserDeleteEvent = core::experimental::structDataEvents::UserDeleteEvent;
		using SDUserChangeEvent = core::experimental::structDataEvents::UserChangeEvent;
		using SDNRCShutdownEvent = core::experimental::structDataEvents::NRCShutdownEvent;
		using SDPFSInit = core::experimental::structDataEvents::PFSInit;
		using SDPFSPostInit = core::experimental::structDataEvents::PFSPostInit;

		private:
			static inline std::vector<std::function<SDUserAddEvent(SDUserAddEvent)>> userAddEvents;
			static inline std::vector<std::function<SDUserDeleteEvent(SDUserDeleteEvent)>> userDeleteEvents;
			static inline std::vector<std::function<SDUserChangeEvent(SDUserChangeEvent)>> userChangeEvents;
			static inline std::vector<std::function<SDNRCShutdownEvent(SDNRCShutdownEvent)>> NRCShutdownEvents;
			static inline std::vector<std::function<SDPFSInit(SDPFSInit)>> pseudoFSInitEvents;
			static inline std::vector<std::function<SDPFSPostInit(SDPFSPostInit)>> pseudoFSPostInitEvents;
		public:
			static inline bool enableEvents = false;

			// Starts all functions of a certain event.
			static void eventsStart(int eventId, std::any event);
			//auto eventStart(std::string name, std::any event, size_t id);

			static void addEvent(std::function<SDUserAddEvent(SDUserAddEvent)> func);
			static void addEvent(std::function<SDUserDeleteEvent(SDUserDeleteEvent)> func);
			static void addEvent(std::function<SDUserChangeEvent(SDUserChangeEvent)> func);
			static void addEvent(std::function<SDNRCShutdownEvent(SDNRCShutdownEvent)> func);
			static void addEvent(std::function<SDPFSInit(SDPFSInit)> func);
			static void addEvent(std::function<SDPFSPostInit(SDPFSPostInit)> func);
	};

};

#endif