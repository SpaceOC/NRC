#include "Core/experimental/thread_manager.h"

core::ThreadManager::ThreadManager() {}
core::ThreadManager::ThreadManager(std::string firstThreadName, std::function<void()> func, bool wrapInWhileTrue) {
	if (wrapInWhileTrue) {
		auto wrappedFunc = [this, func, firstThreadName]() -> void {
			while(this->threads[firstThreadName].work) {
				func();
			}
		};
		core::ThreadInfo ti;
		ti.thread = std::make_shared<std::thread>(wrappedFunc);
		ti.work = true;
		threads.emplace(firstThreadName, ti);
	}
	else {
		core::ThreadInfo ti;
		ti.thread = std::make_shared<std::thread>(func);
		ti.work = true;
		threads.emplace(firstThreadName, ti);
	}
}

void core::ThreadManager::createThread(std::string threadName, std::function<void()> func, bool wrapInWhileTrue) {
	if ((threads.size() - 1) < static_cast<size_t>(maxThreads)) {
		if (wrapInWhileTrue) {
			auto wrappedFunc = [this, func, threadName]() -> void {
				while(this->threads[threadName].work) {
					func();
				}
			};
			core::ThreadInfo ti;
			ti.thread = std::make_shared<std::thread>(wrappedFunc);
			ti.work = true;
			threads[threadName] = ti;
		}
		else {
			core::ThreadInfo ti;
			ti.thread = std::make_shared<std::thread>(func);
			ti.work = true;
			threads[threadName] = ti;
		}
	}
}

void core::ThreadManager::startThread(std::string threadName) {
	if (threads.find(threadName) != threads.end() && threads[threadName].thread->joinable()) {
		threads[threadName].thread->detach();
	}
}

void core::ThreadManager::stopThread(std::string threadName) {
	if (threads.find(threadName) != threads.end()) {
		threads[threadName].work = false;
	}
}

std::shared_ptr<std::thread> &core::ThreadManager::getThread(std::string threadName) {
	return threads[threadName].thread;
}

std::map<std::string, core::ThreadInfo> &core::ThreadManager::getAllThreads() {
	return threads;
}