#ifndef ARYIELE_TOKENTABLE_H
#define ARYIELE_TOKENTABLE_H

#include <Aryiele/Tokenizer/TokenizerTokens.h>

namespace Aryiele
{
    int TokenizerTable[][11] =
    {{   0,                           TokenizerTokens_Number,  TokenizerTokens_String,  TokenizerTokens_Operator, TokenizerTokens_Scope,   TokenizerTokens_Identifier, TokenizerTokens_EOF,     TokenizerTokens_StringQuote, TokenizerTokens_Space,  TokenizerTokens_Newline, TokenizerTokens_Unknown },
    {    TokenizerTokens_Number,      TokenizerTokens_Number,  TokenizerTokens_Reject,  TokenizerTokens_Reject,   TokenizerTokens_Reject,  TokenizerTokens_Reject,     TokenizerTokens_Reject,  TokenizerTokens_Reject,      TokenizerTokens_Reject, TokenizerTokens_Reject,  TokenizerTokens_Reject  },
    {    TokenizerTokens_String,      TokenizerTokens_Reject,  TokenizerTokens_Reject,  TokenizerTokens_Reject,   TokenizerTokens_Reject,  TokenizerTokens_Reject,     TokenizerTokens_Reject,  TokenizerTokens_Reject,      TokenizerTokens_Reject, TokenizerTokens_Reject,  TokenizerTokens_Reject  },
    {    TokenizerTokens_Operator,    TokenizerTokens_Reject,  TokenizerTokens_Reject,  TokenizerTokens_Operator, TokenizerTokens_Reject,  TokenizerTokens_Reject,     TokenizerTokens_Reject,  TokenizerTokens_Reject,      TokenizerTokens_Reject, TokenizerTokens_Reject,  TokenizerTokens_Reject  },
    {    TokenizerTokens_Scope,       TokenizerTokens_Reject,  TokenizerTokens_Reject,  TokenizerTokens_Reject,   TokenizerTokens_Reject,  TokenizerTokens_Reject,     TokenizerTokens_Reject,  TokenizerTokens_Reject,      TokenizerTokens_Reject, TokenizerTokens_Reject,  TokenizerTokens_Reject  },
    {    TokenizerTokens_Identifier,  TokenizerTokens_Reject,  TokenizerTokens_Reject,  TokenizerTokens_Reject,   TokenizerTokens_Reject,  TokenizerTokens_Identifier, TokenizerTokens_Reject,  TokenizerTokens_Reject,      TokenizerTokens_Reject, TokenizerTokens_Reject,  TokenizerTokens_Reject  },
    {    TokenizerTokens_EOF,         TokenizerTokens_Reject,  TokenizerTokens_Reject,  TokenizerTokens_Reject,   TokenizerTokens_Reject,  TokenizerTokens_Reject,     TokenizerTokens_Reject,  TokenizerTokens_Reject,      TokenizerTokens_Reject, TokenizerTokens_Reject,  TokenizerTokens_Reject  },
    {    TokenizerTokens_StringQuote, TokenizerTokens_Reject,  TokenizerTokens_Reject,  TokenizerTokens_Reject,   TokenizerTokens_Reject,  TokenizerTokens_Reject,     TokenizerTokens_Reject,  TokenizerTokens_Reject,      TokenizerTokens_Reject, TokenizerTokens_Reject,  TokenizerTokens_Reject  },
    {    TokenizerTokens_Space,       TokenizerTokens_Reject,  TokenizerTokens_Reject,  TokenizerTokens_Reject,   TokenizerTokens_Reject,  TokenizerTokens_Reject,     TokenizerTokens_Reject,  TokenizerTokens_Reject,      TokenizerTokens_Space,  TokenizerTokens_Reject,  TokenizerTokens_Reject  },
    {    TokenizerTokens_Newline,     TokenizerTokens_Reject,  TokenizerTokens_Reject,  TokenizerTokens_Reject,   TokenizerTokens_Reject,  TokenizerTokens_Reject,     TokenizerTokens_Reject,  TokenizerTokens_Reject,      TokenizerTokens_Reject, TokenizerTokens_Reject,  TokenizerTokens_Reject  },
    {    TokenizerTokens_Unknown,     TokenizerTokens_Reject,  TokenizerTokens_Reject,  TokenizerTokens_Reject,   TokenizerTokens_Reject,  TokenizerTokens_Reject,     TokenizerTokens_Reject,  TokenizerTokens_Reject,      TokenizerTokens_Reject, TokenizerTokens_Reject,  TokenizerTokens_Reject  }};

} /* Namespace Aryiele. */

#endif /* ARYIELE_TOKENTABLE_H. */
