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
        ParserToken_LiteralValueInteger, // A number, example: 9
        ParserToken_LiteralValueDecimal, // A decimal number, example: 5.5
        ParserToken_LiteralValueString, // A string, example: "This is a text"
        ParserToken_LiteralValueBoolean, // A boolean, example: true
        ParserToken_OperatorEqual, // =, example: 5 = 5
        ParserToken_OperatorArithmeticPlus, // +, example: 5 + 5
        ParserToken_OperatorArithmeticUnaryPlus, // + (as left expression), example: +7
        ParserToken_OperatorArithmeticMinus, // -, example: 5 - 8
        ParserToken_OperatorArithmeticUnaryMinus, // - (as left expression), example: -2
        ParserToken_OperatorArithmeticMultiply, // *, example: 5 * 7
        ParserToken_OperatorArithmeticDivide, // /, example: 7 / 4
        ParserToken_OperatorArithmeticRemainder, // %, example: 5 % 7
        ParserToken_OperatorComparisonEqual, // ==, example: x == y
        ParserToken_OperatorComparisonNotEqual, // !=, example: x != y
        ParserToken_OperatorComparisonLessThan, // <, example: x < y
        ParserToken_OperatorComparisonGreaterThan, // >, example: x > y
        ParserToken_OperatorComparisonLessThanOrEqual, // <=, example: x <= y
        ParserToken_OperatorComparisonGreaterThanOrEqual, // >=, example: x >= y
        ParserToken_OperatorLogicalAnd, // &&, example: x && y
        ParserToken_OperatorLogicalOr, // ||, example: x || y
        ParserToken_OperatorLogicalNot, // ! (as left expression), example: !x
        //ParserToken_OperatorLogicalBitwiseAnd, // &, example: x & y
        //ParserToken_OperatorLogicalBitwiseOr, // |, example: x | y
        //ParserToken_OperatorLogicalBitwiseExcluseOr, // ^, example: x ^ y
        //ParserToken_OperatorLogicalBitwiseComplement, // ~ (as left expression), example: ~x
        ParserToken_SeparatorRoundBracketOpen, // (
        ParserToken_SeparatorRoundBracketClosed, // )
        ParserToken_SeparatorSquareBracketOpen, // [
        ParserToken_SeparatorSquareBracketClosed, // ]
        ParserToken_SeparatorCurlyBracketOpen, // {
        ParserToken_SeparatorCurlyBracketClosed, // }
        ParserToken_SeparatorColon, // :, example: var test: int = 0;
        ParserToken_SeparatorSemicolon, // ;, example: return 0;
        ParserToken_SeparatorComma, // ,
        //ParserToken_SeparatorAngleBracketOpen, // <
        //ParserToken_SeparatorAngleBracketClosed, // >
        ParserToken_Keyword_TopLevel_Function, // function
        ParserToken_KeywordVar, // var
        ParserToken_KeywordReturn, // return
        ParserToken_KeywordIf, // if
        ParserToken_KeywordElse, // else
        //ParserToken_KeywordLet, // let
        //ParserToken_KeywordImport, // import
        //ParserToken_KeywordClass, // class
        //ParserToken_KeywordThis, // this
        //ParserToken_KeywordStruct, // struct
        //ParserToken_KeywordEnum, // enum
        //ParserToken_KeywordNamespace, // namespace
        //ParserToken_KeywordStatic, // statoc
        //ParserToken_KeywordTypeInt, // int
        //ParserToken_KeywordTypeDouble, // float
        //ParserToken_KeywordTypeBool, // bool
        //ParserToken_KeywordTypeFloat, // double
        //ParserToken_KeywordTypeChar, // char
        //ParserToken_KeywordTypeString, // string
        //ParserToken_KeywordTypeByte, // byte
        //ParserToken_KeywordTypeLong, // long
        //ParserToken_KeywordTypeUInt, // uint
        //ParserToken_KeywordTypeULong, // ulong
        //ParserToken_KeywordTypeUByte, // ubyte
        //ParserToken_KeywordTypeList, // list, can change later
        //ParserToken_KeywordTypeArray, // array, can change later
        //ParserToken_PreprocessorStart, // #, #TEST "Aryiele"
        //ParserToken_PreprocessorNewline, // \, #TEST "Aryiele" \ "test"
        ParserToken_Identifier, // Any characters sequence other than a keyword.
        ParserToken_EOF, // EOF
        ParserToken_Unknown
    };
    
    struct ParserToken {
        explicit ParserToken(const std::string& content = std::string(), ParserTokenEnum type = ParserToken_Unknown);

        std::string content;
        ParserTokenEnum type;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_PARSER_PARSERTOKEN_H. */
