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
#include <ARC/Core/ARC.h>

namespace ARC {
    std::vector<Vanir::CLIOption> ARC::m_options;
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

    BuildType ARC::m_buildType = BuildType_Executable;
    
    int ARC::Run(const int argc, char *argv[]) {
        Aryiele::Lexer::start();
        Aryiele::Parser::start();
        Aryiele::CodeGenerator::start();

#ifdef _WIN32
        FILE* stream;
    freopen_s(&stream, "CONOUT$", "w+", stdout);
#endif

#ifndef FINAL_RELEASE
        Vanir::Logger::StartNoLog();
#endif
        
        m_options.emplace_back(
            std::vector<std::string>({"-h", "--help"}),
            &ARC::CommandShowHelp,
            std::vector<std::string>({"Display this information."}));
        m_options.emplace_back(
            std::vector<std::string>({"-V", "--version"}),
            &ARC::CommandShowVersion,
            std::vector<std::string>({"Display version information."}));
        m_options.emplace_back(
            std::vector<std::string>({"-k", "--keep-files"}),
            &ARC::CommandKeepAllFiles,
            std::vector<std::string>({"Keep all files after compilation."}));
        m_options.emplace_back(
            std::vector<std::string>({"-v", "--verbose"}),
            &ARC::CommandActivateVerboseMode,
            std::vector<std::string>({"Activate the verbose mode,", "which displays informations about the compilation."}));
        m_options.emplace_back(
            std::vector<std::string>({"-o", "--output-file"}),
            &ARC::CommandDefineOutputFilepath,
            std::vector<std::string>({"Define the output file at <file>."}),
            Vanir::CLIOptionType_OptionWithValue,
            "<filename>");
        m_options.emplace_back(
            std::vector<std::string>({"-t", "--build-type"}),
            &ARC::CommandDefineBuildType,
            std::vector<std::string>({"Choose a build type (default is exe):"}),
            Vanir::CLIOptionType_OptionWithValue,
            "<type>",
            std::vector<Vanir::CLIArgument>({
                Vanir::CLIArgument(
                    std::vector<std::string>({"irl"}),
                    std::vector<std::string>({"Emit an LLVM IR ('.ll') file."})),
                Vanir::CLIArgument(
                    std::vector<std::string>({"obj"}),
                    std::vector<std::string>({"Emit a native object ('.o') file."})),
                Vanir::CLIArgument(std::vector<std::string>({"exe"}),std::vector<std::string>({
#ifdef PLATFORM_WINDOWS
                        "Emit a native executable ('.exe') file."
#else
                        "Emit a native executable file."
#endif
                }))
            }));

#ifdef FINAL_RELEASE
        if(argc < 2) {
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

        auto result = Vanir::CLI::parse(argc, argv, m_options, false);

        if (result.errors.size() > 0) {
            LOG_WARNING("command interpretation had ", result.errors.size(), " errors")
            for (auto e : result.errors) {
                LOG_WARNING(Vanir::CLIParsingResult::errorToString(e))
            }
        }

        m_inputFilepath = result.result;

#ifndef FINAL_RELEASE
        if (m_inputFilepath.empty())
            m_inputFilepath = "../../../../Examples/example-1.ac";
#endif
        
        if (!m_inputFilepath.empty()) {
            if (!Vanir::FileSystem::fileExist(m_inputFilepath)) {
                LOG_WARNING(m_inputFilepath, ": no such file or directory")
                LOG_ERROR("no input file");
            }
            else {
                Vanir::Logger::resetCounters();
                
                auto lexerPass = DoLexerPass(m_inputFilepath);
                
                ARC_RUN_CHECKERRORS();
                
                auto parserPass = DoParserPass(lexerPass);
                
                ARC_RUN_CHECKERRORS();
                
                DoCodeGeneratorPass(parserPass);
                
                if (m_buildType == BuildType_Object || m_buildType == BuildType_Executable) {
                    ARC_RUN_CHECKERRORS();
                    
                    DoObjectGeneratorPass();
                    
                    if (!m_keepAllFiles)
                        remove(m_tempIRFilepath.c_str());
                }
                
                if (m_buildType == BuildType_Executable) {
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
        
        Aryiele::CodeGenerator::shutdown();
        Aryiele::Parser::shutdown();
        Aryiele::Lexer::shutdown();
        
        return 0;
    }
    
    std::vector<Aryiele::LexerToken> ARC::DoLexerPass(const std::string& filepath) {
        auto lexer = Aryiele::Lexer::getInstancePtr();
        
        auto lexerTokens = lexer->Tokenize(filepath);
        
        for (auto& token : lexerTokens) {
            if (m_verboseMode) {
                LOG_VERBOSE("lexer: ", token.Content, " => ", lexer->GetTokenName(token));
            }
            
            if (token.Type == Aryiele::LexerTokens_Unknown) {
                LOG_ERROR("lexer: unknown token");
            }
        }
        
        return lexerTokens;
    }
    
    std::vector<std::shared_ptr<Aryiele::Node>> ARC::DoParserPass(std::vector<Aryiele::LexerToken> lexerTokens) {
        auto parser = Aryiele::Parser::getInstancePtr();
        
        auto parserTokens = parser->ConvertTokens(std::move(lexerTokens));
        
        for (auto& token : parserTokens) {
            if (m_verboseMode) {
                LOG_VERBOSE("parser: ", token.Content, " => ", parser->GetTokenName(token.Type));
            }
            
            if (token.Type == Aryiele::ParserTokens_Unknown) {
                LOG_ERROR("parser: unknown token");
            }
        }
        
        auto nodes = parser->Parse(parserTokens);
        
        if (m_verboseMode) {
            auto dumpNode = std::make_shared<Aryiele::ParserInformation>(nullptr, "AST");
            
            // TODO: Small memory leak coming from here (see valgrind)
            for (auto& node : nodes)
                node->DumpInformations(dumpNode);
            
            DumpASTInformations(dumpNode, " ");
            
            dumpNode.reset();
        }
        
        return nodes;
    }
    
    void ARC::DoCodeGeneratorPass(std::vector<std::shared_ptr<Aryiele::Node>> astNodes) {
        auto codeGenerator = Aryiele::CodeGenerator::getInstancePtr();
        
        codeGenerator->GenerateCode(std::move(astNodes));
        
        if (::Vanir::Logger::errorCount > 0) {
            LOG_ERROR("ir code generation failed with ", ::Vanir::Logger::errorCount, " errors");
        }
        else {
            if (m_verboseMode) {
                LOG_VERBOSE("ir code generated with success");
            }
            
            m_tempIRFilepath = Vanir::FileSystem::getFilePath(m_inputFilepath) + ".ll";
            
            if (!m_outputFilepath.empty()) {
                switch (m_buildType) {
                    case BuildType_IR:
                        m_tempIRFilepath = m_outputFilepath;
                        break;
                    default:
                        m_tempIRFilepath = Vanir::FileSystem::getFilePath(m_outputFilepath) + ".ll";
                        break;
                }
            }
            
            std::error_code errorCode;
            llvm::raw_fd_ostream ostream(m_tempIRFilepath, errorCode, llvm::sys::fs::F_None);
            
            codeGenerator->GetModule()->print(ostream, nullptr);
            ostream.flush();
            
            if (m_verboseMode) {
                LOG_VERBOSE("ir code file generated with success");
            }
        }
    }
    
    void ARC::DoObjectGeneratorPass() {
        m_tempOBJFilepath = Vanir::FileSystem::getFilePath(m_inputFilepath) + ".o";
        
        if (!m_outputFilepath.empty()) {
            switch (m_buildType) {
                case BuildType_Object:
                    m_tempOBJFilepath = m_outputFilepath;
                    break;
                default:
                    m_tempOBJFilepath = Vanir::FileSystem::getFilePath(m_outputFilepath) + ".o";
                    break;
            }
        }
        
        int errorCode;
        
        if(!(errorCode = system(("llc " + m_tempIRFilepath + " -filetype=obj -o=" + m_tempOBJFilepath).c_str()))) {
            if (m_verboseMode) {
                LOG_VERBOSE("object file generated with success");
            }
        }
        else {
            LOG_WARNING("cannot generate object: command exited with code: ", errorCode);
        }
    }
    
    void ARC::DoExecutableGeneratorPass() {
        m_tempEXEFilepath = Vanir::FileSystem::getFilePath(m_inputFilepath);
        
        if (!m_outputFilepath.empty()) {
            switch (m_buildType) {
                case BuildType_Executable:
                    m_tempEXEFilepath = m_outputFilepath;
                    break;
                default:
                    m_tempEXEFilepath = Vanir::FileSystem::getFilePath(m_outputFilepath);
                    break;
            }
        }
        
        int errorCode;
        
        if(!(errorCode = system(("gcc " + m_tempOBJFilepath + " -L../libs -lSTD -o " + m_tempEXEFilepath).c_str()))) {
            if (m_verboseMode) {
                LOG_VERBOSE("executable file generated with success");
            }
        }
        else {
            LOG_WARNING("cannot generate executable: command exited with code: ", errorCode);
        }
    }
    
    void ARC::DumpASTInformations(const std::shared_ptr<Aryiele::ParserInformation>& node, std::string indent) {
        const auto isRoot = node->Parent == nullptr;
        const auto hasChildren = !node->Children.empty();
        auto isLastSibling = true;
        
        if (!isRoot) {
            isLastSibling = (static_cast<int>(std::distance(node->Parent->Children.begin(),
                                std::find(node->Parent->Children.begin(), node->Parent->Children.end(), node)))
                             == static_cast<int>(node->Parent->Children.size()) - 1);
        }
        
        if (isRoot) {
            ULOG_VERBOSE("ast: ", node->Name);
        }
        else {
            ULOG_VERBOSE("ast:", indent + (isLastSibling ? "└╴" : "├╴"), node->Name);
            indent += isLastSibling ? "  " : "│ ";
        }
        
        if (hasChildren) {
            for (auto& nodeChild : node->Children)
                DumpASTInformations(nodeChild, indent);
        }
    }
    
    void ARC::CommandShowHelp(const std::string& s) {
#ifdef PLATFORM_WINDOWS
        LOG("Usage: arc.exe [options] <file>");
#else
        LOG("Usage: arc [options] <file>");
#endif
        LOG("")
        
        auto help = Vanir::CLI::getOptionsDescriptionList(m_options);
        
        for (auto& hLine : help) {
            LOG(hLine);
        }
    }
    
    void ARC::CommandShowVersion(const std::string& s) {
        LOG("arc (Aryiele Compiler) ", ARC_VERSION);
    }
    
    void ARC::CommandDefineInputFilepath(const std::string& s) {
        m_inputFilepath = s;
    }
    
    void ARC::CommandDefineOutputFilepath(const std::string& s) {
        auto result = GetOptionValue(s);
        
        if (!result.empty()) {
            m_outputFilepath = result;
        }
    }
    
    void ARC::CommandActivateVerboseMode(const std::string& s) {
        m_verboseMode = true;
    }
    
    void ARC::CommandKeepAllFiles(const std::string& s) {
        m_keepAllFiles = true;
    }
    
    void ARC::CommandDefineBuildType(const std::string& s) {
        auto result = GetOptionValue(s);
        
        if (!result.empty()) {
            if (result == "ir") {
                m_buildType = BuildType_IR;
            }
            else if (result == "obj") {
                m_buildType = BuildType_Object;
            }
            else if (result == "exe") {
                m_buildType = BuildType_Executable;
            }
            else {
                ULOG_WARNING("arc: unknown type value: ", s);
            }
        }
    }
    
    void ARC::CommandOptionNotFound(const std::string& s) {
        ULOG("arc: unknown command line argument '", s, "'. Try: 'arc --help'")
        ULOG("arc: did you mean '", Vanir::CLI::findClosestOption(s, m_options), "'?");
    }
    
    std::string ARC::GetOptionValue(const std::string &option) {
        auto optionName = option.substr(0, option.find('=') + 1);
        auto result = option.substr(option.find('=') + 1);
        
        if (result.empty()) {
            LOG_WARNING("empty value after option '", optionName, "'");
            
            return std::string();
        }
        else {
            return result;
        }
    }
    
} /* Namespace ARC. */
