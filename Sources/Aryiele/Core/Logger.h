#ifndef ARYIELE_LOGGER_H
#define ARYIELE_LOGGER_H

#include <Vanir/Logger/Logger.h>

#define LOG(...) ::Vanir::Logger::Log(__VA_ARGS__);
#define LOG_INFO(...) \
{ \
    ::Vanir::Logger::Log("aryiele: ", __VA_ARGS__); \
    ::Vanir::Logger::InfoCount += 1; \
}
#define LOG_VERBOSE(...) \
{ \
    ::Vanir::Logger::Log("aryiele: ", ::Vanir::LogColor(::Vanir::TerminalColor_Bright_Blue), "verbose: ", ::Vanir::LogColor(), __VA_ARGS__); \
    ::Vanir::Logger::WarningCount += 1; \
}
#define LOG_WARNING(...) \
{ \
    ::Vanir::Logger::Log("aryiele: ", ::Vanir::LogColor(::Vanir::TerminalColor_Yellow), "warning: ", __VA_ARGS__, ::Vanir::LogColor()); \
    ::Vanir::Logger::WarningCount += 1; \
}
#define LOG_ERROR(...) \
{ \
    ::Vanir::Logger::Log("aryiele: ", ::Vanir::LogColor(::Vanir::TerminalColor_Red), "error: ", __VA_ARGS__, ::Vanir::LogColor()); \
    ::Vanir::Logger::ErrorCount += 1; \
}

#ifdef _WIN32
#define ULOG(...) ::Vanir::Logger::ULog(__VA_ARGS__);
#define ULOG_INFO(...) \
{ \
    ::Vanir::Logger::ULog("aryiele: ", __VA_ARGS__); \
    ::Vanir::Logger::InfoCount += 1; \
}
#define ULOG_VERBOSE(...) \
{ \
    ::Vanir::Logger::ULog("aryiele: ", ::Vanir::LogColor(::Vanir::TerminalColor_Bright_Blue), "verbose: ", ::Vanir::LogColor(), __VA_ARGS__); \
    ::Vanir::Logger::WarningCount += 1; \
}
#define ULOG_WARNING(...) \
{ \
    ::Vanir::Logger::ULog("aryiele: ", ::Vanir::LogColor(::Vanir::TerminalColor_Yellow), "warning: ", __VA_ARGS__, ::Vanir::LogColor()); \
    ::Vanir::Logger::WarningCount += 1; \
}
#define ULOG_ERROR(...) \
{ \
    ::Vanir::Logger::ULog("aryiele: ", ::Vanir::LogColor(::Vanir::TerminalColor_Red), "error: ", __VA_ARGS__, ::Vanir::LogColor()); \
    ::Vanir::Logger::ErrorCount += 1; \
}
#else
#define ULOG(...) LOG(__VA_ARGS__)
#define ULOG_INFO(...) LOG_INFO(__VA_ARGS__)
#define ULOG_WARNING(...) LOG_WARNING(__VA_ARGS__)
#define ULOG_ERROR(...) LOG_ERROR(__VA_ARGS__)
#endif

#endif /* ARYIELE_LOGGER_H. */
