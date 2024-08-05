#include "Core/experimental/process.h"

process::process() {}
process::process(std::string firstThreadName, std::function<void()> func) {
    threads.emplace(firstThreadName, std::make_shared<std::thread>(func));
}

void process::createThread(std::string threadName, std::function<void()> func) {
    if ((threads.size() - 1) < maxThreads) {
        threads[threadName] = std::make_shared<std::thread>(func);
    }
}

void process::startThread(std::string threadName) {
    if (threads.find(threadName) != threads.end() && threads[threadName]->joinable()) {
        threads[threadName]->join();
    }
}

void process::stopThread(std::string threadName) {
    if (threads.find(threadName) != threads.end()) {
        threads[threadName]->detach();
    }
}

std::shared_ptr<std::thread> &process::getThread(std::string threadName) {
    return threads[threadName];
}

std::map<std::string, std::shared_ptr<std::thread>> &process::getAllThreads() {
    return threads;
}