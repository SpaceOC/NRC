#ifndef NRC_BASE_MAIN_H_
#define NRC_BASE_MAIN_H_

#include <functional>
#include <vector>
#include "Core/command/command_sender.h"
#include "Core/modules/module_base.h"

namespace core {
	class CommandSenderBasic;
	class main {
		private:
			void fixNOW();
			
			// Adding all standard commands.
			void addCommands();
			void addCRules();
			void loop();
			void searchModules();
			void checkModule(const std::string& name);
			std::function<void()> loopedFunc;
			std::function<void()> startFunc;
			CommandSenderBasic* commandSender;
			std::vector<ModuleBase*> modules;
		protected:
			std::atomic<bool> work = true;
		public:
			main();
			main(std::function<void()> start, std::function<void()> loop);
			main(std::function<void()> start);
			~main();

			void init();
			void start();
			void stopWork();
			void setCommandSender(CommandSenderBasic* newCommandSender);
	};
}

#endif