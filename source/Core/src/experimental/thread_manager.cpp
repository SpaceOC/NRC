#ifndef NRC_DISABLE_EXPERIMENTAL_FEATURES
#include "Core/experimental/thread_manager.h"

core::experimental::ThreadManager::ThreadManager() {}
core::experimental::ThreadManager::ThreadManager(const std::string& firstThreadName, std::function<void()> func, bool wrapInWhileTrue) {
	if (wrapInWhileTrue) {
		auto wrappedFunc = [this, func, firstThreadName]() -> void {
			while(this->threads[firstThreadName].work) {
				func();
			}
		};
		core::experimental::ThreadInfo ti;
		ti.thread = std::make_shared<std::thread>(wrappedFunc);
		ti.work = true;
		threads.emplace(firstThreadName, ti);
	}
	else {
		core::experimental::ThreadInfo ti;
		ti.thread = std::make_shared<std::thread>(func);
		ti.work = true;
		threads.emplace(firstThreadName, ti);
	}
}

void core::experimental::ThreadManager::createThread(const std::string& threadName, std::function<void()> func, bool wrapInWhileTrue) {
	if ((threads.size() - 1) < static_cast<size_t>(maxThreads)) {
		if (wrapInWhileTrue) {
			auto wrappedFunc = [this, func, threadName]() -> void {
				while(this->threads[threadName].work) {
					func();
				}
			};
			core::experimental::ThreadInfo ti;
			ti.thread = std::make_shared<std::thread>(wrappedFunc);
			ti.work = true;
			threads[threadName] = ti;
		}
		else {
			core::experimental::ThreadInfo ti;
			ti.thread = std::make_shared<std::thread>(func);
			ti.work = true;
			threads[threadName] = ti;
		}
	}
}

void core::experimental::ThreadManager::startThread(const std::string& threadName) {
	if (threads.find(threadName) != threads.end() && threads[threadName].thread->joinable()) {
		threads[threadName].thread->detach();
	}
}

void core::experimental::ThreadManager::stopThread(const std::string& threadName) {
	if (threads.find(threadName) != threads.end()) {
		threads[threadName].work = false;
	}
}

std::shared_ptr<std::thread> &core::experimental::ThreadManager::getThread(const std::string& threadName) {
	return threads[threadName].thread;
}

std::map<std::string, core::experimental::ThreadInfo> &core::experimental::ThreadManager::getAllThreads() {
	return threads;
}
#endif