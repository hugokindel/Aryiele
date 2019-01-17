#include <Aryiele/Tokenizer/Token.h>

namespace Aryiele
{
    Token::Token(const std::string& content, const TokenTypes type)
    {
        Content = content;
        Type = type;
    }

} /* Namespace Aryiele. */