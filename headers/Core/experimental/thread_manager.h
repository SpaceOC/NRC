// is not finished!
#ifndef NRC_DISABLE_EXPERIMENTAL_FEATURES
#ifndef NRC_EXPERIMENTAL_THREAD_MANAGER_H_
#define NRC_EXPERIMENTAL_THREAD_MANAGER_H_
#include <map>
#include <string>
#include <atomic>
#include <mutex>
#include <thread>
#include <functional>
#include <memory>

namespace core::experimental {
	struct ThreadInfo {
		std::shared_ptr<std::thread> thread;
		bool work = true;
	};

	class ThreadManager {
		private:
			std::map<std::string, ThreadInfo> _threads = {};
		public:
			ThreadManager();
			ThreadManager(const std::string& firstThreadName, std::function<void()> func = []{}, bool wrapInLoop = true);
			void createThread(const std::string& threadName, std::function<void()> func = []{}, bool wrapInLoop = true);
			void startThread(const std::string& threadName);
			void stopThread(const std::string& threadName);

			std::shared_ptr<std::thread> &getThread(const std::string& threadName);
			std::map<std::string, ThreadInfo> &getAllThreads();

			static const int kMaxThreads = 6;
	};
}

#endif // NRC_EXPERIMENTAL_THREAD_MANAGER_H_
#endif // NRC_DISABLE_EXPERIMENTAL_FEATURES