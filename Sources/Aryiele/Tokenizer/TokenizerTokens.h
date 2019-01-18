#ifndef ARYIELE_LEXERTOKENS_H
#define ARYIELE_LEXERTOKENS_H

namespace Aryiele
{
    enum TokenizerTokens
    {
        TokenizerTokens_Reject,

        TokenizerTokens_Number,
        TokenizerTokens_String,

        TokenizerTokens_Operator,
        TokenizerTokens_Scope,
        TokenizerTokens_Identifier,

        TokenizerTokens_EOL,

        // Not on final pass.
        TokenizerTokens_StringQuote,
        TokenizerTokens_Space,
        TokenizerTokens_Newline,

        // Not in final pass (except if the code contains errors).
        TokenizerTokens_Unknown
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_LEXERTOKENS_H. */
