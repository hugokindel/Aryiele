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

#ifndef ARYIELE_CORE_ARC_H
#define ARYIELE_CORE_ARC_H

#include <Vanir/CLI/CLI.h>
#include <Aryiele/Common.h>
#include <Aryiele/Lexer/Lexer.h>
#include <Aryiele/Parser/Parser.h>
#include <ARC/Core/BuildType.h>
#include <ARC/Utility/Logger.h>

#define ARC_VERSION "0.0.1"
#define FINAL_RELEASE // ONLY use for final releases.

#define ARC_RUN_CHECKERRORS() { \
    if (::Vanir::Logger::errorCount > 0) \
        return 1; \
    Vanir::Logger::resetCounters(); \
}

namespace ARC {
    class ARC {
    public:
        static int run(int argc, char *argv[]);
        static std::vector<Aryiele::LexerToken> doLexerPass(const std::string& filepath);
        static std::shared_ptr<Aryiele::NodeRoot> doParserPass(const std::string& path,
            std::vector<Aryiele::LexerToken> lexerTokens);
        static void doCodeGeneratorPass(std::shared_ptr<Aryiele::NodeRoot> nodeRoot);
        static void doObjectGeneratorPass();
        static void doExecutableGeneratorPass();
        static void dumpASTInformations(const std::shared_ptr<Aryiele::ParserInformation>& node, std::string indent = "");
        static void commandShowHelp(const std::string& s = "");
        static void commandShowVersion(const std::string& s = "");
        static void commandDefineInputFilepath(const std::string& s = "");
        static void commandDefineOutputFilepath(const std::string& s = "");
        static void commandActivateVerboseMode(const std::string& s = "");
        static void commandKeepAllFiles(const std::string& s = "");
        static void commandDefineBuildType(const std::string& s = "");
        static void commandOptionNotFound(const std::string& s = "");
        static std::string getOptionValue(const std::string& option);

    private:
        static std::vector<Vanir::CLIOption> m_options;
        static std::string m_inputFilepath;
        static std::string m_outputFilepath;
        static std::string m_tempIRFilepath;
        static std::string m_tempOBJFilepath;
        static std::string m_tempEXEFilepath;
        static std::string m_tempArgv;
        static bool m_verboseMode;
        static bool m_keepAllFiles;
        static BuildType m_buildType;
        static bool m_doLexerPass;
        static bool m_doParserPass;
        static bool m_doCodeGeneratorPass;
    };

} /* Namespace ARC. */

#endif /* ARYIELE_CORE_ARC_H. */
