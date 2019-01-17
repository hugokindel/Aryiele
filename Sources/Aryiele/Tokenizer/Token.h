#ifndef ARYIELE_TOKEN_H
#define ARYIELE_TOKEN_H

#include <string>
#include <Aryiele/Tokenizer/TokenTypes.h>

namespace Aryiele
{
    struct Token
    {
        Token(const std::string& content = "", TokenTypes type = Unknown);

        std::string Content;
        TokenTypes Type;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_TOKEN_H. */