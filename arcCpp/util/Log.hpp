#ifndef ARC_UTIL_LOG_HPP
#define ARC_UTIL_LOG_HPP

#include <string>
#include "LogHandler.hpp"
namespace arc::util {
    class Log {
    public:
static int logLevel;
inline static arc::util::LogHandler logger;
        static void log(const std::string& str);
static void warn(const std::string& str);
static void debug(const std::string& str);
static void err(const std::string_view& str);
static void info(const std::string_view& str);

    };
}

#endif
