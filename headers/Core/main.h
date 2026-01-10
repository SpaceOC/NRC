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
	class Main {
	private:
		std::function<void()> _loopedFunc;
		std::function<void()> _startFunc;
		CommandSenderBasic* _commandSender;
		#ifndef NRC_DISABLE_EXPERIMENTAL_FEATURES
		std::vector<ModuleBase*> _modules;
		#endif
		bool _isCommandSenderReplaced = false;

		void fixNOW();
			
		// Adding all standard commands.
		void addCommands();
		void addCRules();
		void loop();
		#ifndef NRC_DISABLE_EXPERIMENTAL_FEATURES
		void searchModules();
		bool checkModule(const std::string& name);
		#endif

	protected:
		std::atomic<bool> work = true;

	public:
		static inline const VersionC kCoreVersion = VersionC(CORE_VERSION);

		Main();
		Main(std::function<void()> start, std::function<void()> loop);
		Main(std::function<void()> start);
		~Main();

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