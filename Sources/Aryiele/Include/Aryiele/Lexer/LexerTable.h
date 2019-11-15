//==================================================================================//
//                                                                                  //
//  Copyright (c) 2019 Hugo Kindel <kindelhugo.pro@gmail.com>                       //
//                                                                                  //
//  This file is part of the Aryiele project.                                       //
//  Licensed under MIT License:                                                     //
//                                                                                  //
//  Permission is hereby granted, free of charge, to any person obtaining           //
//  a copy of this software and associated documentation files (the "Software"),    //
//  to deal in the Software without restriction, including without limitation       //
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,        //
//  and/or sell copies of the Software, and to permit persons to whom the           //
//  Software is furnished to do so, subject to the following conditions:            //
//                                                                                  //
//  The above copyright notice and this permission notice shall be included in      //
//  all copies or substantial portions of the Software.                             //
//                                                                                  //
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      //
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        //
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     //
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          //
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   //
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   //
//  SOFTWARE.                                                                       //
//                                                                                  //
//==================================================================================//

#ifndef ARYIELE_LEXER_TOKENTABLE_H
#define ARYIELE_LEXER_TOKENTABLE_H

#include <Aryiele/Lexer/LexerToken.h>

namespace Aryiele
{
    int LexerTable[][12] =
        {{ 0,                      LexerToken_Number,     LexerToken_String, LexerToken_Character, LexerToken_Operator, LexerToken_Separator, LexerToken_Identifier, LexerToken_StringQuote, LexerToken_Space,  LexerToken_Newline, LexerToken_Unknown },
         { LexerToken_Number,      LexerToken_Number,     LexerToken_Reject, LexerToken_Reject,    LexerToken_Reject,   LexerToken_Reject,    LexerToken_Number, LexerToken_Reject,      LexerToken_Reject, LexerToken_Reject,  LexerToken_Reject  },
         { LexerToken_String,      LexerToken_Reject,     LexerToken_Reject, LexerToken_Reject,    LexerToken_Reject,   LexerToken_Reject,    LexerToken_Reject,     LexerToken_Reject,      LexerToken_Reject, LexerToken_Reject,  LexerToken_Reject  },
         { LexerToken_Character,   LexerToken_Reject,     LexerToken_Reject, LexerToken_Reject,    LexerToken_Reject,   LexerToken_Reject,    LexerToken_Reject,     LexerToken_Reject,      LexerToken_Reject, LexerToken_Reject,  LexerToken_Reject },
         { LexerToken_Operator,    LexerToken_Reject,     LexerToken_Reject, LexerToken_Reject,    LexerToken_Operator, LexerToken_Reject,    LexerToken_Reject,     LexerToken_Reject,      LexerToken_Reject, LexerToken_Reject,  LexerToken_Reject  },
         { LexerToken_Separator,   LexerToken_Reject,     LexerToken_Reject, LexerToken_Reject,    LexerToken_Reject,   LexerToken_Reject,    LexerToken_Reject,     LexerToken_Reject,      LexerToken_Reject, LexerToken_Reject,  LexerToken_Reject  },
         { LexerToken_Identifier,  LexerToken_Identifier, LexerToken_Reject, LexerToken_Reject,    LexerToken_Reject,   LexerToken_Reject,    LexerToken_Identifier, LexerToken_Reject,      LexerToken_Reject, LexerToken_Reject,  LexerToken_Reject  },
         { LexerToken_StringQuote, LexerToken_Reject,     LexerToken_Reject, LexerToken_Reject,    LexerToken_Reject,   LexerToken_Reject,    LexerToken_Reject,     LexerToken_Reject,      LexerToken_Reject, LexerToken_Reject,  LexerToken_Reject  },
         { LexerToken_Space,       LexerToken_Reject,     LexerToken_Reject, LexerToken_Reject,    LexerToken_Reject,   LexerToken_Reject,    LexerToken_Reject,     LexerToken_Reject,      LexerToken_Reject,  LexerToken_Reject,  LexerToken_Reject  },
         { LexerToken_Newline,     LexerToken_Reject,     LexerToken_Reject, LexerToken_Reject,    LexerToken_Reject,   LexerToken_Reject,    LexerToken_Reject,     LexerToken_Reject,      LexerToken_Reject, LexerToken_Reject,  LexerToken_Reject  },
         { LexerToken_Unknown,     LexerToken_Reject,     LexerToken_Reject, LexerToken_Reject,    LexerToken_Reject,   LexerToken_Reject,    LexerToken_Reject,     LexerToken_Reject,      LexerToken_Reject, LexerToken_Reject,  LexerToken_Reject  }
        };
    
} /* Namespace Aryiele. */

#endif /* ARYIELE_LEXER_TOKENTABLE_H. */
