//==================================================================================//
//                                                                                  //
//  Copyright (c) 2019 Hugo Kindel <kindelhugo.pro@gmail.com>                       //
//                                                                                  //
//  This file is part of the Aryiele project.                                       //
//  Licensed under MIT License:                                                     //
//                                                                                  //
//  Permission is hereby granted, free of charge, to any person obtaining           //
//  a copy of this software and associated documentation files (the "Software"),    //
//  to deal in the Software without restriction, including without limitation       //
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,        //
//  and/or sell copies of the Software, and to permit persons to whom the           //
//  Software is furnished to do so, subject to the following conditions:            //
//                                                                                  //
//  The above copyright notice and this permission notice shall be included in      //
//  all copies or substantial portions of the Software.                             //
//                                                                                  //
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      //
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        //
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     //
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          //
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   //
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   //
//  SOFTWARE.                                                                       //
//                                                                                  //
//==================================================================================//

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
    ::Vanir::Logger::Log("aryiele: ", ::Vanir::LogColor(::Vanir::TerminalColor_Yellow), "warning: ", ::Vanir::LogColor(), __VA_ARGS__); \
    ::Vanir::Logger::WarningCount += 1; \
}
#define LOG_ERROR(...) \
{ \
    ::Vanir::Logger::Log("aryiele: ", ::Vanir::LogColor(::Vanir::TerminalColor_Red), "error: ", ::Vanir::LogColor(), __VA_ARGS__); \
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
    ::Vanir::Logger::ULog("aryiele: ", ::Vanir::LogColor(::Vanir::TerminalColor_Yellow), "warning: ", ::Vanir::LogColor(), __VA_ARGS__); \
    ::Vanir::Logger::WarningCount += 1; \
}
#define ULOG_ERROR(...) \
{ \
    ::Vanir::Logger::ULog("aryiele: ", ::Vanir::LogColor(::Vanir::TerminalColor_Red), "error: ", ::Vanir::LogColor(), __VA_ARGS__); \
    ::Vanir::Logger::ErrorCount += 1; \
}
#else
#define ULOG(...) LOG(__VA_ARGS__)
#define ULOG_INFO(...) LOG_INFO(__VA_ARGS__)
#define ULOG_WARNING(...) LOG_WARNING(__VA_ARGS__)
#define ULOG_ERROR(...) LOG_ERROR(__VA_ARGS__)
#endif

#endif /* ARYIELE_LOGGER_H. */