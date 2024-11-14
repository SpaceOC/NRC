#include <iostream>
#include <chrono>
#include <atomic>
#include "Core/base/print.h"
#include "Core/experimental/process.h"

extern void commandsZone(); // from os_main.cpp
extern std::atomic<int> atomicI;
extern bool work;

void counterMov();
core::Process processNew("main", counterMov);

void startProcess() {
    processNew.createThread("notmain", commandsZone);
    processNew.startThread("main");
    processNew.startThread("notmain");
}

void counterMov() {
    for (int i = 0; i <= 2024; i++) {
        atomicI = i;
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        if (!work) break;

        if (i == 2024) {
            core::print("GOOD JOB\n", core::PrintColors::blue);
            core::print("Thread id: ", core::PrintColors::green);
            std::cout << processNew.getThread("main")->get_id() << '\n';
            core::print("NRC Process name: main\n", core::PrintColors::aqua);
        }
    }
}