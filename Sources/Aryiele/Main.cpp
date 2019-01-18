#include <Aryiele/Core/Includes.h>
#include <Aryiele/Lexer/Lexer.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <memory>
#include <Vanir/FileUtils.h>
#include <Aryiele/Parser/Parser.h>

// TODO: Change Lexer -> Lexer

int main(const int argc, char *argv[])
{
#ifdef _WIN32
    FILE* stream;
    freopen_s(&stream, "CONOUT$", "w+", stdout);
#endif

    Vanir::Logger::Start();

    LOG_DEFAULT("-------------------------------------------- ARYIELE --------------------------------------------");

    std::string filename = argc < 2 ? "../test.as" : argv[1];

    if (!Vanir::FileUtils::FileExist(filename))
    {
        LOG_ERROR("No input file.");
    }
    else
    {
        auto lexer = std::make_shared<Aryiele::Lexer>();

        LOG_INFO("--> Lexing...");

        auto lexerTokens = lexer->Tokenize(filename);

        for (auto& token : lexerTokens)
        {
            LOG_INFO("%s: %s", lexer->GetTokenName(token).c_str(), token.Content.c_str());

            if (token.Type == Aryiele::LexerTokens_Unknown)
                return 20100001;
        }

        LOG_INFO("--> Lexing finished.");

        lexer.reset();

        auto parser = std::make_shared<Aryiele::Parser>();

        LOG_INFO("--> Parsing...");

        auto parserTokens = parser->ConvertTokens(lexerTokens);

        LOG_INFO("-> Creating parsing tokens...");

        for (auto& token : parserTokens)
        {
            LOG_INFO("%s: %s", parser->GetTokenName(token).c_str(), token.Content.c_str());

            if (token.Type == Aryiele::ParserTokens_Unknown)
                return 20100002;
        }

        LOG_INFO("-> Parsing tokens created.");

        parser->Parse(parserTokens);

        LOG_INFO("--> Parsing finished.");

        parser.reset();
    }

    LOG_DEFAULT("-------------------------------------------------------------------------------------------------");

    Vanir::Logger::Stop();

    return 0;
}
