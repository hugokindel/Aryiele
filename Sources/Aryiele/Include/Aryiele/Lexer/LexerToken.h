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

#ifndef ARYIELE_LEXER_LEXERTOKEN_H
#define ARYIELE_LEXER_LEXERTOKEN_H

#include <string>

namespace Aryiele {
    enum LexerTokenEnum {
        LexerToken_Reject,
        LexerToken_Number,
        LexerToken_String,
        LexerToken_Operator,
        LexerToken_Separator,
        LexerToken_Identifier,
        // Not on final pass.
        LexerToken_StringQuote,
        LexerToken_Space,
        LexerToken_Newline,
        LexerToken_Unknown
    };
    
    struct LexerToken {
        explicit LexerToken(const std::string& content = std::string(), LexerTokenEnum type = LexerToken_Unknown);

        std::string content;
        LexerTokenEnum type;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_LEXER_LEXERTOKEN_H. */
