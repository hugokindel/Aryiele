#ifndef ARYIELE_TOKENTABLE_H
#define ARYIELE_TOKENTABLE_H

#include <Aryiele/Lexer/LexerTokens.h>

namespace Aryiele
{
    int LexerTable[][11] =
    {{   0,                       LexerTokens_Number,  LexerTokens_String,  LexerTokens_Operator, LexerTokens_Separator, LexerTokens_Identifier, LexerTokens_StringQuote, LexerTokens_Space,  LexerTokens_Newline, LexerTokens_Unknown },
    {    LexerTokens_Number,      LexerTokens_Number,  LexerTokens_Reject,  LexerTokens_Reject,   LexerTokens_Reject,    LexerTokens_Reject,     LexerTokens_Reject,      LexerTokens_Reject, LexerTokens_Reject,  LexerTokens_Reject  },
    {    LexerTokens_String,      LexerTokens_Reject,  LexerTokens_Reject,  LexerTokens_Reject,   LexerTokens_Reject,    LexerTokens_Reject,     LexerTokens_Reject,      LexerTokens_Reject, LexerTokens_Reject,  LexerTokens_Reject  },
    {    LexerTokens_Operator,    LexerTokens_Reject,  LexerTokens_Reject,  LexerTokens_Operator, LexerTokens_Reject,    LexerTokens_Reject,     LexerTokens_Reject,      LexerTokens_Reject, LexerTokens_Reject,  LexerTokens_Reject  },
    {    LexerTokens_Separator,   LexerTokens_Reject,  LexerTokens_Reject,  LexerTokens_Reject,   LexerTokens_Reject,    LexerTokens_Reject,     LexerTokens_Reject,      LexerTokens_Reject, LexerTokens_Reject,  LexerTokens_Reject  },
    {    LexerTokens_Identifier,  LexerTokens_Reject,  LexerTokens_Reject,  LexerTokens_Reject,   LexerTokens_Reject,    LexerTokens_Identifier, LexerTokens_Reject,      LexerTokens_Reject, LexerTokens_Reject,  LexerTokens_Reject  },
    {    LexerTokens_StringQuote, LexerTokens_Reject,  LexerTokens_Reject,  LexerTokens_Reject,   LexerTokens_Reject,    LexerTokens_Reject,     LexerTokens_Reject,      LexerTokens_Reject, LexerTokens_Reject,  LexerTokens_Reject  },
    {    LexerTokens_Space,       LexerTokens_Reject,  LexerTokens_Reject,  LexerTokens_Reject,   LexerTokens_Reject,    LexerTokens_Reject,     LexerTokens_Reject,      LexerTokens_Space,  LexerTokens_Reject,  LexerTokens_Reject  },
    {    LexerTokens_Newline,     LexerTokens_Reject,  LexerTokens_Reject,  LexerTokens_Reject,   LexerTokens_Reject,    LexerTokens_Reject,     LexerTokens_Reject,      LexerTokens_Reject, LexerTokens_Reject,  LexerTokens_Reject  },
    {    LexerTokens_Unknown,     LexerTokens_Reject,  LexerTokens_Reject,  LexerTokens_Reject,   LexerTokens_Reject,    LexerTokens_Reject,     LexerTokens_Reject,      LexerTokens_Reject, LexerTokens_Reject,  LexerTokens_Reject  }};

} /* Namespace Aryiele. */

#endif /* ARYIELE_TOKENTABLE_H. */
