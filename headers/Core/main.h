#ifndef NRC_BASE_MAIN_H_
#define NRC_BASE_MAIN_H_

#include <functional>
#include <vector>
#include "CORE_info.h"
#include "Core/other/versionc.h"

#if defined(_WIN32)
#define OS_NAME_STR "WINDOWS"
#elif defined(__APPLE__)
#define OS_NAME_STR "MAC"
#else
#define OS_NAME_STR "LINUX"
#endif

namespace core {
	class CommandSenderBasic;
	#ifndef NRC_DISABLE_EXPERIMENTAL_FEATURES
	class ModuleBase;
	#endif
	class main {
		private:
			void fixNOW();
			
			// Adding all standard commands.
			void addCommands();
			void addCRules();
			void loop();
			#ifndef NRC_DISABLE_EXPERIMENTAL_FEATURES
			void searchModules();
			void checkModule(const std::string& name);
			#endif
			std::function<void()> loopedFunc;
			std::function<void()> startFunc;
			CommandSenderBasic* commandSender;
			#ifndef NRC_DISABLE_EXPERIMENTAL_FEATURES
			std::vector<ModuleBase*> modules;
			#endif
			bool commandSenderReplaced = false;
		protected:
			std::atomic<bool> work = true;
		public:
			const VersionC version = VersionC(CORE_VERSION);

			main();
			main(std::function<void()> start, std::function<void()> loop);
			main(std::function<void()> start);
			~main();

			/**
			 * Инициализация NRC.
			 * @note Обязательно вызывайте этот метод перед start()
			 */
			void init();
			/**
			 * Запуск NRC
			 * @note Обязательно вызывайте этот метод после init()
			 */
			void start();
			void stopWork();
			void setCommandSender(CommandSenderBasic* newCommandSender);
			#ifndef NRC_DISABLE_EXPERIMENTAL_FEATURES
			void* getRequiredClassPtr(const std::string& request, ModuleBase* module);
			#endif
	};
}

#endif