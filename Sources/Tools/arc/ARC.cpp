#include <arc/ARC.h>
#include <stdio.h>
#include <memory>
#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/Bitcode/BitcodeReader.h>
#include <Aryiele/Core/Includes.h>
#include <Aryiele/Lexer/Lexer.h>
#include <Aryiele/Parser/Parser.h>
#include <Vanir/FileUtils.h>
#include <Vanir/JSONFile.h>
#include "ARC.h"

#ifdef PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace ARC
{
    ARC::ARC() :
        m_buildType(BuildType_Executable)
    {

    }

    int ARC::Run(const int argc, char *argv[])
    {
#ifdef _WIN32
        FILE* stream;
    freopen_s(&stream, "CONOUT$", "w+", stdout);
#endif

#ifndef FINAL_RELEASE
        Vanir::Logger::Start("data/logs.log");
#else
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
            LOG("arc --help  shows full documentation on the options.")
            LOG("");
        }
#endif

        for (int i = 1; i < argc; i++)
        {
            if (!strcmp(argv[i], "--help"))
                CommandShowHelp();
            else if (!strcmp(argv[i], "--version"))
                CommandShowVersion();
            else if (std::string(argv[i]).rfind("-o=", 0) == 0)
                CommandDefineOutputFilepath(argv[i]);
            else if (std::string(argv[i]).rfind("-type=", 0) == 0)
                CommandDefineBuildType(argv[i]);
            else if (!strcmp(argv[i], "-v"))
                CommandActivateVerboseMode();
            else if (!strcmp(argv[i], "-k"))
                CommandKeepAllFiles();
            else if (std::string(argv[i]).rfind('-', 0) == 0)
                CommandOptionNotFound(argv[i]);
            else
                CommandDefineInputFilepath(argv[i]);
        }

#ifndef FINAL_RELEASE
        if (m_inputFilepath.empty())
            m_inputFilepath = "../debug.ac";
#endif

        if (!m_inputFilepath.empty())
        {
            if (!Vanir::FileUtils::FileExist(m_inputFilepath))
            {
                ALOG_WARNING(m_inputFilepath, ": no such file or directory");
                ALOG_ERROR("no input file");
            }
            else
            {
                DoCodeGeneratorPass(DoParserPass(DoLexerPass(m_inputFilepath)));

                if (m_buildType == BuildType_Object || m_buildType == BuildType_Executable)
                {
                    DoObjectGeneratorPass();

                    if (!m_keepAllFiles)
                        remove(m_tempIRFilepath.c_str());
                }
                if (m_buildType == BuildType_Executable)
                {
                    DoExecutableGeneratorPass();

                    if (!m_keepAllFiles)
                        remove(m_tempOBJFilepath.c_str());
                }
            }
        }

#ifndef FINAL_RELEASE
        Vanir::Logger::Stop();
#endif

        return 0;
    }

    std::vector<Aryiele::LexerToken> ARC::DoLexerPass(const std::string& filepath)
    {
        auto lexer = Aryiele::Lexer::GetInstance();

        auto lexerTokens = lexer->Tokenize(filepath);

        for (auto& token : lexerTokens)
        {
            if (m_verboseMode)
            {
                ALOG_VERBOSE("lexer: ", token.Content, " => ", lexer->GetTokenName(token));
            }

            if (token.Type == Aryiele::LexerTokens_Unknown)
            {
                ALOG_ERROR("lexer: unknown token");
            }
        }

        return lexerTokens;
    }

    std::vector<std::shared_ptr<Aryiele::Node>> ARC::DoParserPass(std::vector<Aryiele::LexerToken> lexerTokens)
    {
        auto parser = Aryiele::Parser::GetInstance();

        auto parserTokens = parser->ConvertTokens(lexerTokens);

        for (auto& token : parserTokens)
        {
            if (m_verboseMode)
            {
                ALOG_VERBOSE("parser: ", token.Content, " => ", parser->GetTokenName(token.Type));
            }

            if (token.Type == Aryiele::ParserTokens_Unknown)
            {
                ALOG_ERROR("parser: unknown token");
            }
        }

        auto nodes = parser->Parse(parserTokens);

        if (m_verboseMode)
        {
            auto dumpNode = std::make_shared<Aryiele::ParserInformation>(nullptr, "AST");

            for (auto& node : nodes)
                node->DumpInformations(dumpNode);

            DumpASTInformations(dumpNode);
        }

        return nodes;
    }

    void ARC::DoCodeGeneratorPass(std::vector<std::shared_ptr<Aryiele::Node>> astNodes)
    {
        auto codeGenerator = Aryiele::CodeGenerator::GetInstance();

        codeGenerator->GenerateCode(astNodes);

        m_tempIRFilepath = Vanir::FileUtils::GetFilePathWithoutExtension(m_inputFilepath) + ".ll";

        if (!m_outputFilepath.empty())
        {
            switch (m_buildType)
            {
                case BuildType_IR:
                    m_tempIRFilepath = m_outputFilepath;
                    break;
                default:
                    m_tempIRFilepath = Vanir::FileUtils::GetFilePathWithoutExtension(m_outputFilepath) + ".ll";
                    break;
            }
        }

        std::error_code errorCode;
        llvm::raw_fd_ostream ostream(m_tempIRFilepath, errorCode, llvm::sys::fs::F_None);

        codeGenerator->Module->print(ostream, nullptr);
        ostream.flush();

        if (m_verboseMode)
        {
            ALOG_VERBOSE("ir code generated with success");
        }
    }

    void ARC::DoObjectGeneratorPass()
    {
        m_tempOBJFilepath = Vanir::FileUtils::GetFilePathWithoutExtension(m_inputFilepath) + ".o";

        if (!m_outputFilepath.empty())
        {
            switch (m_buildType)
            {
                case BuildType_Object:
                    m_tempOBJFilepath = m_outputFilepath;
                    break;
                default:
                    m_tempOBJFilepath = Vanir::FileUtils::GetFilePathWithoutExtension(m_outputFilepath) + ".o";
                    break;
            }
        }

        int errorCode;

        if(!(errorCode = system(("llc " + m_tempIRFilepath + " -filetype=obj -o=" + m_tempOBJFilepath).c_str())))
        {
            if (m_verboseMode)
            {
                ALOG_VERBOSE("object generated with success");
            }
        }
        else
        {
            ALOG_WARNING("cannot generate object: command exited with code: ", errorCode);
        }
    }

    void ARC::DoExecutableGeneratorPass()
    {
        m_tempEXEFilepath = Vanir::FileUtils::GetFilePathWithoutExtension(m_inputFilepath);

        if (!m_outputFilepath.empty())
        {
            switch (m_buildType)
            {
                case BuildType_Executable:
                    m_tempEXEFilepath = m_outputFilepath;
                    break;
                default:
                    m_tempEXEFilepath = Vanir::FileUtils::GetFilePathWithoutExtension(m_outputFilepath);
                    break;
            }
        }

        int errorCode;

        if(!(errorCode = system(("gcc " + m_tempOBJFilepath + " -o " + m_tempEXEFilepath).c_str())))
        {
            if (m_verboseMode)
            {
                ALOG_VERBOSE("executable generated with success");
            }
        }
        else
        {
            ALOG_WARNING("cannot generate executable: command exited with code: ", errorCode);
        }
    }

    void ARC::DumpASTInformations(const std::shared_ptr<Aryiele::ParserInformation>& node, std::string indent) const
    {
        const auto isRoot = node->Parent == nullptr;
        const auto hasChildren = !node->Children.empty();
        auto isLastSibling = true;

        if (!isRoot)
        {
            isLastSibling = (static_cast<int>(std::distance(node->Parent->Children.begin(),
                                                            std::find(node->Parent->Children.begin(), node->Parent->Children.end(), node)))
                             == static_cast<int>(node->Parent->Children.size()) - 1);
        }

        if (isRoot)
        {
            AULOG_VERBOSE("ast: ", node->Name);
        }
        else
        {
            AULOG_VERBOSE("ast :", indent + (isLastSibling ? "└╴" : "├╴"), node->Name);
            indent += isLastSibling ? "  " : "│ ";
        }

        if (hasChildren)
        {
            for (auto& nodeChild : node->Children)
                DumpASTInformations(nodeChild, indent);
        }
    }

    void ARC::CommandShowHelp()
    {
#ifdef PLATFORM_WINDOWS
        LOG("Usage: arc.exe [options] <file>");
#else
        LOG("Usage: arc [options] <file>");
#endif
        LOG("")
        LOG("Options:")
        LOG("  --help         Display this information.");
        LOG("  --version      Display version information.");
        LOG("  -o=<filename>  Define the output file at <filename>.");
        LOG("  -v             Activate the verbose mode to display");
        LOG("  -k             Keep all files after compilation.");
        LOG("                 informations about the compilation.")
        LOG("  -type          Choose a build type (default is exe):");
        LOG("    =ir            Emit an LLVM IR ('.ll') file");
        LOG("    =obj           Emit a native object ('.o') file.");
#ifdef PLATFORM_WINDOWS
        LOG("    =exe           Emit a native executable ('.exe') file.");
#else
        LOG("    =exe           Emit a native executable file.");
#endif
    }

    void ARC::CommandShowVersion()
    {
        LOG("arc (Aryiele Compiler) ", ARC_VERSION);
    }

    void ARC::CommandDefineInputFilepath(const std::string &filepath)
    {
        m_inputFilepath = filepath;
    }

    void ARC::CommandDefineOutputFilepath(const std::string &filepath)
    {
        auto result = GetOptionValue(filepath);

        if (!result.empty())
        {
            m_outputFilepath = result;
        }
    }

    void ARC::CommandActivateVerboseMode()
    {
        m_verboseMode = true;
    }

    void ARC::CommandKeepAllFiles()
    {
        m_keepAllFiles = true;
    }

    void ARC::CommandDefineBuildType(const std::string& option)
    {
        auto result = GetOptionValue(option);

        if (!result.empty())
        {
            if (result == "ir")
            {
                m_buildType = BuildType_IR;
            }
            else if (result == "obj")
            {
                m_buildType = BuildType_Object;
            }
            else if (result == "exe")
            {
                m_buildType = BuildType_Executable;
            }
            else
            {
                ULOG_WARNING("arc: Unknown type value: ", option);
            }
        }
    }

    void ARC::CommandOptionNotFound(const std::string &option)
    {
        ULOG("arc: Unknown command line argument '", option, "'. Try: 'arc --help'")
    }

    std::string ARC::GetOptionValue(const std::string &option)
    {
        auto optionName = option.substr(0, option.find('=') + 1);
        auto result = option.substr(option.find('=') + 1);

        if (result.empty())
        {
            ALOG_WARNING("Empty value after option '", optionName, "'");
        }
        else
        {
            return result;
        }
    }

} /* Namespace ARC. */