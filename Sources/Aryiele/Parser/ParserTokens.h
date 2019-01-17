#ifndef ARYIELE_PARSERTOKEN_H
#define ARYIELE_PARSERTOKEN_H

namespace Aryiele
{
    // TODO - V2: Implement all token types.
    enum ParserTokens
    {
        ParserToken_LiteralValue_Integer, // A number, example: 9
        ParserToken_LiteralValue_Decimal, // A decimal number, example: 5.5
        ParserToken_LiteralValue_String, // A string, example: "This is a text"
        ParserToken_LiteralValue_Boolean, // A boolean, example: true

        ParserToken_Operator_Arithmetic_Plus, // +, example: 5 + 5
        ParserToken_Operator_Arithmetic_UnaryPlus, // + (as left expression), example: +7
        ParserToken_Operator_Arithmetic_Minus, // -, example: 5 - 8
        ParserToken_Operator_Arithmetic_UnaryMinus, // - (as left expression), example: -2
        ParserToken_Operator_Arithmetic_Multiply, // *, example: 5 * 7
        ParserToken_Operator_Arithmetic_Divide, // /, example: 7 / 4
        ParserToken_Operator_Arithmetic_Remainder, // %, example: 5 % 7

        ParserToken_Operator_Comparison_Equal, // ==, example: x == y
        ParserToken_Operator_Comparison_NotEqual, // !=, example: x != y
        ParserToken_Operator_Comparison_LessThan, // <, example: x < y
        ParserToken_Operator_Comparison_GreaterThan, // >, example: x > y
        ParserToken_Operator_Comparison_LessThanOrEqual, // <=, example: x <= y
        ParserToken_Operator_Comparison_GreaterThanOrEqual, // >=, example: x >= y

        ParserToken_Operator_Logical_And, // &&, example: x && y
        ParserToken_Operator_Logical_Or, // ||, example: x || y
        ParserToken_Operator_Logical_Not, // ! (as left expression), example: !x
        //ParserToken_Operator_Logical_Bitwise_And, // &, example: x & y
        //ParserToken_Operator_Logical_Bitwise_Or, // |, example: x | y
        //ParserToken_Operator_Logical_Bitwise_ExcluseOr, // ^, example: x ^ y
        //ParserToken_Operator_Logical_Bitwise_Complement, // ~ (as left expression), example: ~x

        ParserToken_Scope_RoundBracket_Open, // (
        ParserToken_Scope_RoundBracket_Closed, // )
        ParserToken_Scope_SquareBracket_Open, // [
        ParserToken_Scope_SquareBracket_Closed, // ]
        ParserToken_Scope_CurlyBracket_Open, // {
        ParserToken_Scope_CurlyBracket_Closed, // }
        //ParserToken_Scope_AngleBracket_Open, // <
        //ParserToken_Scope_AngleBracket_Closed, // >

        ParserToken_Keyword_Function, // function
        ParserToken_Keyword_Var, // var
        ParserToken_Keyword_Return, // return
        //ParserToken_Keyword_Let, // let
        //ParserToken_Keyword_Import, // import
        //ParserToken_Keyword_Class, // class
        //ParserToken_Keyword_This, // this
        //ParserToken_Keyword_Struct, // struct
        //ParserToken_Keyword_Enum, // enum
        //ParserToken_Keyword_Namespace, // namespace
        //ParserToken_Keyword_Static, // statoc

        ParserToken_Keyword_Type_Int, // int
        ParserToken_Keyword_Type_Float, // float
        ParserToken_Keyword_Type_Bool, // bool
        //ParserToken_Keyword_Type_Double, // double
        //ParserToken_Keyword_Type_Char, // char
        //ParserToken_Keyword_Type_String, // string
        //ParserToken_Keyword_Type_Byte, // byte
        //ParserToken_Keyword_Type_Long, // long
        //ParserToken_Keyword_Type_UInt, // uint
        //ParserToken_Keyword_Type_ULong, // ulong
        //ParserToken_Keyword_Type_UByte, // ubyte
        //ParserToken_Keyword_Type_List, // list, can change later
        //ParserToken_Keyword_Type_Array, // array, can change later

        ParserToken_Identifier, // Any characters sequence other than a keyword.
        ParserToken_TypeDefiner, // :, example: var test: int = 0;
        ParserToken_Preprocessor, // #, #TEST "Aryiele"
        ParserToken_EOF, // ;, example: return 0;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_PARSERTOKEN_H. */
