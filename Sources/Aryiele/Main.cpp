#include <Aryiele/Core/Includes.h>
#include <Aryiele/Tokenizer/Tokenizer.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <memory>

int main(const int argc, char *argv[])
{
#ifdef _WIN32
    FILE* stream;
    freopen_s(&stream, "CONOUT$", "w+", stdout);
#endif

    Vanir::Logger::Start();

    LOG_DEFAULT("-------------------------------------------- ARYIELE --------------------------------------------");

    std::string filename = argc < 2 ? "../test.as" : argv[1];

    auto tokenizer = std::make_shared<Aryiele::Tokenizer>();

    LOG_INFO("--> Tokenizing...");

    for (auto& token : tokenizer->Tokenize(filename))
        LOG_INFO("%s: %s", tokenizer->GetTokenName(token).c_str(), token.Content.c_str());

    LOG_INFO("--> Tokenizing finished.");

    LOG_DEFAULT("-------------------------------------------------------------------------------------------------");

    Vanir::Logger::Stop();

    return 0;
}
