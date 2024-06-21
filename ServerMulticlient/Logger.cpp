//
// Created by student on 6/21/24.
//

#include "Logger.h"
#include <iostream>
#include <ctime>

Logger::Logger() {
    logFile.open("serverlog.txt", std::ios::out | std::ios::app);
    if (!logFile.is_open()) {
        throw std::runtime_error("Unable to open log file.");
    }
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::log(const std::string& message) {
    std::lock_guard<std::mutex> lock(mtx);

    std::time_t now = std::time(nullptr);
    char time_str[20];
    std::strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

    logFile << "[" << time_str << "] " << message << std::endl;
    std::cout << "[" << time_str << "] " << message << std::endl;
}
