#include "../../include/Core/Logger.h"
#include <algorithm>
#include <cstdarg>
#include <cstdio>
#include <iostream>
#include <string>

void Logger::Log(int log_type, const std::string& format, ...) {
    std::string logTypeString = "";

    switch (log_type) {
        case LOG_INFO:
            logTypeString = "INFO";
            break;
        case LOG_DEBUG:
            logTypeString = "DEBUG";
            break;
        case LOG_ERROR:
            logTypeString = "ERROR";
            break;

        default:
            logTypeString = "INFO";
    }

    std::cout << "[" << logTypeString << "] :: " << format << std::endl;
}
