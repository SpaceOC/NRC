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
// is not finished!
#pragma once
#ifndef NRC_EXPERIMENTAL_PROCESS_H_
#define NRC_EXPERIMENTAL_PROCESS_H_
#include <map>
#include <string>
#include <atomic>
#include <mutex>
#include <thread>
#include <functional>
#include <memory>

namespace core {
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
}

#endif