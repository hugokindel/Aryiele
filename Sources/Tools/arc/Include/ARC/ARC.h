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

#ifndef ARYIELE_ARC_H
#define ARYIELE_ARC_H

#include <Vanir/CLI/CLI.h>
#include <Aryiele/Core/Includes.h>
#include <Aryiele/Lexer/Lexer.h>
#include <Aryiele/Parser/Parser.h>
#include <ARC/BuildTypes.h>

#define ARC_VERSION "0.0.1"
#define FINAL_RELEASE // ONLY use for final releases.

#include <Vanir/Logger/Logger.h>

#define LOG(...) ::Vanir::Logger::Log(__VA_ARGS__);
#define LOG_INFO(...) \
{ \
    ::Vanir::Logger::Log("arc: ", __VA_ARGS__); \
    ::Vanir::Logger::InfoCount += 1; \
}
#define LOG_VERBOSE(...) \
{ \
    ::Vanir::Logger::Log("arc: ", ::Vanir::LogColor(::Vanir::TerminalColor_Bright_Blue), "verbose: ", ::Vanir::LogColor(), __VA_ARGS__); \
    ::Vanir::Logger::WarningCount += 1; \
}
#define LOG_WARNING(...) \
{ \
    ::Vanir::Logger::Log("arc: ", ::Vanir::LogColor(::Vanir::TerminalColor_Yellow), "warning: ", ::Vanir::LogColor(), __VA_ARGS__); \
    ::Vanir::Logger::WarningCount += 1; \
}
#define LOG_ERROR(...) \
{ \
    ::Vanir::Logger::Log("arc: ", ::Vanir::LogColor(::Vanir::TerminalColor_Red), "error: ", ::Vanir::LogColor(), __VA_ARGS__); \
    ::Vanir::Logger::ErrorCount += 1; \
}

#ifdef _WIN32
#define ULOG(...) ::Vanir::Logger::ULog(__VA_ARGS__);
#define ULOG_INFO(...) \
{ \
    ::Vanir::Logger::ULog("arc: ", __VA_ARGS__); \
    ::Vanir::Logger::InfoCount += 1; \
}
#define ULOG_VERBOSE(...) \
{ \
    ::Vanir::Logger::ULog("arc: ", ::Vanir::LogColor(::Vanir::TerminalColor_Bright_Blue), "verbose: ", ::Vanir::LogColor(), __VA_ARGS__); \
    ::Vanir::Logger::WarningCount += 1; \
}
#define ULOG_WARNING(...) \
{ \
    ::Vanir::Logger::ULog("arc: ", ::Vanir::LogColor(::Vanir::TerminalColor_Yellow), "warning: ", ::Vanir::LogColor(), __VA_ARGS__); \
    ::Vanir::Logger::WarningCount += 1; \
}
#define ULOG_ERROR(...) \
{ \
    ::Vanir::Logger::ULog("arc: ", ::Vanir::LogColor(::Vanir::TerminalColor_Red), "error: ", ::Vanir::LogColor(), __VA_ARGS__); \
    ::Vanir::Logger::ErrorCount += 1; \
}
#else
#define ULOG(...) LOG(__VA_ARGS__)
#define ULOG_INFO(...) LOG_INFO(__VA_ARGS__)
#define ULOG_WARNING(...) LOG_WARNING(__VA_ARGS__)
#define ULOG_ERROR(...) LOG_ERROR(__VA_ARGS__)
#endif

#define ARC_RUN_CHECKERRORS() \
{ \
    if (::Vanir::Logger::ErrorCount > 0) \
        return 0; \
    Vanir::Logger::ResetCounters(); \
}

namespace ARC
{
    // Main class of the Aryiele Compiler command line tool.
    class ARC
    {
    public:
        static int Run(int argc, char *argv[]);
        static std::vector<Aryiele::LexerToken> DoLexerPass(const std::string& filepath);
        static std::vector<std::shared_ptr<Aryiele::Node>> DoParserPass(std::vector<Aryiele::LexerToken> lexerTokens);
        static void DoCodeGeneratorPass(std::vector<std::shared_ptr<Aryiele::Node>> astNodes);
        static void DoObjectGeneratorPass();
        static void DoExecutableGeneratorPass();
        static void DumpASTInformations(const std::shared_ptr<Aryiele::ParserInformation>& node, std::string indent = "");
        static void CommandShowHelp(const std::string& s = "");
        static void CommandShowVersion(const std::string& s = "");
        static void CommandDefineInputFilepath(const std::string& s = "");
        static void CommandDefineOutputFilepath(const std::string& s = "");
        static void CommandActivateVerboseMode(const std::string& s = "");
        static void CommandKeepAllFiles(const std::string& s = "");
        static void CommandDefineBuildType(const std::string& s = "");
        static void CommandOptionNotFound(const std::string& s = "");
        static std::string GetOptionValue(const std::string& option);

    private:
        static std::vector<Vanir::Argument> m_options;
        static std::string m_inputFilepath;
        static std::string m_outputFilepath;
        static std::string m_tempIRFilepath;
        static std::string m_tempOBJFilepath;
        static std::string m_tempEXEFilepath;
        static std::string m_tempArgv;
        static bool m_verboseMode;
        static bool m_keepAllFiles;
        static BuildTypes m_buildType;
    };

} /* Namespace ARC. */

#endif /* ARYIELE_ARC_H. */
