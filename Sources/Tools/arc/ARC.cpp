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
    std::vector<Vanir::CommandLineOption> ARC::m_options;
    std::string ARC::m_inputFilepath;
    std::string ARC::m_outputFilepath;
    std::string ARC::m_tempIRFilepath;
    std::string ARC::m_tempOBJFilepath;
    std::string ARC::m_tempEXEFilepath;
    std::string ARC::m_tempArgv;
#ifndef FINAL_RELEASE
    bool ARC::m_verboseMode = true;
#else
    bool ARC::m_verboseMode;
#endif
    bool ARC::m_keepAllFiles;
#ifndef FINAL_RELEASE
    BuildType ARC::m_buildType = BuildType_IR;
#else
    BuildType ARC::m_buildType = BuildType_Executable;
#endif

    int ARC::Run(const int argc, char *argv[])
    {
#ifdef _WIN32
        FILE* stream;
    freopen_s(&stream, "CONOUT$", "w+", stdout);
#endif

#ifndef FINAL_RELEASE
        Vanir::Logger::Start("data/logs.log");
#endif

        m_options.emplace_back(
            "--help", std::vector<std::string>({
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
                "Activate the verbose mode to display",
                "informations about the compilation."
            }),
            &ARC::CommandActivateVerboseMode);
        m_options.emplace_back(
            "-out", std::vector<std::string>({
                "Define the output file at <filename>."
            }),
            &ARC::CommandDefineOutputFilepath,
            Vanir::CommandLineOptionTypes_Value,
            "<filename>");
        m_options.emplace_back(
            "-type", std::vector<std::string>({
                "Choose a build type (default is exe):"
            }),
            &ARC::CommandDefineBuildType,
            Vanir::CommandLineOptionTypes_Value,
            std::string(),
            std::vector<Vanir::CommandLineOption>({
                Vanir::CommandLineOption(
                    "ir", std::vector<std::string>({
                        "Emit an LLVM IR ('.ll') file."
                    })),
                Vanir::CommandLineOption(
                    "obj",
                    std::vector<std::string>({
                        "Emit a native object ('.o') file."
                    })),
                Vanir::CommandLineOption(
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
            LOG("arc --help  shows full documentation on the options.")
            LOG("");
        }
#endif

        for (int i = 1; i < argc; i++)
        {
            m_tempArgv = argv[i];
            bool found = false;

            for (auto y : m_options)
            {
                if (y.Type == Vanir::CommandLineOptionTypes_Option)
                {
                    if (!strcmp(m_tempArgv.c_str(), y.Name.c_str()))
                    {
                        y.FunctionToCall();

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
                            y.FunctionToCall();

                            found = true;

                            break;
                        }
                        else
                        {
                            ALOG_WARNING("You cannot use an empty value for option: '", y.Name, "'");
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

        LOG_RESETCOUNTERS();

        codeGenerator->GenerateCode(astNodes);

        if (::Vanir::Logger::ErrorCount > 0)
        {
            if (m_verboseMode)
            {
                ALOG_VERBOSE("ir code generation failed with ", ::Vanir::Logger::ErrorCount, " errors");
            }
        }
        else
        {
            if (m_verboseMode)
            {
                ALOG_VERBOSE("ir code generated with success");
            }

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
                ALOG_VERBOSE("ir code file generated with success");
            }
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
                ALOG_VERBOSE("object file generated with success");
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
                ALOG_VERBOSE("executable file generated with success");
            }
        }
        else
        {
            ALOG_WARNING("cannot generate executable: command exited with code: ", errorCode);
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
                                                            std::find(node->Parent->Children.begin(), node->Parent->Children.end(), node)))
                             == static_cast<int>(node->Parent->Children.size()) - 1);
        }

        if (isRoot)
        {
            AULOG_VERBOSE("ast: ", node->Name);
        }
        else
        {
            AULOG_VERBOSE("ast:", indent + (isLastSibling ? "└╴" : "├╴"), node->Name);
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

        Vanir::CommandLineUtils::DrawOptions(m_options);
    }

    void ARC::CommandShowVersion()
    {
        LOG("arc (Aryiele Compiler) ", ARC_VERSION);
    }

    void ARC::CommandDefineInputFilepath()
    {
        m_inputFilepath = m_tempArgv;
    }

    void ARC::CommandDefineOutputFilepath()
    {
        auto result = GetOptionValue(m_tempArgv);

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

    void ARC::CommandDefineBuildType()
    {
        auto result = GetOptionValue(m_tempArgv);

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
                ULOG_WARNING("arc: Unknown type value: ", m_tempArgv);
            }
        }
    }

    void ARC::CommandOptionNotFound()
    {
        ULOG("arc: Unknown command line argument '", m_tempArgv, "'. Try: 'arc --help'")

        int distance = -1;
        std::string nearest = std::string();

        for (const auto& y : m_options)
        {
            auto optionDistance = Vanir::StringUtils::GeneralizedLevensteinDistance(y.Name, m_tempArgv);

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
            ALOG_WARNING("Empty value after option '", optionName, "'");
        }
        else
        {
            return result;
        }
    }

} /* Namespace ARC. */