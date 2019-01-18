#ifndef ARYIELE_LEXERTOKEN_H
#define ARYIELE_LEXERTOKEN_H

#include <string>
#include <Aryiele/Tokenizer/TokenizerTokens.h>

namespace Aryiele
{
    struct TokenizerToken
    {
        TokenizerToken(const std::string& content = std::string(), TokenizerTokens type = TokenizerTokens_Unknown);

        std::string Content;
        TokenizerTokens Type;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_LEXERTOKEN_H. */
