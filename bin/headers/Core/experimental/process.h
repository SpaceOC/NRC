#pragma once
#ifndef PROCESS
#define PROCESS
#include <map>
#include <string>
#include <atomic>
#include <mutex>
#include <thread>
#include <functional>
#include <memory>

class process {
    private:
        const int maxThreads = 10;
        std::map<std::string, std::shared_ptr<std::thread>> threads;
    public:
        process();
        process(std::string firstThreadName, std::function<void()> func = []{});
        void createThread(std::string threadName, std::function<void()> func = []{});
        void startThread(std::string threadName);
        void stopThread(std::string threadName);

        std::shared_ptr<std::thread> &getThread(std::string threadName);
        std::map<std::string, std::shared_ptr<std::thread>> &getAllThreads();
};

#endif