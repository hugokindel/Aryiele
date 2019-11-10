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
#include <arc/BuildTypes.h>

#define ARC_VERSION "0.0.1"
#define FINAL_RELEASE // ONLY use for final releases.

/*#define ALOG(...) VANIR_LOG("arc: ", __VA_ARGS__);
#define ALOG_VERBOSE(...) VANIR_LOG("arc: ", Vanir::LoggerColor(Vanir::LoggerColors_Bright_Blue), "verbose: ", Vanir::LoggerColor(), __VA_ARGS__);
#define ALOG_INFO(...) \
{ \
    VANIR_LOG("arc: ", Vanir::LoggerColor(Vanir::LoggerColors_Blue), "info: ", Vanir::LoggerColor(), __VA_ARGS__); \
    VANIR_LOG_INCREASECOUNTER(::Vanir::LoggerTypes::LoggerTypes_Info); \
}
#define ALOG_WARNING(...) \
{ \
    VANIR_LOG("arc: ", Vanir::LoggerColor(Vanir::LoggerColors_Yellow), "warning: ", __VA_ARGS__, Vanir::LoggerColor()); \
    VANIR_LOG_INCREASECOUNTER(::Vanir::LoggerTypes::LoggerTypes_Warning); \
}
#define ALOG_ERROR(...) \
{ \
    VANIR_LOG("arc: ", Vanir::LoggerColor(Vanir::LoggerColors_Red), "error: ", __VA_ARGS__, Vanir::LoggerColor()); \
    VANIR_LOG_INCREASECOUNTER(::Vanir::LoggerTypes::LoggerTypes_Error); \
}
#ifdef PLATFORM_WINDOWS
#define AULOG(...) VANIR_ULOG("arc: ", __VA_ARGS__);
#define AULOG_VERBOSE(...) VANIR_ULOG("arc: ", Vanir::LoggerColor(Vanir::LoggerColors_Bright_Blue), "verbose: ", Vanir::LoggerColor(), __VA_ARGS__);
#define AULOG_INFO(...) \
{ \
    VANIR_ULOG("arc: ", Vanir::LoggerColor(Vanir::LoggerColors_Blue), "info: ", Vanir::LoggerColor(), __VA_ARGS__); \
    VANIR_LOG_INCREASECOUNTER(::Vanir::LoggerTypes::LoggerTypes_Info); \
}
#define AULOG_WARNING(...) \
{ \
    VANIR_ULOG("arc: ", Vanir::LoggerColor(Vanir::LoggerColors_Yellow), "warning: ", __VA_ARGS__, Vanir::LoggerColor()); \
    VANIR_LOG_INCREASECOUNTER(::Vanir::LoggerTypes::LoggerTypes_Warning); \
}
#define AULOG_ERROR(...) \
{ \
    VANIR_ULOG("arc: ", Vanir::LoggerColor(Vanir::LoggerColors_Red), "error: ", __VA_ARGS__, Vanir::LoggerColor()); \
    VANIR_LOG_INCREASECOUNTER(::Vanir::LoggerTypes::LoggerTypes_Error); \
}
#else
#define AULOG(...) ALOG(__VA_ARGS__);
#define AULOG_INFO(...) ALOG_INFO(__VA_ARGS__);
#define AULOG_VERBOSE(...) ALOG_VERBOSE(__VA_ARGS__);
#define AULOG_WARNING(...) ALOG_WARNING(__VA_ARGS__);
#define AULOG_ERROR(...) ALOG_ERROR(__VA_ARGS__);
#endif*/

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
