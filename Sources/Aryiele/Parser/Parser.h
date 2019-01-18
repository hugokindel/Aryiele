#ifndef ARYIELE_PARSER_H
#define ARYIELE_PARSER_H

#include <vector>
#include <Aryiele/Core/Includes.h>
#include <Aryiele/Parser/ParserToken.h>
#include <Aryiele/Tokenizer/TokenizerToken.h>

namespace Aryiele
{
    class Parser
    {
    public:
        std::vector<ParserToken> ConvertTokens(std::vector<TokenizerToken> tokenizerTokens);
        std::string GetTokenName(ParserToken tokenType);
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_PARSER_H. */
