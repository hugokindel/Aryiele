#ifndef ARYIELE_TOKENTYPES_H
#define ARYIELE_TOKENTYPES_H

namespace Aryiele
{
    enum TokenTypes
    {
        Reject = 0,
        Integer,
        Decimal,
        String,
        Boolean,
        Operator,
        Scope,
        Identifier,
        Keyword,
        Comment, // Not on final pass
        Newline, // Not on final pass
        EndOfLine,
        StringQuote, // Not on final pass
        Unknown, // Should not be in final pass except if error occurs
        Space // Not on final pass
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_TOKENTYPES_H. */