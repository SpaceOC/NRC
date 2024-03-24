#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <chrono>

time_t activationTimeT = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

/*
void calculateWorkTime() {
    time_t currentTime = time(nullptr);

    time_t workTime = difftime(currentTime, activationTimeT);

    int hours = workTime / 3600;
    int minutes = (workTime % 3600) / 60;
    int seconds = workTime % 60;
    
    std::cout << "Work Time: " << std::setw(2) << std::setfill('0') << hours << ":" << std::setw(2) << std::setfill('0') << minutes << ":" << std::setw(2) << std::setfill('0') << seconds << std::endl;
}

void CurrentTimeAndData() {
    auto now = std::chrono::system_clock::now();
    time_t currentTime = std::chrono::system_clock::to_time_t(now);
    tm timeInfo;
    #if defined(_WIN32)
        localtime_s(&timeInfo, &currentTime);
    #else
        localtime_r(&currentTime, &timeInfo);
    #endif

    char buffer[80];
    strftime(buffer, 80, "%H:%M:%S / %d.%m.%Y", &timeInfo);

    std::cout << "Time & Data: " << buffer << std::endl;
}
*/