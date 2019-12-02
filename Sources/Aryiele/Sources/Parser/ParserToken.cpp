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

#include <Aryiele/Parser/ParserToken.h>

namespace Aryiele {
    ParserToken::ParserToken(const std::string &content, Aryiele::ParserTokenEnum type) :
        content(content), type(type) {

    }
    
    std::string ParserToken::getTypeName(ParserTokenEnum type) {
        switch (type) {
            case ParserToken_LiteralValueInteger:
                return "LiteralValueInteger";
            case ParserToken_LiteralValueDecimal:
                return "LiteralValueDecimal";
            case ParserToken_LiteralValueBoolean:
                return "LiteralValueBoolean";
            case ParserToken_LiteralValueString:
                return "LiteralValueString";
            case ParserToken_LiteralValueCharacter:
                return "LiteralValueCharacter";
                
            case ParserToken_OperatorEqual:
                return "OperatorEqual";
            case ParserToken_OperatorArithmeticPlus:
                return "OperatorArithmeticPlus";
            case ParserToken_OperatorArithmeticMinus:
                return "OperatorArithmeticMinus";
            case ParserToken_OperatorUnaryArithmeticPlus:
                return "OperatorUnaryArithmeticPlus";
            case ParserToken_OperatorUnaryArithmeticMinus:
                return "OperatorUnaryArithmeticMinus";
            case ParserToken_OperatorArithmeticMultiply:
                return "OperatorArithmeticMultiply";
            case ParserToken_OperatorArithmeticDivide:
                return "OperatorArithmeticDivide";
            case ParserToken_OperatorArithmeticRemainder:
                return "OperatorArithmeticRemainder";
            case ParserToken_OperatorUnaryArithmeticIncrement:
                return "OperatorUnaryArithmeticIncrement";
            case ParserToken_OperatorUnaryArithmeticDecrement:
                return "OperatorUnaryArithmeticDecrement";
            case ParserToken_OperatorComparisonEqual:
                return "OperatorComparisonEqual";
            case ParserToken_OperatorComparisonNotEqual:
                return "OperatorComparisonNotEqual";
            case ParserToken_OperatorComparisonLessThan:
                return "OperatorComparisonLessThan";
            case ParserToken_OperatorComparisonGreaterThan:
                return "OperatorComparisonGreaterThan";
            case ParserToken_OperatorComparisonLessThanOrEqual:
                return "OperatorComparisonLessThanOrEqual";
            case ParserToken_OperatorComparisonGreaterThanOrEqual:
                return "OperatorComparisonGreaterThanOrEqual";
            case ParserToken_OperatorLogicalAnd:
                return "OperatorLogicalAnd";
            case ParserToken_OperatorLogicalOr:
                return "OperatorLogicalOr";
            case ParserToken_OperatorUnaryLogicalNot:
                return "OperatorUnaryLogicalNot";
            case ParserToken_OperatorQuestionMark:
                return "OperatorQuestionMark";
            case ParserToken_OperatorArithmeticPlusEqual:
                return "OperatorArithmeticPlusEqual";
            case ParserToken_OperatorArithmeticMinusEqual:
                return "OperatorArithmeticMinusqual";
            case ParserToken_OperatorArithmeticMultiplyEqual:
                return "OperatorArithmeticMultiplyEqual";
            case ParserToken_OperatorArithmeticDivideEqual:
                return "OperatorArithmeticDivideEqual";
            case ParserToken_OperatorArithmeticRemainderEqual:
                return "OperatorArithmeticRemainderEqual";
                
            case ParserToken_SeparatorRoundBracketOpen:
                return "SeparatorRoundBracketOpen";
            case ParserToken_SeparatorRoundBracketClosed:
                return "SeparatorRoundBracketClosed";
            case ParserToken_SeparatorSquareBracketOpen:
                return "SeparatorSquareBracketOpen";
            case ParserToken_SeparatorSquareBracketClosed:
                return "SeparatorSquareBracketClosed";
            case ParserToken_SeparatorCurlyBracketOpen:
                return "SeparatorCurlyBracketOpen";
            case ParserToken_SeparatorCurlyBracketClosed:
                return "SeparatorCurlyBracketClosed";
            case ParserToken_SeparatorColon:
                return "SeparatorColon";
            case ParserToken_SeparatorComma:
                return "SeparatorComma";
            case ParserToken_SeparatorDot:
                return "SeparatorDot";
            case ParserToken_SeparatorTripleDot:
                return "SeparatorTripleDot";
            case ParserToken_SeparatorSemicolon:
                return "SeparatorSemicolon";
                
            case ParserToken_KeywordFunction:
                return "KeywordFunction";
            case ParserToken_KeywordVar:
                return "KeywordVar";
            case ParserToken_KeywordReturn:
                return "KeywordReturn";
            case ParserToken_KeywordIf:
                return "KeywordIf";
            case ParserToken_KeywordElse:
                return "KeywordElse";
            case ParserToken_KeywordBreak:
                return "KeywordBreak";
            case ParserToken_KeywordContinue:
                return "KeywordContinue";
            case ParserToken_Identifier:
                return "Identifier";
            case ParserToken_KeywordLet:
                return "KeywordLet";
            case ParserToken_KeywordFor:
                return "KeywordFor";
            case ParserToken_KeywordBy:
                return "KeywordBy";
            case ParserToken_KeywordDo:
                return "KeywordDo";
            case ParserToken_KeywordWhile:
                return "KeywordWhile";
            case ParserToken_KeywordSwitch:
                return "KeywordSwitch";
            case ParserToken_KeywordCase:
                return "KeywordCase";
            case ParserToken_KeywordNamespace:
                return "KeywordNamespace";
            case ParserToken_KeywordDefault:
                return "KeywordDefault";
                
            case ParserToken_Space:
                return "Space";
            case ParserToken_Newline:
                return "Newline";
            case ParserToken_EOF:
                return "EOF";
                
            case ParserToken_Unknown:
                return "Unknown";
                
            default:
                return "Error";
        }
    }
    
} /* Namespace Aryiele. */
