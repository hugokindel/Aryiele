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

#include <iostream>
#include <fcntl.h>
#include <utility>
#include <llvm/ADT/STLExtras.h>
#include <Aryiele/Parser/Parser.h>
#include <Aryiele/AST/Nodes/NodeConstantDouble.h>
#include <Aryiele/AST/Nodes/NodeConstantInteger.h>
#include <Aryiele/AST/Nodes/NodeOperationBinary.h>
#include <Aryiele/AST/Nodes/NodeStatementBlock.h>
#include <Aryiele/AST/Nodes/NodeStatementFunctionCall.h>
#include <Aryiele/AST/Nodes/NodeStatementIf.h>
#include <Aryiele/AST/Nodes/NodeStatementReturn.h>
#include <Aryiele/AST/Nodes/NodeStatementVariableDeclaration.h>
#include <Aryiele/AST/Nodes/NodeVariable.h>

namespace Aryiele {
    /* TODO: Add operators:
 * Bitwise operator ?
 * Ternary operator ?
 * Negative logical operator ? */
    Parser::Parser() {
        m_binaryOperatorPrecedence[";"] = -1;
    
        m_binaryOperatorPrecedence["="] = 10;
    
        m_binaryOperatorPrecedence["&&"] = 20;
        m_binaryOperatorPrecedence["||"] = 20;
    
        m_binaryOperatorPrecedence["<"] = 30;
        m_binaryOperatorPrecedence[">"] = 30;
        m_binaryOperatorPrecedence["<="] = 30;
        m_binaryOperatorPrecedence[">="] = 30;
        m_binaryOperatorPrecedence["=="] = 30;
        m_binaryOperatorPrecedence["!="] = 30;
    
        m_binaryOperatorPrecedence["+"] = 40;
        m_binaryOperatorPrecedence["-"] = 40;
    
        m_binaryOperatorPrecedence["%"] = 50;
        m_binaryOperatorPrecedence["*"] = 50;
        m_binaryOperatorPrecedence["/"] = 50;
    }
    
    std::vector<ParserToken> Parser::ConvertTokens(std::vector<LexerToken> LexerTokens) {
        auto tokens = std::vector<ParserToken>();
        auto lastToken = LexerToken(std::string(), LexerToken_Unknown);

        for (auto& token : LexerTokens) {
            switch (token.Type) {
                case LexerToken_Number:
                    // Numbers.
                    if (token.Content.find('.') != std::string::npos)
                        tokens.emplace_back(token.Content, ParserToken_LiteralValueDecimal);
                    else
                        tokens.emplace_back(token.Content, ParserToken_LiteralValueInteger);
                    break;
                case LexerToken_String:
                    tokens.emplace_back(token.Content, ParserToken_LiteralValueString);
                    break;
                case LexerToken_Operator:
                    // Operators.
                    if (token.Content == "=")
                        tokens.emplace_back(token.Content, ParserToken_OperatorEqual);
                    // Arithmetic Operators.
                    else if ((token.Content == "+" &&
                             lastToken.Type != LexerToken_Number &&
                             lastToken.Type != LexerToken_Identifier) &&
                             lastToken.Content != "(" &&
                             lastToken.Content != ")")
                        tokens.emplace_back(token.Content, ParserToken_OperatorArithmeticUnaryPlus);
                    else if (token.Content == "+")
                        tokens.emplace_back(token.Content, ParserToken_OperatorArithmeticPlus);
                    else if ((token.Content == "-" &&
                              lastToken.Type != LexerToken_Number &&
                              lastToken.Type != LexerToken_Identifier) &&
                              lastToken.Content != "(" &&
                              lastToken.Content != ")")
                        tokens.emplace_back(token.Content, ParserToken_OperatorArithmeticUnaryMinus);
                    else if (token.Content == "-")
                        tokens.emplace_back(token.Content, ParserToken_OperatorArithmeticMinus);
                    else if (token.Content == "*")
                        tokens.emplace_back(token.Content, ParserToken_OperatorArithmeticMultiply);
                    else if (token.Content == "/")
                        tokens.emplace_back(token.Content, ParserToken_OperatorArithmeticDivide);
                    else if (token.Content == "%")
                        tokens.emplace_back(token.Content, ParserToken_OperatorArithmeticRemainder);
                    // Comparison Operators.
                    else if (token.Content == "==")
                        tokens.emplace_back(token.Content, ParserToken_OperatorComparisonEqual);
                    else if (token.Content == "!=")
                        tokens.emplace_back(token.Content, ParserToken_OperatorComparisonNotEqual);
                    else if (token.Content == "<")
                        tokens.emplace_back(token.Content, ParserToken_OperatorComparisonLessThan);
                    else if (token.Content == ">")
                        tokens.emplace_back(token.Content, ParserToken_OperatorComparisonGreaterThan);
                    else if (token.Content == "<=")
                        tokens.emplace_back(token.Content, ParserToken_OperatorComparisonLessThanOrEqual);
                    else if (token.Content == ">=")
                        tokens.emplace_back(token.Content, ParserToken_OperatorComparisonGreaterThanOrEqual);
                    // Logical Operators.
                    else if (token.Content == "&&")
                        tokens.emplace_back(token.Content, ParserToken_OperatorLogicalAnd);
                    else if (token.Content == "||")
                        tokens.emplace_back(token.Content, ParserToken_OperatorLogicalOr);
                    else if (token.Content == "!")
                        tokens.emplace_back(token.Content, ParserToken_OperatorLogicalNot);
                    else
                        tokens.emplace_back(token.Content, ParserToken_Unknown);
                    break;
                case LexerToken_Separator:
                    // Separators
                    if (token.Content == "(")
                        tokens.emplace_back(token.Content, ParserToken_SeparatorRoundBracketOpen);
                    else if (token.Content == ")")
                        tokens.emplace_back(token.Content, ParserToken_SeparatorRoundBracketClosed);
                    else if (token.Content == "[")
                        tokens.emplace_back(token.Content, ParserToken_SeparatorSquareBracketOpen);
                    else if (token.Content == "]")
                        tokens.emplace_back(token.Content, ParserToken_SeparatorSquareBracketClosed);
                    else if (token.Content == "{")
                        tokens.emplace_back(token.Content, ParserToken_SeparatorCurlyBracketOpen);
                    else if (token.Content == "}")
                        tokens.emplace_back(token.Content, ParserToken_SeparatorCurlyBracketClosed);
                    else if (token.Content == ";")
                        tokens.emplace_back(token.Content, ParserToken_SeparatorSemicolon);
                    else if (token.Content == ":")
                        tokens.emplace_back(token.Content, ParserToken_SeparatorColon);
                    else if (token.Content == ",")
                        tokens.emplace_back(token.Content, ParserToken_SeparatorComma);
                    else
                        tokens.emplace_back(token.Content, ParserToken_Unknown);
                    break;
                case LexerToken_Identifier:
                    // Boolean
                    if (token.Content == "true" || token.Content == "false")
                        tokens.emplace_back(token.Content, ParserToken_LiteralValueBoolean);
                    // Keywords
                    else if (token.Content == "func")
                        tokens.emplace_back(token.Content, ParserToken_Keyword_TopLevel_Function);
                    else if (token.Content == "var")
                        tokens.emplace_back(token.Content, ParserToken_KeywordVar);
                    else if (token.Content == "return")
                        tokens.emplace_back(token.Content, ParserToken_KeywordReturn);
                    else if (token.Content == "if")
                        tokens.emplace_back(token.Content, ParserToken_KeywordIf);
                    else if (token.Content == "else")
                        tokens.emplace_back(token.Content, ParserToken_KeywordElse);
                    else
                        tokens.emplace_back(token.Content, ParserToken_Identifier);
                    break;
                default:
                    tokens.emplace_back(token.Content, ParserToken_Unknown);
                    break;
            }

            lastToken = token;
        }

        return tokens;
    }

    std::string Parser::GetTokenName(ParserTokenEnum tokenType) {
        switch (tokenType) {
            case ParserToken_LiteralValueInteger:
                return "LiteralValueInteger";
            case ParserToken_LiteralValueDecimal:
                return "LiteralValueDecimal";
            case ParserToken_LiteralValueString:
                return "LiteralValueString";
            case ParserToken_LiteralValueBoolean:
                return "LiteralValueBoolean";
            case ParserToken_OperatorEqual:
                return "OperatorEqual";
            case ParserToken_OperatorArithmeticPlus:
                return "OperatorArithmeticPlus";
            case ParserToken_OperatorArithmeticUnaryPlus:
                return "OperatorArithmeticUnaryPlus";
            case ParserToken_OperatorArithmeticMinus:
                return "OperatorArithmeticMinus";
            case ParserToken_OperatorArithmeticUnaryMinus:
                return "OperatorArithmeticUnaryMinus";
            case ParserToken_OperatorArithmeticMultiply:
                return "OperatorArithmeticMultiply";
            case ParserToken_OperatorArithmeticDivide:
                return "OperatorArithmeticDivide";
            case ParserToken_OperatorArithmeticRemainder:
                return "OperatorArithmeticRemainder";
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
            case ParserToken_OperatorLogicalNot:
                return "OperatorLogicalNot";
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
            case ParserToken_SeparatorSemicolon:
                return "SeparatorSemicolon";
            case ParserToken_Keyword_TopLevel_Function:
                return "KeywordTopLevelFunction";
            case ParserToken_KeywordVar:
                return "KeywordVar";
            case ParserToken_KeywordReturn:
                return "KeywordReturn";
            case ParserToken_KeywordIf:
                return "KeywordIf";
            case ParserToken_KeywordElse:
                return "KeywordElse";
            case ParserToken_Identifier:
                return "Identifier";
            case ParserToken_EOF:
                return "EOF";
            default:
                return "Unknown";
        }
    }

    std::vector<std::shared_ptr<Node>> Parser::Parse(std::vector<ParserToken> tokens) {
        m_tokens = std::move(tokens);
        m_tokens.emplace_back("", ParserToken_EOF);

        while (true) {
            GetNextToken();

            if (m_currentToken.Type == ParserToken_EOF)
                break;
            if (m_currentToken.Type == ParserToken_Keyword_TopLevel_Function)
                m_nodes.emplace_back(ParseFunction());
        }

        return m_nodes;
    }

    ParserToken Parser::GetCurrentToken() {
        return m_currentToken;
    }

    ParserToken Parser::GetNextToken() {
        m_currentToken = m_tokens[++m_currentTokenIndex];

        return m_currentToken;
    }

    ParserToken Parser::GetPreviousToken() {
        if (m_currentTokenIndex > 0)
            m_currentToken = m_tokens[--m_currentTokenIndex];

        return m_currentToken;
    }
    
    int Parser::GetOperatorPrecedence(const std::string &binaryOperator) {
        if(m_binaryOperatorPrecedence.find(binaryOperator) == m_binaryOperatorPrecedence.end())
            return -1;
        else
            return m_binaryOperatorPrecedence[binaryOperator] <= 0 ? -1 : m_binaryOperatorPrecedence[binaryOperator];
    }

    std::shared_ptr<NodeFunction> Parser::ParseFunction() {
        std::string name;
        std::string type;
        std::vector<Argument> arguments;

        GetNextToken();

        if (m_currentToken.Type == ParserToken_Identifier)
            name = m_currentToken.Content;
        else {
            LOG_ERROR("Expected an identifier.");

            return nullptr;
        }

        GetNextToken();

        if (m_currentToken.Type != ParserToken_SeparatorRoundBracketOpen) {
            LOG_ERROR("Expected an opened round bracket.");

            return nullptr;
        }

        while (true) {
            GetNextToken();

            if (m_currentToken.Type == ParserToken_SeparatorRoundBracketClosed)
                break;
            else if (m_currentToken.Type == ParserToken_Identifier) {
                auto identifier = m_currentToken.Content;

                GetNextToken();
                PARSER_CHECKTOKEN(ParserToken_SeparatorColon);

                GetNextToken();
                PARSER_CHECKTOKEN(ParserToken_Identifier);

                arguments.emplace_back(Argument(identifier, m_currentToken.Content));
            }
            else if (m_currentToken.Type == ParserToken_SeparatorComma) {
                continue;
            }
            else {
                LOG_ERROR("Expected either a closed round bracket or a variable.");

                return nullptr;
            }
        }

        GetNextToken();

        PARSER_CHECKTOKEN(ParserToken_SeparatorColon);

        GetNextToken();

        if (m_currentToken.Type == ParserToken_Identifier)
            type = m_currentToken.Content;
        else {
            LOG_ERROR("Expected a type name.");

            return nullptr;
        }

        GetNextToken();

        PARSER_CHECKTOKEN(ParserToken_SeparatorCurlyBracketOpen);

        auto expressions = ParseBody();

        return std::make_shared<NodeFunction>(name, type, arguments, expressions);
    }

    std::shared_ptr<Node> Parser::ParsePrimary() {
        switch (m_currentToken.Type) {
            case ParserToken_LiteralValueInteger:
                return ParseInteger();
            case ParserToken_LiteralValueDecimal:
                return ParseDouble();
            case ParserToken_SeparatorRoundBracketOpen:
                return ParseParenthese();
            case ParserToken_KeywordReturn:
                return ParseReturn();
            case ParserToken_KeywordIf:
                return ParseIf();
            case ParserToken_Identifier:
                return ParseIdentifier();
            case ParserToken_SeparatorCurlyBracketOpen:
                return ParseBlock();
            case ParserToken_KeywordVar:
                return ParseVariableDeclaration();

            default:
                return nullptr;
        }
    }

    std::shared_ptr<Node> Parser::ParseExpression() {
        auto leftExpression = ParsePrimary();

        if (!leftExpression)
            return nullptr;

        return ParseBinaryOperation(0, leftExpression);
    }

    std::shared_ptr<Node> Parser::ParseBinaryOperation(int expressionPrecedence, std::shared_ptr<Node> leftExpression) {
        while (true) {
            int tokenPrecedence = GetOperatorPrecedence(m_currentToken.Content);

            if (tokenPrecedence < expressionPrecedence)
                return leftExpression;

            auto operationType = m_currentToken.Type;

            GetNextToken();

            auto rightExpression = ParsePrimary();

            if (!rightExpression)
                return nullptr;

            int nextPrecedence = GetOperatorPrecedence(m_currentToken.Content);

            if (tokenPrecedence < nextPrecedence) {
                rightExpression = ParseBinaryOperation(tokenPrecedence + 1, std::move(rightExpression));

                if (!rightExpression)
                    return nullptr;
            }

            leftExpression = std::make_shared<NodeOperationBinary>(operationType, std::move(leftExpression), rightExpression);
        }
    }

    std::vector<std::shared_ptr<Node>> Parser::ParseBody() {
        std::vector<std::shared_ptr<Node>> expressions;

        while (true) {
            GetNextToken();

            if (m_currentToken.Type == ParserToken_SeparatorCurlyBracketClosed)
                break;

            auto expression = ParseExpression();

            if (expression)
                expressions.emplace_back(expression);
        }

        return expressions;
    }

    std::shared_ptr<Node> Parser::ParseInteger() {
        auto result = std::make_shared<NodeConstantInteger>(std::stoi(m_currentToken.Content));

        GetNextToken();

        return result;
    }

    std::shared_ptr<Node> Parser::ParseDouble() {
        auto result = std::make_shared<NodeConstantDouble>(std::stod(m_currentToken.Content));

        GetNextToken();

        return result;
    }

    std::shared_ptr<Node> Parser::ParseIdentifier() {
        auto identifier = m_currentToken.Content;

        GetNextToken();

        if (m_currentToken.Type == ParserToken_SeparatorRoundBracketOpen) {
            GetNextToken();

            std::vector<std::shared_ptr<Node>> arguments;

            if (m_currentToken.Type != ParserToken_SeparatorRoundBracketClosed) {
                while (true) {
                    if (auto arg = ParseExpression())
                        arguments.emplace_back(arg);
                    else
                        return nullptr;

                    if (m_currentToken.Type == ParserToken_SeparatorRoundBracketClosed)
                        break;

                    if (m_currentToken.Type != ParserToken_SeparatorComma)
                    {
                        LOG_ERROR("Expected ')' or ',' in argument list");
                    }

                    GetNextToken();
                }
            }

            GetNextToken();

            return std::make_shared<NodeStatementFunctionCall>(identifier, arguments);
        }
        else {
            return std::make_shared<NodeVariable>(identifier);
        }
    }

    std::shared_ptr<Node> Parser::ParseParenthese() {
        GetNextToken();

        auto expression = ParseExpression();

        GetNextToken();

        return expression;
    }

    std::shared_ptr<Node> Parser::ParseReturn() {
        GetNextToken();

        return std::make_shared<NodeStatementReturn>(ParseExpression());
    }

    std::shared_ptr<Node> Parser::ParseIf() {
        GetNextToken();

        auto condition = ParseParenthese();

        if (!condition) {
            LOG_ERROR("Cannot parse if condition");

            return nullptr;
        }

        PARSER_CHECKTOKEN(ParserToken_SeparatorCurlyBracketOpen);

        auto ifBody = ParseBody();

        PARSER_CHECKTOKEN(ParserToken_SeparatorCurlyBracketClosed);

        std::vector<std::shared_ptr<Node>> elseBody;

        GetNextToken();

        if (m_currentToken.Type == ParserToken_KeywordElse) {
            GetNextToken();

            if (m_currentToken.Type == ParserToken_KeywordIf) {
                elseBody.emplace_back(ParseIf());

                PARSER_CHECKTOKEN(ParserToken_SeparatorCurlyBracketClosed);
            }
            else {
                PARSER_CHECKTOKEN(ParserToken_SeparatorCurlyBracketOpen);

                elseBody = ParseBody();

                PARSER_CHECKTOKEN(ParserToken_SeparatorCurlyBracketClosed);
            }
        }
        else {
            GetPreviousToken();
        }

        return std::make_shared<NodeStatementIf>(condition, ifBody, elseBody);
    }

    std::shared_ptr<Node> Parser::ParseBlock() {
        auto block = std::make_shared<NodeStatementBlock>();

        block->Body = ParseBody();

        return block;
    }

    // TODO: PARSER: Define a variable with no default value (eg: var x: int;).
    // TODO: PARSER: Define multiples variables at once (eg: var x: int = 0, y: int, z: int = 2, w: int = z;).
    std::shared_ptr<Node> Parser::ParseVariableDeclaration() {
        std::vector<std::shared_ptr<Variable>> variables;

        while (true) {
            PARSER_CHECKNEXTTOKEN(ParserToken_Identifier);

            auto identifier = m_currentToken.Content;

            PARSER_CHECKNEXTTOKEN(ParserToken_SeparatorColon);
            PARSER_CHECKNEXTTOKEN(ParserToken_Identifier);

            auto type = m_currentToken.Content;

            GetNextToken();

            std::shared_ptr<Node> value = nullptr;

            if (m_currentToken.Type == ParserToken_OperatorEqual) {
                GetNextToken();

                value = ParseExpression();
            }

            variables.emplace_back(std::make_shared<Variable>(identifier, type, value));

            if (m_currentToken.Type != ParserToken_SeparatorComma)
                break;
        }

        return std::make_shared<NodeStatementVariableDeclaration>(variables);
    }
    
    Parser &GetParser() {
        return Parser::getInstance();
    }
    
} /* Namespace Aryiele. */
