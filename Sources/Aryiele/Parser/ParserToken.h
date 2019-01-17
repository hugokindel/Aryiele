#ifndef ARYIELE_PARSERTOKEN_H
#define ARYIELE_PARSERTOKEN_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/Parser/ParserTokens.h>

namespace Aryiele
{
    struct ParserToken
    {
        explicit ParserToken(const std::string& content = std::string(), ParserTokens type = ParserTokens_Unknown);

        std::string Content;
        ParserTokens Type;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_PARSERTOKEN_H. */
