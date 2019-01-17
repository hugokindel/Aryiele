#ifndef ARYIELE_TOKENTABLE_H
#define ARYIELE_TOKENTABLE_H

#include <Aryiele/Tokenizer/TokenTypes.h>

namespace Aryiele
{
    int TokenTable[][15] =
    {{0,           Integer, Decimal, String,  Boolean, Operator, Scope,   Identifier, Keyword,  Comment, Newline, EndOfLine, StringQuote, Unknown, Space   },
    { Integer,     Integer, Decimal, Reject,  Reject,  Reject,   Reject,  Reject,     Reject,   Reject,  Reject,  Reject,    Reject,      Reject,  Reject  },
    { Decimal,     Decimal, Decimal, Reject,  Reject,  Reject,   Reject,  Reject,     Reject,   Reject,  Reject,  Reject,    Reject,      Reject,  Reject  },
    { String,      Reject,  Reject,  Reject,  Reject,  Reject,   Reject,  Reject,     Reject,   Reject,  Reject,  Reject,    Reject,      Reject,  Reject  },
    { Boolean,     Reject,  Reject,  Reject,  Reject,  Reject,   Reject,  Reject,     Reject,   Reject,  Reject,  Reject,    Reject,      Reject,  Reject  },
    { Operator,    Reject,  Reject,  Reject,  Reject,  Operator, Reject,  Reject,     Reject,   Reject,  Reject,  Reject,    Reject,      Reject,  Reject  },
    { Scope,       Reject,  Reject,  Reject,  Reject,  Reject,   Reject,  Reject,     Reject,   Reject,  Reject,  Reject,    Reject,      Reject,  Reject  },
    { Identifier,  Reject,  Reject,  Reject,  Reject,  Reject,   Reject,  Identifier, Reject,   Reject,  Reject,  Reject,    Reject,      Reject,  Reject  },
    { Keyword,     Reject,  Reject,  Reject,  Reject,  Reject,   Reject,  Identifier, Reject,   Reject,  Reject,  Reject,    Reject,      Reject,  Reject  },
    { Comment,     Reject,  Reject,  Reject,  Reject,  Reject,   Reject,  Identifier, Reject,   Reject,  Reject,  Reject,    Reject,      Reject,  Reject  },
    { Newline,     Reject,  Reject,  Reject,  Reject,  Reject,   Reject,  Identifier, Reject,   Reject,  Reject,  Reject,    Reject,      Reject,  Reject  },
    { EndOfLine,   Reject,  Reject,  Reject,  Reject,  Reject,   Reject,  Identifier, Reject,   Reject,  Reject,  Reject,    Reject,      Reject,  Reject  },
    { StringQuote, Reject,  Reject,  Reject,  Reject,  Reject,   Reject,  Reject,     Reject,   Reject,  Reject,  Reject,    Reject,      Reject,  Reject  },
    { Unknown,     Unknown, Unknown, Unknown, Unknown, Unknown,  Unknown, Unknown,    Unknown,  Unknown, Unknown, Unknown,   Unknown,     Unknown, Unknown },
    { Space,       Reject,  Reject,  Reject,  Reject,  Reject,   Reject,  Reject,     Reject,   Reject,  Reject,  Reject,    Reject,      Reject,  Reject }};

} /* Namespace Aryiele. */

#endif /* ARYIELE_TOKENTABLE_H. */