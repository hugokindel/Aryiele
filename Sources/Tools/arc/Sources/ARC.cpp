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

// TODO: Simulate llc compilation to avoid executable dependencies.

#ifdef PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <utility>
#include <stdio.h>
#include <memory>
#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/Bitcode/BitcodeReader.h>
#include <Vanir/FileSystem/FileSystem.h>
#include <Aryiele/Core/Includes.h>
#include <Aryiele/Lexer/Lexer.h>
#include <Aryiele/Parser/Parser.h>
#include <Aryiele/CodeGenerator/CodeGenerator.h>
#include <ARC/ARC.h>

namespace ARC
{
    std::vector<Vanir::Argument> ARC::m_options;
    std::string ARC::m_inputFilepath;
    std::string ARC::m_outputFilepath;
    std::string ARC::m_tempIRFilepath;
    std::string ARC::m_tempOBJFilepath;
    std::string ARC::m_tempEXEFilepath;
    std::string ARC::m_tempArgv;
#ifndef FINAL_RELEASE
    bool ARC::m_verboseMode = true;
    bool ARC::m_keepAllFiles = true;
#else
    bool ARC::m_verboseMode;
    bool ARC::m_keepAllFiles;
#endif

#ifndef FINAL_RELEASE
    BuildTypes ARC::m_buildType = BuildTypes_Executable; // TODO: Put IR sometimes
#else
    BuildTypes ARC::m_buildType = BuildTypes_Executable;
#endif

    int ARC::Run(const int argc, char *argv[])
    {
        Aryiele::Lexer::Start();
        Aryiele::Parser::Start();
        Aryiele::CodeGenerator::Start();
        
#ifdef _WIN32
        FILE* stream;
    freopen_s(&stream, "CONOUT$", "w+", stdout);
#endif

#ifndef FINAL_RELEASE
        Vanir::Logger::StartNoLog();
#endif

        m_options.emplace_back(
            "--help",
            std::vector<std::string>({
                "Display this information."
            }),
            &ARC::CommandShowHelp);
        m_options.emplace_back(
            "--version", std::vector<std::string>({
                "Display version information."
            }),
            &ARC::CommandShowVersion);
        m_options.emplace_back(
            "-k", std::vector<std::string>({
                "Keep all files after compilation."
            }),
            &ARC::CommandKeepAllFiles);
        m_options.emplace_back(
            "-v", std::vector<std::string>({
                "Activate the verbose mode.",
                "informations about the compilation."
            }),
            &ARC::CommandActivateVerboseMode);
        m_options.emplace_back(
            "-out", std::vector<std::string>({
                "Define the output file at <filename>."
            }),
            &ARC::CommandDefineOutputFilepath,
            Vanir::ArgumentType_Value,
            "<filename>");
        m_options.emplace_back(
            "-type", std::vector<std::string>({
                "Choose a build type (default is exe):"
            }),
            &ARC::CommandDefineBuildType,
            Vanir::ArgumentType_Value,
            std::string(),
            std::vector<Vanir::Argument>({
                Vanir::Argument(
                    "ir", std::vector<std::string>({
                        "Emit an LLVM IR ('.ll') file."
                    })),
                Vanir::Argument(
                    "obj",
                    std::vector<std::string>({
                        "Emit a native object ('.o') file."
                    })),
                Vanir::Argument(
                    "exe",
                    std::vector<std::string>({
#ifdef PLATFORM_WINDOWS
                        "Emit a native executable ('.exe') file."
#else
                        "Emit a native executable file."
#endif
                    }))
            }));

#ifdef FINAL_RELEASE
        if(argc < 2)
        {
            LOG("");
            LOG("                ARC -- Aryiele Compiler");
            LOG("");
            LOG("Usage:");
            LOG("    arc [options] <file>");
            LOG("        Compile Aryiele source code to a native executable.")
            LOG("        Example: arc main.ac")
            LOG("");
            LOG("arc --help  shows full documentation on the options.");
        }
#endif

        for (int i = 1; i < argc; i++)
        {
            m_tempArgv = argv[i];
            bool found = false;

            for (auto y : m_options)
            {
                if (y.Type == Vanir::ArgumentType_Argument)
                {
                    if (!strcmp(m_tempArgv.c_str(), y.Name.c_str()))
                    {
                        y.FunctionToCall(Vanir::CLI::GetValueFromPassedArgument(m_tempArgv));

                        found = true;

                        break;
                    }
                }
                else
                {
                    if (m_tempArgv.rfind(y.Name + "=", 0) == 0)
                    {
                        if (m_tempArgv.size() > y.Name.size() + 1)
                        {
                            y.FunctionToCall(Vanir::CLI::GetValueFromPassedArgument(m_tempArgv));

                            found = true;

                            break;
                        }
                        else
                        {
                            LOG_WARNING("You cannot use an empty value for option: '", y.Name, "'");
                        }
                    }
                }
            }

            if (!found)
            {
                if (std::string(argv[i]).rfind('-', 0) == 0)
                    CommandOptionNotFound();
                else
                    CommandDefineInputFilepath();
            }
        }

#ifndef FINAL_RELEASE
        if (m_inputFilepath.empty())
            m_inputFilepath = "../../../../Examples/example-1.ac";
#endif

        if (!m_inputFilepath.empty())
        {
            if (!Vanir::FileSystem::FileExist(m_inputFilepath))
            {
                LOG_WARNING(m_inputFilepath, ": no such file or directory");
                LOG_ERROR("no input file");
            }
            else
            {
                Vanir::Logger::ResetCounters();

                auto lexerPass = DoLexerPass(m_inputFilepath);

                ARC_RUN_CHECKERRORS();

                auto parserPass = DoParserPass(lexerPass);

                ARC_RUN_CHECKERRORS();

                DoCodeGeneratorPass(parserPass);

                if (m_buildType == BuildTypes_Object || m_buildType == BuildTypes_Executable)
                {
                    ARC_RUN_CHECKERRORS();

                    DoObjectGeneratorPass();

                    if (!m_keepAllFiles)
                        remove(m_tempIRFilepath.c_str());
                }

                if (m_buildType == BuildTypes_Executable)
                {
                    ARC_RUN_CHECKERRORS();

                    DoExecutableGeneratorPass();

                    if (!m_keepAllFiles)
                        remove(m_tempOBJFilepath.c_str());
                }
            }
        }

#ifndef FINAL_RELEASE
        Vanir::Logger::Stop();
#endif
        
        Aryiele::CodeGenerator::Shutdown();
        Aryiele::Parser::Shutdown();
        Aryiele::Lexer::Shutdown();

        return 0;
    }

    std::vector<Aryiele::LexerToken> ARC::DoLexerPass(const std::string& filepath)
    {
        auto lexer = Aryiele::Lexer::GetInstancePtr();

        auto lexerTokens = lexer->Tokenize(filepath);

        for (auto& token : lexerTokens)
        {
            if (m_verboseMode)
            {
                LOG_VERBOSE("lexer: ", token.Content, " => ", lexer->GetTokenName(token));
            }

            if (token.Type == Aryiele::LexerTokens_Unknown)
            {
                LOG_ERROR("lexer: unknown token");
            }
        }

        return lexerTokens;
    }

    std::vector<std::shared_ptr<Aryiele::Node>> ARC::DoParserPass(std::vector<Aryiele::LexerToken> lexerTokens)
    {
        auto parser = Aryiele::Parser::GetInstancePtr();

        auto parserTokens = parser->ConvertTokens(std::move(lexerTokens));

        for (auto& token : parserTokens)
        {
            if (m_verboseMode)
            {
                LOG_VERBOSE("parser: ", token.Content, " => ", parser->GetTokenName(token.Type));
            }

            if (token.Type == Aryiele::ParserTokens_Unknown)
            {
                LOG_ERROR("parser: unknown token");
            }
        }

        auto nodes = parser->Parse(parserTokens);

        if (m_verboseMode)
        {
            auto dumpNode = std::make_shared<Aryiele::ParserInformation>(nullptr, "AST");

            // TODO: Small memory leak coming from here (see valgrind)
            for (auto& node : nodes)
                node->DumpInformations(dumpNode);

            DumpASTInformations(dumpNode);

            dumpNode.reset();
        }

        return nodes;
    }

    void ARC::DoCodeGeneratorPass(std::vector<std::shared_ptr<Aryiele::Node>> astNodes)
    {
        auto codeGenerator = Aryiele::CodeGenerator::GetInstancePtr();

        codeGenerator->GenerateCode(std::move(astNodes));

        if (::Vanir::Logger::ErrorCount > 0)
        {
            LOG_ERROR("ir code generation failed with ", ::Vanir::Logger::ErrorCount, " errors");
        }
        else
        {
            if (m_verboseMode)
            {
                LOG_VERBOSE("ir code generated with success");
            }

            m_tempIRFilepath = Vanir::FileSystem::GetPathWithoutExtension(m_inputFilepath) + ".ll";

            if (!m_outputFilepath.empty())
            {
                switch (m_buildType)
                {
                    case BuildTypes_IR:
                        m_tempIRFilepath = m_outputFilepath;
                        break;
                    default:
                        m_tempIRFilepath = Vanir::FileSystem::GetPathWithoutExtension(m_outputFilepath) + ".ll";
                        break;
                }
            }

            std::error_code errorCode;
            llvm::raw_fd_ostream ostream(m_tempIRFilepath, errorCode, llvm::sys::fs::F_None);

            codeGenerator->GetModule()->print(ostream, nullptr);
            ostream.flush();

            if (m_verboseMode)
            {
                LOG_VERBOSE("ir code file generated with success");
            }
        }
    }

    void ARC::DoObjectGeneratorPass()
    {
        m_tempOBJFilepath = Vanir::FileSystem::GetPathWithoutExtension(m_inputFilepath) + ".o";

        if (!m_outputFilepath.empty())
        {
            switch (m_buildType)
            {
                case BuildTypes_Object:
                    m_tempOBJFilepath = m_outputFilepath;
                    break;
                default:
                    m_tempOBJFilepath = Vanir::FileSystem::GetPathWithoutExtension(m_outputFilepath) + ".o";
                    break;
            }
        }

        int errorCode;

        if(!(errorCode = system(("llc " + m_tempIRFilepath + " -filetype=obj -o=" + m_tempOBJFilepath).c_str())))
        {
            if (m_verboseMode)
            {
                LOG_VERBOSE("object file generated with success");
            }
        }
        else
        {
            LOG_WARNING("cannot generate object: command exited with code: ", errorCode);
        }
    }

    void ARC::DoExecutableGeneratorPass()
    {
        m_tempEXEFilepath = Vanir::FileSystem::GetPathWithoutExtension(m_inputFilepath);

        if (!m_outputFilepath.empty())
        {
            switch (m_buildType)
            {
                case BuildTypes_Executable:
                    m_tempEXEFilepath = m_outputFilepath;
                    break;
                default:
                    m_tempEXEFilepath = Vanir::FileSystem::GetPathWithoutExtension(m_outputFilepath);
                    break;
            }
        }

        int errorCode;

        if(!(errorCode = system(("gcc " + m_tempOBJFilepath + " -L../libs -lSTD -o " + m_tempEXEFilepath).c_str())))
        {
            if (m_verboseMode)
            {
                LOG_VERBOSE("executable file generated with success");
            }
        }
        else
        {
            LOG_WARNING("cannot generate executable: command exited with code: ", errorCode);
        }
    }

    void ARC::DumpASTInformations(const std::shared_ptr<Aryiele::ParserInformation>& node, std::string indent)
    {
        const auto isRoot = node->Parent == nullptr;
        const auto hasChildren = !node->Children.empty();
        auto isLastSibling = true;

        if (!isRoot)
        {
            isLastSibling = (static_cast<int>(std::distance(node->Parent->Children.begin(),
                                                            std::find(
                                                                node->Parent->Children.begin(),
                                                                node->Parent->Children.end(), node)))
                             == static_cast<int>(node->Parent->Children.size()) - 1);
        }

        if (isRoot)
        {
            ULOG_VERBOSE("ast: ", node->Name);
        }
        else
        {
            ULOG_VERBOSE("ast:", indent + (isLastSibling ? "└╴" : "├╴"), node->Name);
            indent += isLastSibling ? "  " : "│ ";
        }

        if (hasChildren)
        {
            for (auto& nodeChild : node->Children)
                DumpASTInformations(nodeChild, indent);
        }
    }

    void ARC::CommandShowHelp(const std::string& s)
    {
#ifdef PLATFORM_WINDOWS
        LOG("Usage: arc.exe [options] <file>");
#else
        LOG("Usage: arc [options] <file>");
#endif
        LOG("")

        Vanir::CLI::DrawOptions(m_options);
    }

    void ARC::CommandShowVersion(const std::string& s)
    {
        LOG("arc (Aryiele Compiler) ", ARC_VERSION);
    }

    void ARC::CommandDefineInputFilepath(const std::string& s)
    {
        m_inputFilepath = m_tempArgv;
    }

    void ARC::CommandDefineOutputFilepath(const std::string& s)
    {
        auto result = GetOptionValue(m_tempArgv);

        if (!result.empty())
        {
            m_outputFilepath = result;
        }
    }

    void ARC::CommandActivateVerboseMode(const std::string& s)
    {
        m_verboseMode = true;
    }

    void ARC::CommandKeepAllFiles(const std::string& s)
    {
        m_keepAllFiles = true;
    }

    void ARC::CommandDefineBuildType(const std::string& s)
    {
        auto result = GetOptionValue(m_tempArgv);

        if (!result.empty())
        {
            if (result == "ir")
            {
                m_buildType = BuildTypes_IR;
            }
            else if (result == "obj")
            {
                m_buildType = BuildTypes_Object;
            }
            else if (result == "exe")
            {
                m_buildType = BuildTypes_Executable;
            }
            else
            {
                ULOG_WARNING("arc: Unknown type value: ", m_tempArgv);
            }
        }
    }

    void ARC::CommandOptionNotFound(const std::string& s)
    {
        ULOG("arc: Unknown command line argument '", m_tempArgv, "'. Try: 'arc --help'")

        int distance = -1;
        std::string nearest = std::string();

        for (const auto& y : m_options)
        {
            auto optionDistance = Vanir::String::CalculateLevensteinDistance(y.Name, m_tempArgv);

            if (optionDistance < distance || distance == -1)
            {
                distance = optionDistance;
                nearest = y.Name;
            }
        }

        ULOG("arc: Did you mean '", nearest, "'?");
    }

    std::string ARC::GetOptionValue(const std::string &option)
    {
        auto optionName = option.substr(0, option.find('=') + 1);
        auto result = option.substr(option.find('=') + 1);

        if (result.empty())
        {
            LOG_WARNING("Empty value after option '", optionName, "'");

            return std::string();
        }
        else
        {
            return result;
        }
    }

} /* Namespace ARC. */