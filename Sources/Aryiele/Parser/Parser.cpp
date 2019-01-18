#include <Aryiele/Parser/Parser.h>
#include <Vanir/StringUtils.h>

namespace Aryiele
{
    std::vector<ParserToken> Parser::ConvertTokens(std::vector<TokenizerToken> tokenizerTokens)
    {
        auto tokens = std::vector<ParserToken>();
        auto lastToken = TokenizerToken(std::string(), TokenizerTokens_Unknown);

        for (auto& token : tokenizerTokens)
        {
            switch (token.Type)
            {
                case TokenizerTokens_Number:
                    // Numbers.
                    if (token.Content.find('.') != std::string::npos)
                        tokens.emplace_back(token.Content, ParserTokens_LiteralValue_Decimal);
                    else
                        tokens.emplace_back(token.Content, ParserTokens_LiteralValue_Integer);
                    break;
                case TokenizerTokens_String:
                    tokens.emplace_back(token.Content, ParserTokens_LiteralValue_String);
                    break;
                case TokenizerTokens_Operator:
                    // Operators.
                    if (token.Content == "=")
                        tokens.emplace_back(token.Content, ParserTokens_Operator_Equal);
                    // Arithmetic Operators.
                    else if (token.Content == "+" && lastToken.Type != TokenizerTokens_Number)
                        tokens.emplace_back(token.Content, ParserTokens_Operator_Arithmetic_UnaryPlus);
                    else if (token.Content == "+" && lastToken.Type == TokenizerTokens_Number)
                        tokens.emplace_back(token.Content, ParserTokens_Operator_Arithmetic_Plus);
                    else if (token.Content == "-" && lastToken.Type != TokenizerTokens_Number)
                        tokens.emplace_back(token.Content, ParserTokens_Operator_Arithmetic_UnaryMinus);
                    else if (token.Content == "-" && lastToken.Type == TokenizerTokens_Number)
                        tokens.emplace_back(token.Content, ParserTokens_Operator_Arithmetic_Minus);
                    else if (token.Content == "*")
                        tokens.emplace_back(token.Content, ParserTokens_Operator_Arithmetic_Multiply);
                    else if (token.Content == "/")
                        tokens.emplace_back(token.Content, ParserTokens_Operator_Arithmetic_Divide);
                    else if (token.Content == "%")
                        tokens.emplace_back(token.Content, ParserTokens_Operator_Arithmetic_Remainder);
                    // Comparison Operators.
                    else if (token.Content == "==")
                        tokens.emplace_back(token.Content, ParserTokens_Operator_Comparison_Equal);
                    else if (token.Content == "!=")
                        tokens.emplace_back(token.Content, ParserTokens_Operator_Comparison_NotEqual);
                    else if (token.Content == "<")
                        tokens.emplace_back(token.Content, ParserTokens_Operator_Comparison_LessThan);
                    else if (token.Content == ">")
                        tokens.emplace_back(token.Content, ParserTokens_Operator_Comparison_GreaterThan);
                    else if (token.Content == "<=")
                        tokens.emplace_back(token.Content, ParserTokens_Operator_Comparison_LessThanOrEqual);
                    else if (token.Content == ">=")
                        tokens.emplace_back(token.Content, ParserTokens_Operator_Comparison_GreaterThanOrEqual);
                    // Logical Operators.
                    else if (token.Content == "&&")
                        tokens.emplace_back(token.Content, ParserTokens_Operator_Logical_And);
                    else if (token.Content == "||")
                        tokens.emplace_back(token.Content, ParserTokens_Operator_Logical_Or);
                    else if (token.Content == "!")
                        tokens.emplace_back(token.Content, ParserTokens_Operator_Logical_Not);
                    else if (token.Content == ":")
                        tokens.emplace_back(token.Content, ParserTokens_TypeDefiner);
                    else
                        tokens.emplace_back(token.Content, ParserTokens_Unknown);
                    break;
                case TokenizerTokens_Scope:
                    // Scopes
                    if (token.Content == "(")
                        tokens.emplace_back(token.Content, ParserTokens_Scope_RoundBracket_Open);
                    else if (token.Content == ")")
                        tokens.emplace_back(token.Content, ParserTokens_Scope_RoundBracket_Closed);
                    else if (token.Content == "[")
                        tokens.emplace_back(token.Content, ParserTokens_Scope_SquareBracket_Open);
                    else if (token.Content == "]")
                        tokens.emplace_back(token.Content, ParserTokens_Scope_SquareBracket_Closed);
                    else if (token.Content == "{")
                        tokens.emplace_back(token.Content, ParserTokens_Scope_CurlyBracket_Open);
                    else if (token.Content == "}")
                        tokens.emplace_back(token.Content, ParserTokens_Scope_CurlyBracket_Closed);
                    else
                        tokens.emplace_back(token.Content, ParserTokens_Unknown);
                    break;
                case TokenizerTokens_Identifier:
                    // Boolean
                    if (token.Content == "true" || token.Content == "false")
                        tokens.emplace_back(token.Content, ParserTokens_LiteralValue_Boolean);
                    // Keywords
                    else if (token.Content == "function")
                        tokens.emplace_back(token.Content, ParserTokens_Keyword_Function);
                    else if (token.Content == "var")
                        tokens.emplace_back(token.Content, ParserTokens_Keyword_Var);
                    else if (token.Content == "return")
                        tokens.emplace_back(token.Content, ParserTokens_Keyword_Return);
                    else if (token.Content == "int")
                        tokens.emplace_back(token.Content, ParserTokens_Keyword_Type_Int);
                    else if (token.Content == "float")
                        tokens.emplace_back(token.Content, ParserTokens_Keyword_Type_Float);
                    else if (token.Content == "bool")
                        tokens.emplace_back(token.Content, ParserTokens_Keyword_Type_Bool);
                    else
                        tokens.emplace_back(token.Content, ParserTokens_Identifier);
                    break;
                case TokenizerTokens_EOL:
                    tokens.emplace_back(token.Content, ParserTokens_EOL);
                    break;
                default:
                    tokens.emplace_back(token.Content, ParserTokens_Unknown);
                    break;
            }

            lastToken = token;
        }

        return tokens;
    }

    std::string Parser::GetTokenName(ParserToken tokenType)
    {
        switch (tokenType.Type)
        {

            case ParserTokens_LiteralValue_Integer:
                return "LiteralValue_Integer";
            case ParserTokens_LiteralValue_Decimal:
                return "LiteralValue_Decimal";
            case ParserTokens_LiteralValue_String:
                return "LiteralValue_String";
            case ParserTokens_LiteralValue_Boolean:
                return "LiteralValue_Boolean";
            case ParserTokens_Operator_Equal:
                return "Operator_Equal";
            case ParserTokens_Operator_Arithmetic_Plus:
                return "Operator_Arithmetic_Plus";
            case ParserTokens_Operator_Arithmetic_UnaryPlus:
                return "Operator_Arithmetic_UnaryPlus";
            case ParserTokens_Operator_Arithmetic_Minus:
                return "Operator_Arithmetic_Minus";
            case ParserTokens_Operator_Arithmetic_UnaryMinus:
                return "Operator_Arithmetic_UnaryMinus";
            case ParserTokens_Operator_Arithmetic_Multiply:
                return "Operator_Arithmetic_Multiply";
            case ParserTokens_Operator_Arithmetic_Divide:
                return "Operator_Arithmetic_Divide";
            case ParserTokens_Operator_Arithmetic_Remainder:
                return "Operator_Arithmetic_Remainder";
            case ParserTokens_Operator_Comparison_Equal:
                return "Operator_Comparison_Equal";
            case ParserTokens_Operator_Comparison_NotEqual:
                return "Operator_Comparison_NotEqual";
            case ParserTokens_Operator_Comparison_LessThan:
                return "Operator_Comparison_LessThan";
            case ParserTokens_Operator_Comparison_GreaterThan:
                return "Operator_Comparison_GreaterThan";
            case ParserTokens_Operator_Comparison_LessThanOrEqual:
                return "Operator_Comparison_LessThanOrEqual";
            case ParserTokens_Operator_Comparison_GreaterThanOrEqual:
                return "Operator_Comparison_GreaterThanOrEqual";
            case ParserTokens_Operator_Logical_And:
                return "Operator_Logical_And";
            case ParserTokens_Operator_Logical_Or:
                return "Operator_Logical_Or";
            case ParserTokens_Operator_Logical_Not:
                return "Operator_Logical_Not";
            case ParserTokens_Scope_RoundBracket_Open:
                return "Scope_RoundBracket_Open";
            case ParserTokens_Scope_RoundBracket_Closed:
                return "Scope_RoundBracket_Closed";
            case ParserTokens_Scope_SquareBracket_Open:
                return "Scope_SquareBracket_Open";
            case ParserTokens_Scope_SquareBracket_Closed:
                return "Scope_SquareBracket_Closed";
            case ParserTokens_Scope_CurlyBracket_Open:
                return "Scope_CurlyBracket_Open";
            case ParserTokens_Scope_CurlyBracket_Closed:
                return "Scope_CurlyBracket_Closed";
            case ParserTokens_Keyword_Function:
                return "Keyword_Function";
            case ParserTokens_Keyword_Var:
                return "Keyword_Var";
            case ParserTokens_Keyword_Return:
                return "Keyword_Return";
            case ParserTokens_Keyword_Type_Int:
                return "Keyword_Type_Int";
            case ParserTokens_Keyword_Type_Float:
                return "Keyword_Type_Float";
            case ParserTokens_Keyword_Type_Bool:
                return "Keyword_Type_Bool";
            case ParserTokens_Identifier:
                return "Identifier";
            case ParserTokens_TypeDefiner:
                return "TypeDefiner";
            case ParserTokens_EOL:
                return "EOL";
            default:
                return "Unknown";
        }
    }

} /* Namespace Aryiele. */
