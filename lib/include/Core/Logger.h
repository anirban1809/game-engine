#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>
#define LOG_INFO 1
#define LOG_DEBUG 2
#define LOG_ERROR 3

class Logger {
   public:
    static void Log(int log_type, const std::string& format, ...);
};

#endif  // __LOGGER_H__