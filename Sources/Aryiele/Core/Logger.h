#ifndef ARYIELE_LOGGER_H
#define ARYIELE_LOGGER_H

#include <Vanir/Logger.h>

#if defined(CONFIGURATION_DEBUG)
#define LOG_TEXT(LOGTEXT, ...) \
{ \
std::string _aesir_log_text__ = ::Vanir::Logger::Log(LOGTEXT, __FUNCTION__, __LINE__, nbarg(__VA_ARGS__), ##__VA_ARGS__); \
}
#define LOG_INFO(LOGTEXT, ...) \
{ \
    std::string _aesir_log_text__ = ::Vanir::Logger::LogInfo(LOGTEXT, __FUNCTION__, __LINE__, nbarg(__VA_ARGS__), ##__VA_ARGS__); \
}
#define LOG_WARNING(LOGTEXT, ...) \
{ \
    std::string _aesir_log_text__ = ::Vanir::Logger::LogWarning(LOGTEXT, __FUNCTION__, __LINE__, nbarg(__VA_ARGS__), ##__VA_ARGS__); \
}
#define LOG_ERROR(LOGTEXT, ...) \
{ \
    std::string _aesir_log_text__ = ::Vanir::Logger::LogError(LOGTEXT, __FUNCTION__, __LINE__, nbarg(__VA_ARGS__), ##__VA_ARGS__); \
}
#else
#define LOG_TEXT(LOGTEXT, ...) \
{ \
    std::string _aesir_log_text__ = ::Vanir::Logger::Log(LOGTEXT, nbarg(__VA_ARGS__), ##__VA_ARGS__); \
}
#define LOG_INFO(LOGTEXT, ...) \
{ \
    std::string _aesir_log_text__ = ::Vanir::Logger::LogInfo(LOGTEXT, nbarg(__VA_ARGS__), ##__VA_ARGS__); \
}
#define LOG_WARNING(LOGTEXT, ...) \
{ \
    std::string _aesir_log_text__ = ::Vanir::Logger::LogWarning(LOGTEXT, nbarg(__VA_ARGS__), ##__VA_ARGS__); \
}
#define LOG_ERROR(LOGTEXT, ...) \
{ \
    std::string _aesir_log_text__ = ::Vanir::Logger::LogError(LOGTEXT, nbarg(__VA_ARGS__), ##__VA_ARGS__); \
}
#endif
#define LOG_DEFAULT(LOGTEXT, ...) \
{ \
    std::string _aesir_log_text__ = ::Vanir::Logger::LogDefault(LOGTEXT, nbarg(__VA_ARGS__), ##__VA_ARGS__); \
}
#define LOG_USER(LOGTEXT, ...) \
{ \
    std::string _aesir_log_text__ = ::Vanir::Logger::LogDefault(LOGTEXT, nbarg(__VA_ARGS__), ##__VA_ARGS__); \
}

#endif /* ARYIELE_LOGGER_H. */
