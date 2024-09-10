/*
    Copyright (C) 2024-2024  SpaceOC

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "Core/experimental/process.h"

core::process::process() {}
core::process::process(std::string firstThreadName, std::function<void()> func) {
    threads.emplace(firstThreadName, std::make_shared<std::thread>(func));
}

void core::process::createThread(std::string threadName, std::function<void()> func) {
    if ((threads.size() - 1) < maxThreads) {
        threads[threadName] = std::make_shared<std::thread>(func);
    }
}

void core::process::startThread(std::string threadName) {
    if (threads.find(threadName) != threads.end() && threads[threadName]->joinable()) {
        threads[threadName]->join();
    }
}

void core::process::stopThread(std::string threadName) {
    if (threads.find(threadName) != threads.end()) {
        threads[threadName]->detach();
    }
}

std::shared_ptr<std::thread> &core::process::getThread(std::string threadName) {
    return threads[threadName];
}

std::map<std::string, std::shared_ptr<std::thread>> &core::process::getAllThreads() {
    return threads;
}