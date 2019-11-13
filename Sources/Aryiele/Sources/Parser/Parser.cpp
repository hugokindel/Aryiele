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
    
    std::vector<std::shared_ptr<Node>> Parser::parse(std::vector<ParserToken> tokens) {
        m_tokens = std::move(tokens);
        m_tokens.emplace_back("", ParserToken_EOF);
        
        while (true) {
            getNextToken();
            
            if (m_currentToken.type == ParserToken_EOF)
                break;
            if (m_currentToken.type == ParserToken_Keyword_TopLevel_Function)
                m_nodes.emplace_back(parseFunction());
        }
        
        return m_nodes;
    }
    
    std::vector<ParserToken> Parser::convertTokens(std::vector<LexerToken> LexerTokens) {
        auto tokens = std::vector<ParserToken>();
        auto lastToken = LexerToken(std::string(), LexerToken_Unknown);

        for (auto& token : LexerTokens) {
            switch (token.type) {
                case LexerToken_Number:
                    // Numbers.
                    if (token.content.find('.') != std::string::npos)
                        tokens.emplace_back(token.content, ParserToken_LiteralValueDecimal);
                    else
                        tokens.emplace_back(token.content, ParserToken_LiteralValueInteger);
                    break;
                case LexerToken_String:
                    tokens.emplace_back(token.content, ParserToken_LiteralValueString);
                    break;
                case LexerToken_Operator:
                    // Operators.
                    if (token.content == "=")
                        tokens.emplace_back(token.content, ParserToken_OperatorEqual);
                    // Arithmetic Operators.
                    else if ((token.content == "+" &&
                             lastToken.type != LexerToken_Number &&
                             lastToken.type != LexerToken_Identifier) &&
                             lastToken.content != "(" &&
                             lastToken.content != ")")
                        tokens.emplace_back(token.content, ParserToken_OperatorArithmeticUnaryPlus);
                    else if (token.content == "+")
                        tokens.emplace_back(token.content, ParserToken_OperatorArithmeticPlus);
                    else if ((token.content == "-" &&
                              lastToken.type != LexerToken_Number &&
                              lastToken.type != LexerToken_Identifier) &&
                              lastToken.content != "(" &&
                              lastToken.content != ")")
                        tokens.emplace_back(token.content, ParserToken_OperatorArithmeticUnaryMinus);
                    else if (token.content == "-")
                        tokens.emplace_back(token.content, ParserToken_OperatorArithmeticMinus);
                    else if (token.content == "*")
                        tokens.emplace_back(token.content, ParserToken_OperatorArithmeticMultiply);
                    else if (token.content == "/")
                        tokens.emplace_back(token.content, ParserToken_OperatorArithmeticDivide);
                    else if (token.content == "%")
                        tokens.emplace_back(token.content, ParserToken_OperatorArithmeticRemainder);
                    // Comparison Operators.
                    else if (token.content == "==")
                        tokens.emplace_back(token.content, ParserToken_OperatorComparisonEqual);
                    else if (token.content == "!=")
                        tokens.emplace_back(token.content, ParserToken_OperatorComparisonNotEqual);
                    else if (token.content == "<")
                        tokens.emplace_back(token.content, ParserToken_OperatorComparisonLessThan);
                    else if (token.content == ">")
                        tokens.emplace_back(token.content, ParserToken_OperatorComparisonGreaterThan);
                    else if (token.content == "<=")
                        tokens.emplace_back(token.content, ParserToken_OperatorComparisonLessThanOrEqual);
                    else if (token.content == ">=")
                        tokens.emplace_back(token.content, ParserToken_OperatorComparisonGreaterThanOrEqual);
                    // Logical Operators.
                    else if (token.content == "&&")
                        tokens.emplace_back(token.content, ParserToken_OperatorLogicalAnd);
                    else if (token.content == "||")
                        tokens.emplace_back(token.content, ParserToken_OperatorLogicalOr);
                    else if (token.content == "!")
                        tokens.emplace_back(token.content, ParserToken_OperatorLogicalNot);
                    else
                        tokens.emplace_back(token.content, ParserToken_Unknown);
                    break;
                case LexerToken_Separator:
                    // Separators
                    if (token.content == "(")
                        tokens.emplace_back(token.content, ParserToken_SeparatorRoundBracketOpen);
                    else if (token.content == ")")
                        tokens.emplace_back(token.content, ParserToken_SeparatorRoundBracketClosed);
                    else if (token.content == "[")
                        tokens.emplace_back(token.content, ParserToken_SeparatorSquareBracketOpen);
                    else if (token.content == "]")
                        tokens.emplace_back(token.content, ParserToken_SeparatorSquareBracketClosed);
                    else if (token.content == "{")
                        tokens.emplace_back(token.content, ParserToken_SeparatorCurlyBracketOpen);
                    else if (token.content == "}")
                        tokens.emplace_back(token.content, ParserToken_SeparatorCurlyBracketClosed);
                    else if (token.content == ";")
                        tokens.emplace_back(token.content, ParserToken_SeparatorSemicolon);
                    else if (token.content == ":")
                        tokens.emplace_back(token.content, ParserToken_SeparatorColon);
                    else if (token.content == ",")
                        tokens.emplace_back(token.content, ParserToken_SeparatorComma);
                    else
                        tokens.emplace_back(token.content, ParserToken_Unknown);
                    break;
                case LexerToken_Identifier:
                    // Boolean
                    if (token.content == "true" || token.content == "false")
                        tokens.emplace_back(token.content, ParserToken_LiteralValueBoolean);
                    // Keywords
                    else if (token.content == "func")
                        tokens.emplace_back(token.content, ParserToken_Keyword_TopLevel_Function);
                    else if (token.content == "var")
                        tokens.emplace_back(token.content, ParserToken_KeywordVar);
                    else if (token.content == "return")
                        tokens.emplace_back(token.content, ParserToken_KeywordReturn);
                    else if (token.content == "if")
                        tokens.emplace_back(token.content, ParserToken_KeywordIf);
                    else if (token.content == "else")
                        tokens.emplace_back(token.content, ParserToken_KeywordElse);
                    else
                        tokens.emplace_back(token.content, ParserToken_Identifier);
                    break;
                default:
                    tokens.emplace_back(token.content, ParserToken_Unknown);
                    break;
            }

            lastToken = token;
        }

        return tokens;
    }

    std::string Parser::getTokenName(ParserTokenEnum tokenType) {
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

    ParserToken Parser::getCurrentToken() {
        return m_currentToken;
    }

    ParserToken Parser::getNextToken() {
        m_currentToken = m_tokens[++m_currentTokenIndex];

        return m_currentToken;
    }

    ParserToken Parser::getPreviousToken() {
        if (m_currentTokenIndex > 0)
            m_currentToken = m_tokens[--m_currentTokenIndex];

        return m_currentToken;
    }
    
    int Parser::getOperatorPrecedence(const std::string &binaryOperator) {
        if(m_binaryOperatorPrecedence.find(binaryOperator) == m_binaryOperatorPrecedence.end())
            return -1;
        else
            return m_binaryOperatorPrecedence[binaryOperator] <= 0 ? -1 : m_binaryOperatorPrecedence[binaryOperator];
    }

    std::shared_ptr<NodeFunction> Parser::parseFunction() {
        std::string name;
        std::string type;
        std::vector<Argument> arguments;

        getNextToken();

        if (m_currentToken.type == ParserToken_Identifier)
            name = m_currentToken.content;
        else {
            LOG_ERROR("Expected an identifier.");

            return nullptr;
        }

        getNextToken();

        if (m_currentToken.type != ParserToken_SeparatorRoundBracketOpen) {
            LOG_ERROR("Expected an opened round bracket.");

            return nullptr;
        }

        while (true) {
            getNextToken();

            if (m_currentToken.type == ParserToken_SeparatorRoundBracketClosed)
                break;
            else if (m_currentToken.type == ParserToken_Identifier) {
                auto identifier = m_currentToken.content;

                getNextToken();
                PARSER_CHECKTOKEN(ParserToken_SeparatorColon);

                getNextToken();
                PARSER_CHECKTOKEN(ParserToken_Identifier);

                arguments.emplace_back(Argument(identifier, m_currentToken.content));
            }
            else if (m_currentToken.type == ParserToken_SeparatorComma) {
                continue;
            }
            else {
                LOG_ERROR("Expected either a closed round bracket or a variable.");

                return nullptr;
            }
        }

        getNextToken();

        PARSER_CHECKTOKEN(ParserToken_SeparatorColon);

        getNextToken();

        if (m_currentToken.type == ParserToken_Identifier)
            type = m_currentToken.content;
        else {
            LOG_ERROR("Expected a type name.");

            return nullptr;
        }

        getNextToken();

        PARSER_CHECKTOKEN(ParserToken_SeparatorCurlyBracketOpen);

        auto expressions = parseBody();

        return std::make_shared<NodeFunction>(name, type, arguments, expressions);
    }

    std::shared_ptr<Node> Parser::parsePrimary() {
        switch (m_currentToken.type) {
            case ParserToken_LiteralValueInteger:
                return parseInteger();
            case ParserToken_LiteralValueDecimal:
                return parseDouble();
            case ParserToken_SeparatorRoundBracketOpen:
                return parseParenthese();
            case ParserToken_KeywordReturn:
                return parseReturn();
            case ParserToken_KeywordIf:
                return parseIf();
            case ParserToken_Identifier:
                return parseIdentifier();
            case ParserToken_SeparatorCurlyBracketOpen:
                return parseBlock();
            case ParserToken_KeywordVar:
                return parseVariableDeclaration();

            default:
                return nullptr;
        }
    }

    std::shared_ptr<Node> Parser::parseExpression() {
        auto leftExpression = parsePrimary();

        if (!leftExpression)
            return nullptr;

        return parseBinaryOperation(0, leftExpression);
    }

    std::shared_ptr<Node> Parser::parseBinaryOperation(int expressionPrecedence, std::shared_ptr<Node> leftExpression) {
        while (true) {
            int tokenPrecedence = getOperatorPrecedence(m_currentToken.content);

            if (tokenPrecedence < expressionPrecedence)
                return leftExpression;

            auto operationType = m_currentToken.type;

            getNextToken();

            auto rightExpression = parsePrimary();

            if (!rightExpression)
                return nullptr;

            int nextPrecedence = getOperatorPrecedence(m_currentToken.content);

            if (tokenPrecedence < nextPrecedence) {
                rightExpression = parseBinaryOperation(tokenPrecedence + 1, std::move(rightExpression));

                if (!rightExpression)
                    return nullptr;
            }

            leftExpression = std::make_shared<NodeOperationBinary>(operationType, std::move(leftExpression), rightExpression);
        }
    }

    std::vector<std::shared_ptr<Node>> Parser::parseBody() {
        std::vector<std::shared_ptr<Node>> expressions;

        while (true) {
            getNextToken();

            if (m_currentToken.type == ParserToken_SeparatorCurlyBracketClosed)
                break;

            auto expression = parseExpression();

            if (expression)
                expressions.emplace_back(expression);
        }

        return expressions;
    }

    std::shared_ptr<Node> Parser::parseInteger() {
        auto result = std::make_shared<NodeConstantInteger>(std::stoi(m_currentToken.content));

        getNextToken();

        return result;
    }

    std::shared_ptr<Node> Parser::parseDouble() {
        auto result = std::make_shared<NodeConstantDouble>(std::stod(m_currentToken.content));

        getNextToken();

        return result;
    }

    std::shared_ptr<Node> Parser::parseIdentifier() {
        auto identifier = m_currentToken.content;

        getNextToken();

        if (m_currentToken.type == ParserToken_SeparatorRoundBracketOpen) {
            getNextToken();

            std::vector<std::shared_ptr<Node>> arguments;

            if (m_currentToken.type != ParserToken_SeparatorRoundBracketClosed) {
                while (true) {
                    if (auto arg = parseExpression())
                        arguments.emplace_back(arg);
                    else
                        return nullptr;

                    if (m_currentToken.type == ParserToken_SeparatorRoundBracketClosed)
                        break;

                    if (m_currentToken.type != ParserToken_SeparatorComma)
                    {
                        LOG_ERROR("Expected ')' or ',' in argument list");
                    }

                    getNextToken();
                }
            }

            getNextToken();

            return std::make_shared<NodeStatementFunctionCall>(identifier, arguments);
        }
        else {
            return std::make_shared<NodeVariable>(identifier);
        }
    }

    std::shared_ptr<Node> Parser::parseParenthese() {
        getNextToken();

        auto expression = parseExpression();

        getNextToken();

        return expression;
    }

    std::shared_ptr<Node> Parser::parseReturn() {
        getNextToken();

        return std::make_shared<NodeStatementReturn>(parseExpression());
    }

    std::shared_ptr<Node> Parser::parseIf() {
        getNextToken();

        auto condition = parseParenthese();

        if (!condition) {
            LOG_ERROR("Cannot parse if condition");

            return nullptr;
        }

        PARSER_CHECKTOKEN(ParserToken_SeparatorCurlyBracketOpen);

        auto ifBody = parseBody();

        PARSER_CHECKTOKEN(ParserToken_SeparatorCurlyBracketClosed);

        std::vector<std::shared_ptr<Node>> elseBody;

        getNextToken();

        if (m_currentToken.type == ParserToken_KeywordElse) {
            getNextToken();

            if (m_currentToken.type == ParserToken_KeywordIf) {
                elseBody.emplace_back(parseIf());

                PARSER_CHECKTOKEN(ParserToken_SeparatorCurlyBracketClosed);
            }
            else {
                PARSER_CHECKTOKEN(ParserToken_SeparatorCurlyBracketOpen);

                elseBody = parseBody();

                PARSER_CHECKTOKEN(ParserToken_SeparatorCurlyBracketClosed);
            }
        }
        else {
            getPreviousToken();
        }

        return std::make_shared<NodeStatementIf>(condition, ifBody, elseBody);
    }

    std::shared_ptr<Node> Parser::parseBlock() {
        auto block = std::make_shared<NodeStatementBlock>();

        block->body = parseBody();

        return block;
    }

    // TODO: PARSER: Define a variable with no default value (eg: var x: int;).
    // TODO: PARSER: Define multiples variables at once (eg: var x: int = 0, y: int, z: int = 2, w: int = z;).
    std::shared_ptr<Node> Parser::parseVariableDeclaration() {
        std::vector<std::shared_ptr<Variable>> variables;

        while (true) {
            PARSER_CHECKNEXTTOKEN(ParserToken_Identifier);

            auto identifier = m_currentToken.content;

            PARSER_CHECKNEXTTOKEN(ParserToken_SeparatorColon);
            PARSER_CHECKNEXTTOKEN(ParserToken_Identifier);

            auto type = m_currentToken.content;

            getNextToken();

            std::shared_ptr<Node> value = nullptr;

            if (m_currentToken.type == ParserToken_OperatorEqual) {
                getNextToken();

                value = parseExpression();
            }

            variables.emplace_back(std::make_shared<Variable>(identifier, type, value));

            if (m_currentToken.type != ParserToken_SeparatorComma)
                break;
        }

        return std::make_shared<NodeStatementVariableDeclaration>(variables);
    }
    
    Parser &getParser() {
        return Parser::getInstance();
    }
    
} /* Namespace Aryiele. */
