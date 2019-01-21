#include <Aryiele/Core/Includes.h>
#include <Aryiele/Lexer/Lexer.h>
#ifdef PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <memory>
#include <Vanir/FileUtils.h>
#include <Aryiele/Parser/Parser.h>
#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/Bitcode/BitcodeReader.h>
#include <stdio.h>
#include <Vanir/JSONFile.h>
#include <filesystem>
int main(const int argc, char *argv[])
{
#ifdef _WIN32
    FILE* stream;
    freopen_s(&stream, "CONOUT$", "w+", stdout);
#endif

    Vanir::Logger::Start("data/logs.log");

    LOG(Vanir::LoggerColor(), "--------------------------------------[ ARYIELE COMPILER ]---------------------------------------");

    LOG_INFO("Loading configuration...");

    Vanir::JSONFile jsonFile;
    std::string filepath = std::string();
    
    jsonFile.Load("data/arc.json");

    std::string staticCompilerPath = std::string();
    std::string nativeAssemblerPath = std::string();
    bool isDebug = false;

    if (!jsonFile.Content["Debug"]["State"].is_null())
        isDebug = jsonFile.Content["Debug"]["State"].get<bool>();
    else
    {
#ifdef CONFIGURATION_DEBUG
        jsonFile.Content["Debug"]["State"] = true;
        isDebug = true;
#else
        jsonFile.Content["Debug"]["State"] = false;
        isDebug = false;
#endif
    }

    if (!jsonFile.Content["Tools"]["StaticCompiler"].is_null())
        staticCompilerPath = jsonFile.Content["Tools"]["StaticCompiler"].get<std::string>();
    else
    {
#ifdef PLATFORM_WINDOWS
        if (Vanir::FileUtils::FileExist("data/llvm/bin/llc.exe"))
            jsonFile.Content["Tools"]["StaticCompiler"] = "data/llvm/bin/llc.exe";
#else
        if (Vanir::FileUtils::FileExist("data/llvm/bin/llc"))
            jsonFile.Content["Tools"]["StaticCompiler"] = "data/llvm/bin/llc";
#endif
        jsonFile.Content["Tools"]["StaticCompiler"] = "";
    }
    if (!jsonFile.Content["Tools"]["NativeAssembler"].is_null())
        nativeAssemblerPath = jsonFile.Content["Tools"]["NativeAssembler"].get<std::string>();
    else
    {
#ifdef PLATFORM_WINDOWS
        if (Vanir::FileUtils::FileExist("data/gnu/bin/gcc.exe"))
            jsonFile.Content["Tools"]["NativeAssembler"] = "data/gnu/bin/gcc.exe";
#else
        if (Vanir::FileUtils::FileExist("data/gnu/bin/gcc"))
            jsonFile.Content["Tools"]["NativeAssembler"] = "data/gnu/bin/gcc";
#endif
        jsonFile.Content["Tools"]["NativeAssembler"] = "";
    }

    if (!jsonFile.Content["Debug"]["Filepath"].is_null())
        filepath = jsonFile.Content["Debug"]["Filepath"].get<std::string>();
    else
    {
        jsonFile.Content["Debug"]["Filepath"] = "../debug.ac";

        if (isDebug)
            filepath = "../debug.ac";
    }

    if (isDebug)
    {
        filepath = argc < 2 ? filepath : argv[1];
    }
    else
    {
        if (argc > 2)
            filepath = argv[1];
    }

    LOG_INFO("Configuration loaded.");

    if (staticCompilerPath.empty())
    {
        LOG_WARNING("No static compiler provided (see data/arc.json to edit path).");
        ULOG_WARNING("└╴LLC (LLVM Static Compiler) is recommended.");
    }
    if (nativeAssemblerPath.empty())
    {
        LOG_WARNING("No native assembler/linker provided (see data/arc.json to edit path).");
        ULOG_WARNING("└╴GCC (GNU Compiler Collection) is recommended.");
    }

    if (!Vanir::FileUtils::FileExist(filepath))
    {
        LOG_ERROR("No input file provided.");
    }
    else
    {

#ifdef CONFIGURATION_DEBUG
        LOG_INFO("Reading code...");

        std::ifstream infile(filepath);

        for( std::string line; getline( infile, line ); )
            LOG_INFO(line);

        LOG_INFO("Code readed.");

        infile.close();
#endif

        auto lexer = Aryiele::Lexer::GetInstance();

        LOG_INFO("--> Lexing...");

        auto lexerTokens = lexer->Tokenize(filepath);

        for (auto& token : lexerTokens)
        {
            LOG_INFO(lexer->GetTokenName(token).c_str(), ": ", token.Content.c_str());

            if (token.Type == Aryiele::LexerTokens_Unknown)
                return 20100001;
        }

        LOG_INFO("--> Lexing finished.");

        auto parser = Aryiele::Parser::GetInstance();

        LOG_INFO("--> Parsing...");

        auto parserTokens = parser->ConvertTokens(lexerTokens);

        LOG_INFO("-> Creating parsing tokens...");

        for (auto& token : parserTokens)
        {
            LOG_INFO(parser->GetTokenName(token.Type).c_str(), ": ", token.Content.c_str());

            if (token.Type == Aryiele::ParserTokens_Unknown)
                return 20100002;
        }

        LOG_INFO("-> Parsing tokens created.");

        parser->Parse(parserTokens);

        LOG_INFO("--> Parsing finished.");

        auto codeGenerator = Aryiele::CodeGenerator::GetInstance();

        LOG_INFO("--> Generating code...");

        codeGenerator->GenerateCode(parser->GetNodes());

        auto irFilepath = Vanir::FileUtils::GetFilePathWithoutExtension(filepath) + ".ll";

        std::error_code EC;
        llvm::raw_fd_ostream OS(irFilepath, EC, llvm::sys::fs::F_None);
        codeGenerator->Module->print(OS, nullptr);
        OS.flush();

        auto infile2 = std::ifstream(irFilepath);

        for( std::string line; getline( infile2, line ); )
            LOG_INFO(line);

        infile2.close();

        LOG_INFO("--> Code generated.");

        parser.reset();
    }

    LOG(Vanir::LoggerColor(), "-------------------------------------------------------------------------------------------------");

    jsonFile.Save();

    Vanir::Logger::Stop();

    return 0;
}
