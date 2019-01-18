#ifndef ARYIELE_PARSERTOKENS_H
#define ARYIELE_PARSERTOKENS_H

namespace Aryiele
{
    // TODO - V2: Implement all token types.
    enum ParserTokens
    {
        ParserTokens_LiteralValue_Integer, // A number, example: 9
        ParserTokens_LiteralValue_Decimal, // A decimal number, example: 5.5
        ParserTokens_LiteralValue_String, // A string, example: "This is a text"
        ParserTokens_LiteralValue_Boolean, // A boolean, example: true

        ParserTokens_Operator_Arithmetic_Plus, // +, example: 5 + 5
        ParserTokens_Operator_Arithmetic_UnaryPlus, // + (as left expression), example: +7
        ParserTokens_Operator_Arithmetic_Minus, // -, example: 5 - 8
        ParserTokens_Operator_Arithmetic_UnaryMinus, // - (as left expression), example: -2
        ParserTokens_Operator_Arithmetic_Multiply, // *, example: 5 * 7
        ParserTokens_Operator_Arithmetic_Divide, // /, example: 7 / 4
        ParserTokens_Operator_Arithmetic_Remainder, // %, example: 5 % 7

        ParserTokens_Operator_Comparison_Equal, // ==, example: x == y
        ParserTokens_Operator_Comparison_NotEqual, // !=, example: x != y
        ParserTokens_Operator_Comparison_LessThan, // <, example: x < y
        ParserTokens_Operator_Comparison_GreaterThan, // >, example: x > y
        ParserTokens_Operator_Comparison_LessThanOrEqual, // <=, example: x <= y
        ParserTokens_Operator_Comparison_GreaterThanOrEqual, // >=, example: x >= y

        ParserTokens_Operator_Logical_And, // &&, example: x && y
        ParserTokens_Operator_Logical_Or, // ||, example: x || y
        ParserTokens_Operator_Logical_Not, // ! (as left expression), example: !x
        //ParserTokens_Operator_Logical_Bitwise_And, // &, example: x & y
        //ParserTokens_Operator_Logical_Bitwise_Or, // |, example: x | y
        //ParserTokens_Operator_Logical_Bitwise_ExcluseOr, // ^, example: x ^ y
        //ParserTokens_Operator_Logical_Bitwise_Complement, // ~ (as left expression), example: ~x

        ParserTokens_Scope_RoundBracket_Open, // (
        ParserTokens_Scope_RoundBracket_Closed, // )
        ParserTokens_Scope_SquareBracket_Open, // [
        ParserTokens_Scope_SquareBracket_Closed, // ]
        ParserTokens_Scope_CurlyBracket_Open, // {
        ParserTokens_Scope_CurlyBracket_Closed, // }
        //ParserTokens_Scope_AngleBracket_Open, // <
        //ParserTokens_Scope_AngleBracket_Closed, // >

        ParserTokens_Keyword_Function, // function
        ParserTokens_Keyword_Var, // var
        ParserTokens_Keyword_Return, // return
        //ParserTokens_Keyword_Let, // let
        //ParserTokens_Keyword_Import, // import
        //ParserTokens_Keyword_Class, // class
        //ParserTokens_Keyword_This, // this
        //ParserTokens_Keyword_Struct, // struct
        //ParserTokens_Keyword_Enum, // enum
        //ParserTokens_Keyword_Namespace, // namespace
        //ParserTokens_Keyword_Static, // statoc

        ParserTokens_Keyword_Type_Int, // int
        ParserTokens_Keyword_Type_Float, // float
        ParserTokens_Keyword_Type_Bool, // bool
        //ParserTokens_Keyword_Type_Double, // double
        //ParserTokens_Keyword_Type_Char, // char
        //ParserTokens_Keyword_Type_String, // string
        //ParserTokens_Keyword_Type_Byte, // byte
        //ParserTokens_Keyword_Type_Long, // long
        //ParserTokens_Keyword_Type_UInt, // uint
        //ParserTokens_Keyword_Type_ULong, // ulong
        //ParserTokens_Keyword_Type_UByte, // ubyte
        //ParserTokens_Keyword_Type_List, // list, can change later
        //ParserTokens_Keyword_Type_Array, // array, can change later

        //ParserTokens_Preprocessor_Start, // #, #TEST "Aryiele"
        //ParserTokens_Preprocessor_Newline, // \, #TEST "Aryiele" \ "test"

        ParserTokens_Identifier, // Any characters sequence other than a keyword.
        ParserTokens_TypeDefiner, // :, example: var test: int = 0;
        ParserTokens_EOL, // ;, example: return 0;

        ParserTokens_Unknown
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_PARSERTOKENS_H. */
