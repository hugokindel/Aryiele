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

#ifndef ARYIELE_PARSER_PARSERTOKEN_H
#define ARYIELE_PARSER_PARSERTOKEN_H

#include <Aryiele/Common.h>

namespace Aryiele {
    enum ParserTokenEnum {
        ParserToken_LiteralValueInteger, // A number
        ParserToken_LiteralValueDecimal, // A decimal number
        ParserToken_LiteralValueString, // A string
        ParserToken_LiteralValueCharacter, // A character
        ParserToken_LiteralValueBoolean, // A boolean
        ParserToken_OperatorEqual, // =
        ParserToken_OperatorArithmeticPlus, // +
        ParserToken_OperatorArithmeticMinus, // -
        ParserToken_OperatorArithmeticMultiply, // *
        ParserToken_OperatorArithmeticDivide, // /
        ParserToken_OperatorArithmeticRemainder, // %
        ParserToken_OperatorArithmeticPlusEqual, // +=
        ParserToken_OperatorArithmeticMinusEqual, // -=
        ParserToken_OperatorArithmeticMultiplyEqual, // *=
        ParserToken_OperatorArithmeticDivideEqual, // /=
        ParserToken_OperatorArithmeticRemainderEqual, // %=
        ParserToken_OperatorUnaryArithmeticPlus, // +
        ParserToken_OperatorUnaryArithmeticMinus, // -
        ParserToken_OperatorUnaryArithmeticIncrement, // ++
        ParserToken_OperatorUnaryArithmeticDecrement, // --
        ParserToken_OperatorComparisonEqual, // ==
        ParserToken_OperatorComparisonNotEqual, // !=
        ParserToken_OperatorComparisonLessThan, // <
        ParserToken_OperatorComparisonGreaterThan, // >
        ParserToken_OperatorComparisonLessThanOrEqual, // <=
        ParserToken_OperatorComparisonGreaterThanOrEqual, // >=
        ParserToken_OperatorLogicalAnd, // &&
        ParserToken_OperatorLogicalOr, // ||
        //ParserToken_OperatorBitwiseAnd, // &
        //ParserToken_OperatorBitwiseExclusiveOr, // ^
        //ParserToken_OperatorBitwiseInclusiveOr, // |
        ParserToken_OperatorUnaryLogicalNot, // !
        ParserToken_OperatorQuestionMark, // ?
        ParserToken_SeparatorRoundBracketOpen, // (
        ParserToken_SeparatorRoundBracketClosed, // )
        ParserToken_SeparatorSquareBracketOpen, // [
        ParserToken_SeparatorSquareBracketClosed, // ]
        ParserToken_SeparatorCurlyBracketOpen, // {
        ParserToken_SeparatorCurlyBracketClosed, // }
        ParserToken_SeparatorColon, // :
        ParserToken_SeparatorSemicolon, // ;
        ParserToken_SeparatorComma, // ,
        ParserToken_SeparatorDot, // .
        ParserToken_SeparatorTripleDot, // ...
        ParserToken_KeywordFunction, // func
        ParserToken_KeywordNamespace, // namespace
        ParserToken_KeywordVar, // var
        ParserToken_KeywordLet, // let
        ParserToken_KeywordReturn, // return
        ParserToken_KeywordIf, // if
        ParserToken_KeywordElse, // else
        ParserToken_KeywordFor, // for
        ParserToken_KeywordDo, // do
        ParserToken_KeywordBy, // by
        ParserToken_KeywordContinue, // continue
        ParserToken_KeywordBreak, // break
        ParserToken_KeywordWhile, // while
        ParserToken_KeywordDefault, // default
        ParserToken_KeywordSwitch, // switch
        ParserToken_KeywordCase, // case
        ParserToken_Identifier, // Any characters sequence other than a keyword.
        ParserToken_EOF, // EOF
        ParserToken_Space, // A space
        ParserToken_Newline, // A newline
        ParserToken_Unknown
    };
    
    struct ParserToken {
        explicit ParserToken(const std::string& content = std::string(), ParserTokenEnum type = ParserToken_Unknown);

        std::string content;
        ParserTokenEnum type;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_PARSER_PARSERTOKEN_H. */
