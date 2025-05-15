#include "Core/experimental/process.h"

core::Process::Process() {}
core::Process::Process(std::string firstThreadName, std::function<void()> func) {
	threads.emplace(firstThreadName, std::make_shared<std::thread>(func));
}

void core::Process::createThread(std::string threadName, std::function<void()> func) {
	if ((threads.size() - 1) < static_cast<size_t>(maxThreads)) {
		threads[threadName] = std::make_shared<std::thread>(func);
	}
}

void core::Process::startThread(std::string threadName) {
	if (threads.find(threadName) != threads.end() && threads[threadName]->joinable()) {
		threads[threadName]->join();
	}
}

void core::Process::stopThread(std::string threadName) {
	if (threads.find(threadName) != threads.end()) {
		threads[threadName]->detach();
	}
}

std::shared_ptr<std::thread> &core::Process::getThread(std::string threadName) {
	return threads[threadName];
}

std::map<std::string, std::shared_ptr<std::thread>> &core::Process::getAllThreads() {
	return threads;
}