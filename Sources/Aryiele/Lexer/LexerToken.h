#ifndef ARYIELE_LEXERTOKEN_H
#define ARYIELE_LEXERTOKEN_H

#include <string>
#include <Aryiele/Lexer/LexerTokens.h>

namespace Aryiele
{
    struct LexerToken
    {
        LexerToken(const std::string& content = std::string(), LexerTokens type = LexerTokens_Unknown);

        std::string Content;
        LexerTokens Type;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_LEXERTOKEN_H. */
