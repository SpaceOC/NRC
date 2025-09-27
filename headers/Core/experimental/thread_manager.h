// is not finished!
#ifndef NRC_EXPERIMENTAL_THREAD_MANAGER_H_
#define NRC_EXPERIMENTAL_THREAD_MANAGER_H_
#include <map>
#include <string>
#include <atomic>
#include <mutex>
#include <thread>
#include <functional>
#include <memory>

namespace core {
	struct ThreadInfo {
		std::shared_ptr<std::thread> thread;
		bool work = true;
	};

	class ThreadManager {
		private:
			static const int maxThreads = 6;
			std::map<std::string, ThreadInfo> threads = {};
		public:
			ThreadManager();
			ThreadManager(std::string firstThreadName, std::function<void()> func = []{}, bool wrapInWhileTrue = true);
			void createThread(std::string threadName, std::function<void()> func = []{}, bool wrapInWhileTrue = true);
			void startThread(std::string threadName);
			void stopThread(std::string threadName);

			std::shared_ptr<std::thread> &getThread(std::string threadName);
			std::map<std::string, ThreadInfo> &getAllThreads();
	};
}

#endif