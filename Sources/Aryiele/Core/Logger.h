#ifndef ARYIELE_LOGGER_H
#define ARYIELE_LOGGER_H

#include <Vanir/Logger.h>

#define LOG(...) VANIR_LOG(__VA_ARGS__)
#define LOG_INFO(...) VANIR_LOG_INFO(__VA_ARGS__)
#define LOG_WARNING(...) VANIR_LOG_WARNING(__VA_ARGS__)
#define LOG_ERROR(...) VANIR_LOG_ERROR(__VA_ARGS__)
#ifdef PLATFORM_WINDOWS
#define ULOG(...) VANIR_ULOG(__VA_ARGS__)
#define ULOG_INFO(...) VANIR_ULOG_INFO(__VA_ARGS__)
#define ULOG_WARNING(...) VANIR_ULOG_WARNING(__VA_ARGS__)
#define ULOG_ERROR(...) VANIR_ULOG_ERROR(__VA_ARGS__)
#else
#define ULOG(...) VANIR_LOG(__VA_ARGS__)
#define ULOG_INFO(...) VANIR_LOG_INFO(__VA_ARGS__)
#define ULOG_WARNING(...) VANIR_LOG_WARNING(__VA_ARGS__)
#define ULOG_ERROR(...) VANIR_LOG_ERROR(__VA_ARGS__)
#endif

#endif /* ARYIELE_LOGGER_H. */
