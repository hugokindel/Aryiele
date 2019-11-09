#ifndef ARYIELE_LOGGER_H
#define ARYIELE_LOGGER_H

#include <Vanir/Logger/Logger.h>

/*#define LOG_RESETCOUNTERS() VANIR_LOG_RESETCOUNTERS();

#define LOG(...) VANIR_LOG("aryiele: ", __VA_ARGS__);
#define LOG_VERBOSE(...) VANIR_LOG("aryiele: ", Vanir::LogColor(Vanir::TerminalColor_Bright_Blue), "verbose: ", Vanir::LogColor(), __VA_ARGS__);
#define LOG_INFO(...) \
{ \
    VANIR_LOG("aryiele: ", Vanir::LogColor(Vanir::TerminalColor_Blue), "info: ", Vanir::LogColor(), __VA_ARGS__); \
    VANIR_LOG_INCREASECOUNTER(::Vanir::LoggerTypes::LoggerTypes_Info); \
}
#define LOG_WARNING(...) \
{ \
    VANIR_LOG("aryiele: ", Vanir::LogColor(Vanir::TerminalColor_Yellow), "warning: ", __VA_ARGS__, Vanir::LogColor()); \
    VANIR_LOG_INCREASECOUNTER(::Vanir::LoggerTypes::LoggerTypes_Warning); \
}
#define LOG_ERROR(...) \
{ \
    LOG("aryiele: ", Vanir::LogColor(Vanir::TerminalColor_Red), "error: ", __VA_ARGS__, Vanir::LogColor()); \
    VANIR_LOG_INCREASECOUNTER(::Vanir::LoggerTypes::LoggerTypes_Error); \
}
#ifdef PLATFORM_WINDOWS
#define ULOG(...) VANIR_ULOG("aryiele: ", __VA_ARGS__);
#define ULOG_VERBOSE(...) VANIR_ULOG("aryiele: ", Vanir::LogColor(Vanir::TerminalColor_Bright_Blue), "verbose: ", Vanir::LogColor(), __VA_ARGS__);
#define ULOG_INFO(...) \
{ \
    VANIR_ULOG("aryiele: ", Vanir::LogColor(Vanir::TerminalColor_Blue), "info: ", Vanir::LogColor(), __VA_ARGS__); \
    VANIR_LOG_INCREASECOUNTER(::Vanir::LoggerTypes::LoggerTypes_Info); \
}
#define ULOG_WARNING(...) \
{ \
    VANIR_ULOG("aryiele: ", Vanir::LogColor(Vanir::TerminalColor_Yellow), "warning: ", __VA_ARGS__, Vanir::LogColor()); \
    VANIR_LOG_INCREASECOUNTER(::Vanir::LoggerTypes::LoggerTypes_Warning); \
}
#define ULOG_ERROR(...) \
{ \
    VANIR_ULOG("aryiele: ", Vanir::LogColor(Vanir::TerminalColor_Red), "error: ", __VA_ARGS__, Vanir::LogColor()); \
    VANIR_LOG_INCREASECOUNTER(::Vanir::LoggerTypes::LoggerTypes_Error); \
}
#else
#define ULOG(...) LOG(__VA_ARGS__);
#define ULOG_INFO(...) LOG_INFO(__VA_ARGS__);
#define ULOG_VERBOSE(...) LOG_VERBOSE(__VA_ARGS__);
#define ULOG_WARNING(...) LOG_WARNING(__VA_ARGS__);
#define ULOG_ERROR(...) LOG_ERROR(__VA_ARGS__);
#endif*/

#endif /* ARYIELE_LOGGER_H. */
