#include <Aryiele/Core/Includes.h>
#include <Aryiele/Tokenizer/Tokenizer.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <memory>
#include <Vanir/FileUtils.h>
#include <Aryiele/Parser/Parser.h>

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
        auto tokenizer = std::make_shared<Aryiele::Tokenizer>();

        LOG_INFO("--> Tokenizing...");

        auto tokenizerTokens = tokenizer->Tokenize(filename);

        for (auto& token : tokenizerTokens)
        {
            LOG_INFO("%s: %s", tokenizer->GetTokenName(token).c_str(), token.Content.c_str());

            if (token.Type == Aryiele::TokenizerTokens_Unknown)
                return 20100001;
        }

        LOG_INFO("--> Tokenizing finished.");

        tokenizer.reset();

        auto parser = std::make_shared<Aryiele::Parser>();

        LOG_INFO("--> Parsing...");

        auto parserTokens = parser->ConvertTokens(tokenizerTokens);

        LOG_INFO("-> Creating parsing tokens...");

        for (auto& token : parserTokens)
        {
            LOG_INFO("%s: %s", parser->GetTokenName(token).c_str(), token.Content.c_str());

            if (token.Type == Aryiele::ParserTokens_Unknown)
                return 20100002;
        }

        LOG_INFO("-> Parsing tokens created.");

        LOG_INFO("--> Parsing finished.");

        parser.reset();
    }

    LOG_DEFAULT("-------------------------------------------------------------------------------------------------");

    Vanir::Logger::Stop();

    return 0;
}
