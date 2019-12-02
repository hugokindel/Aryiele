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

#ifndef ARYIELE_LEXER_LEXER_H
#define ARYIELE_LEXER_LEXER_H

#include <vector>
#include <Vanir/Module/Module.h>
#include <Aryiele/Common.h>
#include <Aryiele/Lexer/LexerToken.h>

namespace Aryiele {
class Lexer : public Vanir::Module<Lexer> {
    public:
        std::vector<LexerToken> lex(const std::string& filepath);
        
    private:
        // First pass of the Lexer (separate all characters by expression with a finite-state machine).
        void stateMachine(std::string expression);
        // Second pass of the Lexer (remove all comments as their ain't useful).
        void stateComments();
        // Third pass of the Lexer (define literal strings).
        void stateStrings();
        // Fourth pass of the Lexer (define literal integers and floating-point numbers).
        void stateNumbers();
        // Fifth pass of the Lexer (make spaces more consistent).
        void stateSpaces();
        // Sixth pass of the Lexer (identify some operators).
        void stateOperators();
        // Seventh pass of the Lexer (identifier).
        void stateIdentifiers();
        static LexerTokenEnum getTransitionTableColumn(char currentCharacter);

        std::vector<LexerToken> m_tokens;
    };

    Lexer &getLexer();

} /* Namespace Aryiele. */

#endif /* ARYIELE_LEXER_LEXER_H. */
