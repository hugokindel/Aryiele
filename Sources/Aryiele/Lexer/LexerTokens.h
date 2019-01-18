#ifndef ARYIELE_LEXERTOKENS_H
#define ARYIELE_LEXERTOKENS_H

namespace Aryiele
{
    enum LexerTokens
    {
        LexerTokens_Reject,

        LexerTokens_Number,
        LexerTokens_String,

        LexerTokens_Operator,
        LexerTokens_Scope,
        LexerTokens_Identifier,

        LexerTokens_EOL,

        // Not on final pass.
        LexerTokens_StringQuote,
        LexerTokens_Space,
        LexerTokens_Newline,

        // Not in final pass (except if the code contains errors).
        LexerTokens_Unknown
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_LEXERTOKENS_H. */
