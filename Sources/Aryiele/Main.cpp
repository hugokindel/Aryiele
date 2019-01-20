#include <Aryiele/Core/Includes.h>
#include <Aryiele/Lexer/Lexer.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <memory>
#include <Vanir/FileUtils.h>
#include <Aryiele/Parser/Parser.h>
#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/Bitcode/BitcodeReader.h>
#include <stdio.h>

int main(const int argc, char *argv[])
{
#ifdef _WIN32
    FILE* stream;
    freopen_s(&stream, "CONOUT$", "w+", stdout);
#endif

    Vanir::Logger::Start();

    LOG("-------------------------------------------- ARYIELE --------------------------------------------");

    std::string filename = argc < 2 ? "../test.as" : argv[1];

    if (!Vanir::FileUtils::FileExist(filename))
    {
        LOG_ERROR("No input file provided.");
    }
    else
    {
        LOG_INFO("Reading code...")

        std::ifstream infile(filename);

        for( std::string line; getline( infile, line ); )
            LOG_INFO(line);

        LOG_INFO("Code readed.")

        infile.close();

        auto lexer = Aryiele::Lexer::GetInstance();

        LOG_INFO("--> Lexing...");

        auto lexerTokens = lexer->Tokenize(filename);

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

        std::error_code EC;
        llvm::raw_fd_ostream OS("../test.ll", EC, llvm::sys::fs::F_None); // TODO: Filename without extension
        codeGenerator->Module->print(OS, nullptr);
        OS.flush();

        infile = std::ifstream("../test.ll"); // TODO: Filename without extension

        for( std::string line; getline( infile, line ); )
            LOG_INFO(line);

        infile.close();

        LOG_INFO("--> Code generated.");

        parser.reset();
    }

    LOG("-------------------------------------------------------------------------------------------------");

    Vanir::Logger::Stop();

    return 0;
}
