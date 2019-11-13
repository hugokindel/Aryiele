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

#include <Aryiele/Lexer/LexerTokens.h>

namespace Aryiele {
    int LexerTable[][11] = {{   0,                       LexerTokens_Number,     LexerTokens_String,  LexerTokens_Operator, LexerTokens_Separator, LexerTokens_Identifier, LexerTokens_StringQuote, LexerTokens_Space,  LexerTokens_Newline, LexerTokens_Unknown }, {    LexerTokens_Number,      LexerTokens_Number,     LexerTokens_Reject,  LexerTokens_Reject,   LexerTokens_Reject,    LexerTokens_Identifier, LexerTokens_Reject,      LexerTokens_Reject, LexerTokens_Reject,  LexerTokens_Reject  }, {    LexerTokens_String,      LexerTokens_Reject,     LexerTokens_Reject,  LexerTokens_Reject,   LexerTokens_Reject,    LexerTokens_Reject,     LexerTokens_Reject,      LexerTokens_Reject, LexerTokens_Reject,  LexerTokens_Reject  }, {    LexerTokens_Operator,    LexerTokens_Reject,     LexerTokens_Reject,  LexerTokens_Operator, LexerTokens_Reject,    LexerTokens_Reject,     LexerTokens_Reject,      LexerTokens_Reject, LexerTokens_Reject,  LexerTokens_Reject  }, {    LexerTokens_Separator,   LexerTokens_Reject,     LexerTokens_Reject,  LexerTokens_Reject,   LexerTokens_Reject,    LexerTokens_Reject,     LexerTokens_Reject,      LexerTokens_Reject, LexerTokens_Reject,  LexerTokens_Reject  }, {    LexerTokens_Identifier,  LexerTokens_Identifier, LexerTokens_Reject,  LexerTokens_Reject,   LexerTokens_Reject,    LexerTokens_Identifier, LexerTokens_Reject,      LexerTokens_Reject, LexerTokens_Reject,  LexerTokens_Reject  }, {    LexerTokens_StringQuote, LexerTokens_Reject,     LexerTokens_Reject,  LexerTokens_Reject,   LexerTokens_Reject,    LexerTokens_Reject,     LexerTokens_Reject,      LexerTokens_Reject, LexerTokens_Reject,  LexerTokens_Reject  }, {    LexerTokens_Space,       LexerTokens_Reject,     LexerTokens_Reject,  LexerTokens_Reject,   LexerTokens_Reject,    LexerTokens_Reject,     LexerTokens_Reject,      LexerTokens_Space,  LexerTokens_Reject,  LexerTokens_Reject  }, {    LexerTokens_Newline,     LexerTokens_Reject,     LexerTokens_Reject,  LexerTokens_Reject,   LexerTokens_Reject,    LexerTokens_Reject,     LexerTokens_Reject,      LexerTokens_Reject, LexerTokens_Reject,  LexerTokens_Reject  }, {    LexerTokens_Unknown,     LexerTokens_Reject,     LexerTokens_Reject,  LexerTokens_Reject,   LexerTokens_Reject,    LexerTokens_Reject,     LexerTokens_Reject,      LexerTokens_Reject, LexerTokens_Reject,  LexerTokens_Reject  }};

} /* Namespace Aryiele. */

#endif /* ARYIELE_LEXER_TOKENTABLE_H. */
