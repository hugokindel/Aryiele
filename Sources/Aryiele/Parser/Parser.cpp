#include <Aryiele/Parser/Parser.h>
#include <Aryiele/AST/Nodes/NodeConstantDouble.h>
#include <Aryiele/AST/Nodes/NodeConstantInteger.h>
#include <Aryiele/AST/Nodes/NodeOperationBinary.h>
#include <Aryiele/AST/Nodes/NodeStatementBlock.h>
#include <Aryiele/AST/Nodes/NodeStatementFunctionCall.h>
#include <Aryiele/AST/Nodes/NodeStatementIf.h>
#include <Aryiele/AST/Nodes/NodeStatementReturn.h>
#include <Aryiele/AST/Nodes/NodeVariable.h>
#include <Vanir/StringUtils.h>
#include <llvm/ADT/STLExtras.h>
#include <iostream>
#include <fcntl.h>
#include <utility>

namespace Aryiele
{
    std::vector<ParserToken> Parser::ConvertTokens(std::vector<LexerToken> LexerTokens)
    {
        auto tokens = std::vector<ParserToken>();
        auto lastToken = LexerToken(std::string(), LexerTokens_Unknown);

        for (auto& token : LexerTokens)
        {
            switch (token.Type)
            {
                case LexerTokens_Number:
                    // Numbers.
                    if (token.Content.find('.') != std::string::npos)
                        tokens.emplace_back(token.Content, ParserTokens_LiteralValue_Decimal);
                    else
                        tokens.emplace_back(token.Content, ParserTokens_LiteralValue_Integer);
                    break;
                case LexerTokens_String:
                    tokens.emplace_back(token.Content, ParserTokens_LiteralValue_String);
                    break;
                case LexerTokens_Operator:
                    // Operators.
                    if (token.Content == "=")
                        tokens.emplace_back(token.Content, ParserTokens_Operator_Equal);
                    // Arithmetic Operators.
                    else if ((token.Content == "+" &&
                             lastToken.Type != LexerTokens_Number &&
                             lastToken.Type != LexerTokens_Identifier) &&
                             lastToken.Content != "(" &&
                             lastToken.Content != ")")
                        tokens.emplace_back(token.Content, ParserTokens_Operator_Arithmetic_UnaryPlus);
                    else if (token.Content == "+")
                        tokens.emplace_back(token.Content, ParserTokens_Operator_Arithmetic_Plus);
                    else if ((token.Content == "-" &&
                              lastToken.Type != LexerTokens_Number &&
                              lastToken.Type != LexerTokens_Identifier) &&
                              lastToken.Content != "(" &&
                              lastToken.Content != ")")
                        tokens.emplace_back(token.Content, ParserTokens_Operator_Arithmetic_UnaryMinus);
                    else if (token.Content == "-")
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
                    else
                        tokens.emplace_back(token.Content, ParserTokens_Unknown);
                    break;
                case LexerTokens_Separator:
                    // Separators
                    if (token.Content == "(")
                        tokens.emplace_back(token.Content, ParserTokens_Separator_RoundBracket_Open);
                    else if (token.Content == ")")
                        tokens.emplace_back(token.Content, ParserTokens_Separator_RoundBracket_Closed);
                    else if (token.Content == "[")
                        tokens.emplace_back(token.Content, ParserTokens_Separator_SquareBracket_Open);
                    else if (token.Content == "]")
                        tokens.emplace_back(token.Content, ParserTokens_Separator_SquareBracket_Closed);
                    else if (token.Content == "{")
                        tokens.emplace_back(token.Content, ParserTokens_Separator_CurlyBracket_Open);
                    else if (token.Content == "}")
                        tokens.emplace_back(token.Content, ParserTokens_Separator_CurlyBracket_Closed);
                    else if (token.Content == ";")
                        tokens.emplace_back(token.Content, ParserTokens_Separator_Semicolon);
                    else if (token.Content == ":")
                        tokens.emplace_back(token.Content, ParserTokens_Separator_Colon);
                    else if (token.Content == ",")
                        tokens.emplace_back(token.Content, ParserTokens_Separator_Comma);
                    else
                        tokens.emplace_back(token.Content, ParserTokens_Unknown);
                    break;
                case LexerTokens_Identifier:
                    // Boolean
                    if (token.Content == "true" || token.Content == "false")
                        tokens.emplace_back(token.Content, ParserTokens_LiteralValue_Boolean);
                    // Keywords
                    else if (token.Content == "func")
                        tokens.emplace_back(token.Content, ParserTokens_Keyword_TopLevel_Function);
                    else if (token.Content == "var")
                        tokens.emplace_back(token.Content, ParserTokens_Keyword_Var);
                    else if (token.Content == "return")
                        tokens.emplace_back(token.Content, ParserTokens_Keyword_Return);
                    else if (token.Content == "if")
                        tokens.emplace_back(token.Content, ParserTokens_Keyword_If);
                    else if (token.Content == "else")
                        tokens.emplace_back(token.Content, ParserTokens_Keyword_Else);
                    else
                        tokens.emplace_back(token.Content, ParserTokens_Identifier);
                    break;
                default:
                    tokens.emplace_back(token.Content, ParserTokens_Unknown);
                    break;
            }

            lastToken = token;
        }

        return tokens;
    }

    std::string Parser::GetTokenName(ParserTokens tokenType)
    {
        switch (tokenType)
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
            case ParserTokens_Separator_RoundBracket_Open:
                return "Scope_RoundBracket_Open";
            case ParserTokens_Separator_RoundBracket_Closed:
                return "Scope_RoundBracket_Closed";
            case ParserTokens_Separator_SquareBracket_Open:
                return "Scope_SquareBracket_Open";
            case ParserTokens_Separator_SquareBracket_Closed:
                return "Scope_SquareBracket_Closed";
            case ParserTokens_Separator_CurlyBracket_Open:
                return "Scope_CurlyBracket_Open";
            case ParserTokens_Separator_CurlyBracket_Closed:
                return "Scope_CurlyBracket_Closed";
            case ParserTokens_Separator_Colon:
                return "Separator_Colon";
            case ParserTokens_Separator_Comma:
                return "Separator_Comma";
            case ParserTokens_Separator_Semicolon:
                return "Separator_Semicolon";
            case ParserTokens_Keyword_TopLevel_Function:
                return "Keyword_TopLevel_Function";
            case ParserTokens_Keyword_Var:
                return "Keyword_Var";
            case ParserTokens_Keyword_Return:
                return "Keyword_Return";
            case ParserTokens_Keyword_If:
                return "Keyword_If";
            case ParserTokens_Keyword_Else:
                return "Keyword_Else";
            case ParserTokens_Identifier:
                return "Identifier";
            case ParserTokens_EOF:
                return "EOF";
            default:
                return "Unknown";
        }
    }

    std::vector<std::shared_ptr<Node>> Parser::Parse(std::vector<ParserToken> tokens)
    {
        m_tokens = std::move(tokens);
        m_tokens.emplace_back("", ParserTokens_EOF);

        while (true)
        {
            GetNextToken();

            if (m_currentToken.Type == ParserTokens_EOF)
                break;
            if (m_currentToken.Type == ParserTokens_Keyword_TopLevel_Function)
                m_nodes.emplace_back(ParseFunction());
        }

        return m_nodes;
    }

    ParserToken Parser::GetCurrentToken()
    {
        return m_currentToken;
    }

    ParserToken Parser::GetNextToken()
    {
        m_currentToken = m_tokens[++m_currentTokenIndex];

        return m_currentToken;
    }

    ParserToken Parser::GetPreviousToken()
    {
        if (m_currentTokenIndex > 0)
            m_currentToken = m_tokens[--m_currentTokenIndex];

        return m_currentToken;
    }

    std::shared_ptr<NodeFunction> Parser::ParseFunction()
    {
        std::string name;
        std::string type;
        std::vector<Argument> arguments;

        GetNextToken();

        if (m_currentToken.Type == ParserTokens_Identifier)
            name = m_currentToken.Content;
        else
        {
            LOG_ERROR("Expected an identifier.");

            return nullptr;
        }

        GetNextToken();

        if (m_currentToken.Type != ParserTokens_Separator_RoundBracket_Open)
        {
            LOG_ERROR("Expected an opened round bracket.");

            return nullptr;
        }

        while (true)
        {
            GetNextToken();

            if (m_currentToken.Type == ParserTokens_Separator_RoundBracket_Closed)
                break;
            else if (m_currentToken.Type == ParserTokens_Identifier)
            {
                auto identifier = m_currentToken.Content;

                GetNextToken();
                PARSER_CHECKTOKEN(ParserTokens_Separator_Colon);

                GetNextToken();
                PARSER_CHECKTOKEN(ParserTokens_Identifier);

                arguments.emplace_back(Argument(identifier, m_currentToken.Content));
            }
            else if (m_currentToken.Type == ParserTokens_Separator_Comma)
            {
                continue;
            }
            else
            {
                LOG_ERROR("Expected either a closed round bracket or a variable.");

                return nullptr;
            }
        }

        GetNextToken();

        PARSER_CHECKTOKEN(ParserTokens_Separator_Colon);

        GetNextToken();

        if (m_currentToken.Type == ParserTokens_Identifier)
            type = m_currentToken.Content;
        else
        {
            LOG_ERROR("Expected a type name.");

            return nullptr;
        }

        GetNextToken();

        PARSER_CHECKTOKEN(ParserTokens_Separator_CurlyBracket_Open);

        auto expressions = ParseBody();

        return std::make_shared<NodeFunction>(name, type, arguments, expressions);
    }

    std::shared_ptr<Node> Parser::ParsePrimary()
    {
        switch (m_currentToken.Type)
        {
            case ParserTokens_LiteralValue_Integer:
                return ParseInteger();
            case ParserTokens_LiteralValue_Decimal:
                return ParseDouble();
            case ParserTokens_Separator_RoundBracket_Open:
                return ParseParenthese();
            case ParserTokens_Keyword_Return:
                return ParseReturn();
            case ParserTokens_Keyword_If:
                return ParseIf();
            case ParserTokens_Identifier:
                return ParseIdentifier();
            case ParserTokens_Separator_CurlyBracket_Open:
                return ParseBlock();

            default:
                return nullptr;
        }
    }

    std::shared_ptr<Node> Parser::ParseExpression()
    {
        auto leftExpression = ParsePrimary();

        if (!leftExpression)
            return nullptr;

        return ParseBinaryOperation(0, leftExpression);
    }

    std::shared_ptr<Node> Parser::ParseBinaryOperation(int expressionPrecedence, std::shared_ptr<Node> leftExpression)
    {
        while (true)
        {
            int tokenPrecedence = ParserPrecedence::GetInstance()->GetPrecedence(m_currentToken.Content);

            if (tokenPrecedence < expressionPrecedence)
                return leftExpression;

            auto operationType = m_currentToken.Type;

            GetNextToken();

            auto rightExpression = ParsePrimary();

            if (!rightExpression)
                return nullptr;

            int nextPrecedence = ParserPrecedence::GetInstance()->GetPrecedence(m_currentToken.Content);

            if (tokenPrecedence < nextPrecedence)
            {
                rightExpression = ParseBinaryOperation(tokenPrecedence + 1, std::move(rightExpression));

                if (!rightExpression)
                    return nullptr;
            }

            leftExpression = std::make_shared<NodeOperationBinary>(operationType, std::move(leftExpression), rightExpression);
        }
    }

    std::vector<std::shared_ptr<Node>> Parser::ParseBody()
    {
        std::vector<std::shared_ptr<Node>> expressions;

        while (true)
        {
            GetNextToken();

            if (m_currentToken.Type == ParserTokens_Separator_CurlyBracket_Closed)
                break;

            auto expression = ParseExpression();

            if (expression)
                expressions.emplace_back(expression);
        }

        return expressions;
    }

    std::shared_ptr<Node> Parser::ParseInteger()
    {
        auto result = std::make_shared<NodeConstantInteger>(std::stoi(m_currentToken.Content));

        GetNextToken();

        return result;
    }

    std::shared_ptr<Node> Parser::ParseDouble()
    {
        auto result = std::make_shared<NodeConstantDouble>(std::stod(m_currentToken.Content));

        GetNextToken();

        return result;
    }

    std::shared_ptr<Node> Parser::ParseIdentifier()
    {
        auto identifier = m_currentToken.Content;

        GetNextToken();

        if (m_currentToken.Type == ParserTokens_Separator_RoundBracket_Open)
        {
            GetNextToken();

            std::vector<std::shared_ptr<Node>> arguments;

            if (m_currentToken.Type != ParserTokens_Separator_RoundBracket_Closed)
            {
                while (true)
                {
                    if (auto arg = ParseExpression())
                        arguments.emplace_back(arg);
                    else
                        return nullptr;

                    if (m_currentToken.Type == ParserTokens_Separator_RoundBracket_Closed)
                        break;

                    if (m_currentToken.Type != ParserTokens_Separator_Comma)
                    {
                        LOG_ERROR("Expected ')' or ',' in argument list");
                    }

                    GetNextToken();
                }
            }

            GetNextToken();

            return std::make_shared<NodeStatementFunctionCall>(identifier, arguments);
        }
        else
        {
            return std::make_shared<NodeVariable>(identifier);
        }
    }

    std::shared_ptr<Node> Parser::ParseParenthese()
    {
        GetNextToken();

        auto expression = ParseExpression();

        GetNextToken();

        return expression;
    }

    std::shared_ptr<Node> Parser::ParseReturn()
    {
        GetNextToken();

        return std::make_shared<NodeStatementReturn>(ParseExpression());
    }

    std::shared_ptr<Node> Parser::ParseIf()
    {
        GetNextToken();

        auto condition = ParseParenthese();

        if (!condition)
        {
            LOG_ERROR("Cannot parse if condition");

            return nullptr;
        }

        PARSER_CHECKTOKEN(ParserTokens_Separator_CurlyBracket_Open);

        auto if_body = ParseBody();

        PARSER_CHECKTOKEN(ParserTokens_Separator_CurlyBracket_Closed);

        std::vector<std::shared_ptr<Node>> else_body;

        GetNextToken();

        if (m_currentToken.Type == ParserTokens_Keyword_Else)
        {
            GetNextToken();

            PARSER_CHECKTOKEN(ParserTokens_Separator_CurlyBracket_Open);

            else_body = ParseBody();

            PARSER_CHECKTOKEN(ParserTokens_Separator_CurlyBracket_Closed);
        }
        else
        {
            GetPreviousToken();
        }

        return std::make_shared<NodeStatementIf>(condition, if_body, else_body);
    }

    std::shared_ptr<Node> Parser::ParseBlock()
    {
        auto block = std::make_shared<NodeStatementBlock>();

        block->Body = ParseBody();

        return block;
    }

} /* Namespace Aryiele. */
