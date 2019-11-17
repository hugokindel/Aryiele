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
#include <Aryiele/AST/Nodes/NodeConstantString.h>
#include <Aryiele/AST/Nodes/NodeConstantCharacter.h>
#include <Aryiele/AST/Nodes/NodeConstantBoolean.h>
#include <Aryiele/AST/Nodes/NodeConstantInteger.h>
#include <Aryiele/AST/Nodes/NodeOperationBinary.h>
#include <Aryiele/AST/Nodes/NodeStatementBlock.h>
#include <Aryiele/AST/Nodes/NodeStatementFunctionCall.h>
#include <Aryiele/AST/Nodes/NodeStatementIf.h>
#include <Aryiele/AST/Nodes/NodeStatementReturn.h>
#include <Aryiele/AST/Nodes/NodeStatementVariableDeclaration.h>
#include <Aryiele/AST/Nodes/NodeStatementWhile.h>
#include <Aryiele/AST/Nodes/NodeStatementFor.h>
#include <Aryiele/AST/Nodes/NodeOperationUnary.h>
#include <Aryiele/AST/Nodes/NodeVariable.h>

namespace Aryiele {
    Parser::Parser() {
        m_binaryOperatorPrecedence[ParserToken_OperatorEqual] = 10;
        m_binaryOperatorPrecedence[ParserToken_OperatorArithmeticPlusEqual] = 10;
        m_binaryOperatorPrecedence[ParserToken_OperatorArithmeticMinusEqual] = 10;
        m_binaryOperatorPrecedence[ParserToken_OperatorArithmeticMultiplyEqual] = 10;
        m_binaryOperatorPrecedence[ParserToken_OperatorArithmeticDivideEqual] = 10;
        m_binaryOperatorPrecedence[ParserToken_OperatorArithmeticRemainderEqual] = 10;
        m_binaryOperatorPrecedence[ParserToken_OperatorLogicalAnd] = 20;
        m_binaryOperatorPrecedence[ParserToken_OperatorLogicalOr] = 20;
        m_binaryOperatorPrecedence[ParserToken_OperatorComparisonLessThan] = 30;
        m_binaryOperatorPrecedence[ParserToken_OperatorComparisonGreaterThan] = 30;
        m_binaryOperatorPrecedence[ParserToken_OperatorComparisonLessThanOrEqual] = 30;
        m_binaryOperatorPrecedence[ParserToken_OperatorComparisonGreaterThanOrEqual] = 30;
        m_binaryOperatorPrecedence[ParserToken_OperatorComparisonEqual] = 30;
        m_binaryOperatorPrecedence[ParserToken_OperatorComparisonNotEqual] = 30;
        m_binaryOperatorPrecedence[ParserToken_OperatorArithmeticPlus] = 40;
        m_binaryOperatorPrecedence[ParserToken_OperatorArithmeticMinus] = 40;
        m_binaryOperatorPrecedence[ParserToken_OperatorArithmeticRemainder] = 50;
        m_binaryOperatorPrecedence[ParserToken_OperatorArithmeticMultiply] = 50;
        m_binaryOperatorPrecedence[ParserToken_OperatorArithmeticDivide] = 50;
    }
    
    std::vector<std::shared_ptr<Node>> Parser::parse(std::vector<ParserToken> tokens) {
        m_tokens = std::move(tokens);
        m_tokens.emplace_back("", ParserToken_EOF);
        
        while (true) {
            getNextToken();
            
            if (m_currentToken.type == ParserToken_EOF)
                break;
            if (m_currentToken.type == ParserToken_KeywordFunction)
                m_nodes.emplace_back(parseFunction());
        }
        
        return m_nodes;
    }
    
    std::vector<ParserToken> Parser::convertTokens(const std::vector<LexerToken>& LexerTokens) {
        auto tokens = std::vector<ParserToken>();
        auto lastToken = LexerToken(std::string(), LexerToken_Unknown);

        for (auto& token : LexerTokens) {
            switch (token.type) {
                case LexerToken_Number:
                    if (token.content.find('.') != std::string::npos)
                        tokens.emplace_back(token.content, ParserToken_LiteralValueDecimal);
                    else
                        tokens.emplace_back(token.content, ParserToken_LiteralValueInteger);
                    break;
                case LexerToken_String:
                    tokens.emplace_back(token.content, ParserToken_LiteralValueString);
                    break;
                case LexerToken_Character:
                    tokens.emplace_back(token.content, ParserToken_LiteralValueCharacter);
                    break;
                case LexerToken_Operator:
                    // Operators.
                    if (token.content == "=")
                        tokens.emplace_back("", ParserToken_OperatorEqual);
                    // Arithmetic Operators.
                    else if (token.content == "+=")
                        tokens.emplace_back("", ParserToken_OperatorArithmeticPlusEqual);
                    else if (token.content == "-=")
                        tokens.emplace_back("", ParserToken_OperatorArithmeticMinusEqual);
                    else if (token.content == "*=")
                        tokens.emplace_back("", ParserToken_OperatorArithmeticMultiplyEqual);
                    else if (token.content == "/=")
                        tokens.emplace_back("", ParserToken_OperatorArithmeticDivideEqual);
                    else if (token.content == "%=")
                        tokens.emplace_back("", ParserToken_OperatorArithmeticRemainderEqual);
                    else if ((token.content == "+" &&
                             lastToken.type != LexerToken_Number &&
                             lastToken.type != LexerToken_Identifier) &&
                             lastToken.content != ")" &&
                             lastToken.content != "++" &&
                             lastToken.content != "--")
                        tokens.emplace_back("", ParserToken_OperatorUnaryArithmeticPlus);
                    else if (token.content == "+")
                        tokens.emplace_back("", ParserToken_OperatorArithmeticPlus);
                    else if ((token.content == "-" &&
                              lastToken.type != LexerToken_Number &&
                              lastToken.type != LexerToken_Identifier) &&
                              lastToken.content != ")" &&
                              lastToken.content != "++" &&
                              lastToken.content != "--")
                        tokens.emplace_back("", ParserToken_OperatorUnaryArithmeticMinus);
                    else if (token.content == "-")
                        tokens.emplace_back("", ParserToken_OperatorArithmeticMinus);
                    else if (token.content == "*")
                        tokens.emplace_back("", ParserToken_OperatorArithmeticMultiply);
                    else if (token.content == "/")
                        tokens.emplace_back("", ParserToken_OperatorArithmeticDivide);
                    else if (token.content == "%")
                        tokens.emplace_back("", ParserToken_OperatorArithmeticRemainder);
                    else if (token.content == "==")
                        tokens.emplace_back("", ParserToken_OperatorComparisonEqual);
                    else if (token.content == "!=")
                        tokens.emplace_back("", ParserToken_OperatorComparisonNotEqual);
                    else if (token.content == "<")
                        tokens.emplace_back("", ParserToken_OperatorComparisonLessThan);
                    else if (token.content == ">")
                        tokens.emplace_back("", ParserToken_OperatorComparisonGreaterThan);
                    else if (token.content == "<=")
                        tokens.emplace_back("", ParserToken_OperatorComparisonLessThanOrEqual);
                    else if (token.content == ">=")
                        tokens.emplace_back("", ParserToken_OperatorComparisonGreaterThanOrEqual);
                    else if (token.content == "&&")
                        tokens.emplace_back("", ParserToken_OperatorLogicalAnd);
                    else if (token.content == "||")
                        tokens.emplace_back("", ParserToken_OperatorLogicalOr);
                    else if (token.content == "!")
                        tokens.emplace_back("", ParserToken_OperatorUnaryLogicalNot);
                    else if (token.content == "++")
                        tokens.emplace_back("", ParserToken_OperatorUnaryArithmeticIncrement);
                    else if (token.content == "--")
                        tokens.emplace_back("", ParserToken_OperatorUnaryArithmeticDecrement);
                    break;
                case LexerToken_Separator:
                    // Separators
                    if (token.content == "(")
                        tokens.emplace_back("", ParserToken_SeparatorRoundBracketOpen);
                    else if (token.content == ")")
                        tokens.emplace_back("", ParserToken_SeparatorRoundBracketClosed);
                    else if (token.content == "[")
                        tokens.emplace_back("", ParserToken_SeparatorSquareBracketOpen);
                    else if (token.content == "]")
                        tokens.emplace_back("", ParserToken_SeparatorSquareBracketClosed);
                    else if (token.content == "{")
                        tokens.emplace_back("", ParserToken_SeparatorCurlyBracketOpen);
                    else if (token.content == "}")
                        tokens.emplace_back("", ParserToken_SeparatorCurlyBracketClosed);
                    else if (token.content == ";")
                        tokens.emplace_back("", ParserToken_SeparatorSemicolon);
                    else if (token.content == ":")
                        tokens.emplace_back("", ParserToken_SeparatorColon);
                    else if (token.content == ",")
                        tokens.emplace_back("", ParserToken_SeparatorComma);
                    else if (token.content == ".")
                        tokens.emplace_back("", ParserToken_SeparatorDot);
                    else if (token.content == "...")
                        tokens.emplace_back("", ParserToken_SeparatorTripleDot);
                    else if (token.content == "?")
                        tokens.emplace_back("", ParserToken_SeparatorQuestionMark);
                    break;
                case LexerToken_Identifier:
                    // Boolean
                    if (token.content == "true" || token.content == "false")
                        tokens.emplace_back(token.content, ParserToken_LiteralValueBoolean);
                    // Keywords
                    else if (token.content == "func")
                        tokens.emplace_back("", ParserToken_KeywordFunction);
                    else if (token.content == "var")
                        tokens.emplace_back("", ParserToken_KeywordVar);
                    else if (token.content == "let")
                        tokens.emplace_back("", ParserToken_KeywordLet);
                    else if (token.content == "for")
                        tokens.emplace_back("", ParserToken_KeywordFor);
                    else if (token.content == "until")
                        tokens.emplace_back("", ParserToken_KeywordUntil);
                    else if (token.content == "by")
                        tokens.emplace_back("", ParserToken_KeywordBy);
                    else if (token.content == "do")
                        tokens.emplace_back("", ParserToken_KeywordDo);
                    else if (token.content == "while")
                        tokens.emplace_back("", ParserToken_KeywordWhile);
                    else if (token.content == "switch")
                        tokens.emplace_back("", ParserToken_KeywordSwitch);
                    else if (token.content == "case")
                        tokens.emplace_back("", ParserToken_KeywordCase);
                    else if (token.content == "return")
                        tokens.emplace_back("", ParserToken_KeywordReturn);
                    else if (token.content == "if")
                        tokens.emplace_back("", ParserToken_KeywordIf);
                    else if (token.content == "else")
                        tokens.emplace_back("", ParserToken_KeywordElse);
                    else
                        tokens.emplace_back(token.content, ParserToken_Identifier);
                    break;
                case LexerToken_Space:
                    tokens.emplace_back("", ParserToken_Space);
                    break;
                case LexerToken_Newline:
                    tokens.emplace_back("", ParserToken_Newline);
                    break;
                default:
                    tokens.emplace_back(token.content, ParserToken_Unknown);
                    break;
            }

            if (token.type != LexerToken_Space && token.type != LexerToken_Newline) {
                lastToken = token;
            }
        }
    
        tokens.emplace_back("", ParserToken_EOF);

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
            case ParserToken_SeparatorQuestionMark:
                return "SeparatorQuestionMark";
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
            case ParserToken_Identifier:
                return "Identifier";
            case ParserToken_LiteralValueCharacter:
                return "LiteralValueCharacter";
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
            case ParserToken_KeywordLet:
                return "KeywordLet";
            case ParserToken_KeywordFor:
                return "KeywordFor";
            case ParserToken_KeywordUntil:
                return "KeywordUntil";
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
            case ParserToken_Space:
                return "Space";
            case ParserToken_Newline:
                return "Newline";
            case ParserToken_EOF:
                return "EOF";
            case ParserToken_Unknown:
            default:
                return "Unknown";
        }
    }

    ParserToken Parser::getCurrentToken() {
        return m_currentToken;
    }

    ParserToken Parser::getNextToken(bool incrementCounter) {
        if (incrementCounter) {
            m_currentToken = m_tokens[++m_currentTokenIndex];
    
            if (m_currentToken.type == ParserToken_Newline) {
                m_currentLine++;
                m_currentColumn = 0;
            } else {
                if (m_currentToken.content.empty()) {
                    m_currentColumn++;
                } else {
                    m_currentColumn += m_currentToken.content.size();
                }
            }
    
            while (m_currentToken.type == ParserToken_Space) {
                m_currentToken = m_tokens[++m_currentTokenIndex];
        
                if (m_currentToken.type == ParserToken_Newline) {
                    m_currentLine++;
                    m_currentColumn = 0;
                } else {
                    if (m_currentToken.content.empty()) {
                        m_currentColumn++;
                    } else {
                        m_currentColumn += m_currentToken.content.size();
                    }
                }
            }
    
            return m_currentToken;
        } else {
            auto counter = m_currentTokenIndex;
            auto currentToken = m_tokens[++counter];
    
            while (currentToken.type == ParserToken_Space) {
                currentToken = m_tokens[++counter];
            }
    
            return currentToken;
        }
    }
    
    int Parser::getOperatorPrecedence(ParserTokenEnum binaryOperator) {
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
        
        if (m_currentToken.type == ParserToken_Identifier) {
            name = m_currentToken.content;
        } else {
            PARSER_ERROR("Expected an identifier.")
        }

        getNextToken();

        if (m_currentToken.type != ParserToken_SeparatorRoundBracketOpen) {
            PARSER_ERROR("Expected an opened round bracket.");
        }

        while (true) {
            getNextToken();

            if (m_currentToken.type == ParserToken_SeparatorRoundBracketClosed)
                break;
            else if (m_currentToken.type == ParserToken_Identifier) {
                auto identifier = m_currentToken.content;

                getNextToken();
                PARSER_CHECKTOKEN(ParserToken_SeparatorColon)

                getNextToken();
                PARSER_CHECKTOKEN(ParserToken_Identifier)

                arguments.emplace_back(Argument(identifier, m_currentToken.content));
            }
            else if (m_currentToken.type == ParserToken_SeparatorComma) {
                continue;
            }
            else {
                PARSER_ERROR("expected either a closed round bracket or a variable")
            }
        }

        getNextToken();
        
        if (m_currentToken.type == ParserToken_SeparatorColon) {
            PARSER_CHECKTOKEN(ParserToken_SeparatorColon)
    
            getNextToken();
    
            if (m_currentToken.type == ParserToken_Identifier)
                type = m_currentToken.content;
            else {
                PARSER_ERROR("Expected a type name")
            }
    
            getNextToken();
    
            PARSER_CHECKTOKEN(ParserToken_SeparatorCurlyBracketOpen)
        } else if (m_currentToken.type == ParserToken_SeparatorCurlyBracketOpen) {
            type = "Void";
        } else {
            PARSER_ERROR("expected either identifier or opened curly bracket")
        }
        auto expressions = parseBody();

        return std::make_shared<NodeFunction>(name, type, arguments, expressions);
    }

    std::shared_ptr<Node> Parser::parsePrimary() {
        switch (m_currentToken.type) {
            case ParserToken_LiteralValueInteger:
                return parseInteger();
            case ParserToken_LiteralValueDecimal:
                return parseDouble();
            case ParserToken_LiteralValueString:
                return parseString();
            case ParserToken_LiteralValueCharacter:
                return parseCharacter();
            case ParserToken_LiteralValueBoolean:
                return parseBoolean();
            case ParserToken_SeparatorRoundBracketOpen:
                return parseParenthese();
            case ParserToken_KeywordReturn:
                return parseReturn();
            case ParserToken_KeywordIf:
                return parseIf();
            case ParserToken_KeywordDo:
                return parseWhile(true);
            case ParserToken_KeywordWhile:
                return parseWhile(false);
            case ParserToken_KeywordFor:
                return parseFor();
            case ParserToken_Identifier:
                return parseIdentifier();
            case ParserToken_SeparatorCurlyBracketOpen:
                return parseBlock();
            case ParserToken_KeywordVar:
                return parseVariableDeclaration();
            case ParserToken_OperatorUnaryArithmeticMinus:
            case ParserToken_OperatorUnaryArithmeticPlus:
            case ParserToken_OperatorUnaryArithmeticIncrement:
            case ParserToken_OperatorUnaryArithmeticDecrement:
            case ParserToken_OperatorUnaryLogicalNot:
                return parseUnaryOperation();
            default:
                return nullptr;
        }
    }

    std::shared_ptr<Node> Parser::parseExpression() {
        auto leftExpression = parsePrimary();
    
        if (!leftExpression)
            return nullptr;
    
        if (m_currentToken.type == ParserToken_OperatorUnaryArithmeticIncrement ||
            m_currentToken.type == ParserToken_OperatorUnaryArithmeticDecrement) {
            auto unaryExpression = std::make_shared<NodeOperationUnary>(m_currentToken.type, leftExpression, false);
            getNextToken();
            return parseBinaryOperation(0, unaryExpression);
        }
        
        return parseBinaryOperation(0, leftExpression);
    }

    std::shared_ptr<Node> Parser::parseBinaryOperation(int expressionPrecedence, std::shared_ptr<Node> leftExpression) {
        while (true) {
            int tokenPrecedence = getOperatorPrecedence(m_currentToken.type);
    
            if (tokenPrecedence < expressionPrecedence)
                return leftExpression;
    
            auto operationType = m_currentToken.type;
            
            getNextToken();
    
            auto rightExpression = parsePrimary();

            if (!rightExpression)
                return nullptr;

            int nextPrecedence = getOperatorPrecedence(m_currentToken.type);

            if (tokenPrecedence < nextPrecedence) {
                rightExpression = parseBinaryOperation(tokenPrecedence + 1, std::move(rightExpression));

                if (!rightExpression)
                    return nullptr;
            }

            leftExpression = std::make_shared<NodeOperationBinary>(operationType, std::move(leftExpression), rightExpression);
        }
    }
    
    std::shared_ptr<Node> Parser::parseUnaryOperation() {
        if (m_currentToken.type != ParserToken_OperatorUnaryArithmeticPlus &&
            m_currentToken.type != ParserToken_OperatorUnaryArithmeticMinus &&
            m_currentToken.type != ParserToken_OperatorUnaryArithmeticIncrement &&
            m_currentToken.type != ParserToken_OperatorUnaryArithmeticDecrement) {
            return parsePrimary();
        }
        
        auto type = m_currentToken.type;
        
        getNextToken();
        
        auto operand = parseUnaryOperation();
        
        return std::make_shared<NodeOperationUnary>(type, operand);
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
    
    std::shared_ptr<Node> Parser::parseString() {
        auto result = std::make_shared<NodeConstantString>(m_currentToken.content);
        
        getNextToken();
        
        return result;
    }
    
    std::shared_ptr<Node> Parser::parseCharacter() {
        auto result = std::make_shared<NodeConstantCharacter>(m_currentToken.content);
        
        getNextToken();
        
        return result;
    }
    
    std::shared_ptr<Node> Parser::parseBoolean() {
        auto result = std::make_shared<NodeConstantDouble>(m_currentToken.content == "true");
        
        getNextToken();
        
        return result;
    }

    std::shared_ptr<Node> Parser::parseIdentifier() {
        auto identifier = m_currentToken.content;
        
        getNextToken();
        
        if (m_currentToken.type == ParserToken_SeparatorDot) {
            getNextToken();
            
            auto result = parseIdentifier();
            auto resultC = std::dynamic_pointer_cast<NodeStatementFunctionCall>(result);
            
            resultC->decorations.insert(resultC->decorations.begin(), identifier);
            
            return resultC;
        } else if (m_currentToken.type == ParserToken_SeparatorRoundBracketOpen) {
            getNextToken();

            std::vector<std::shared_ptr<Node>> arguments;

            if (m_currentToken.type != ParserToken_SeparatorRoundBracketClosed) {
                while (true) {
                    if (auto arg = parseExpression()) {
                        arguments.emplace_back(arg);
                    } else {
                        return nullptr;
                    }

                    if (m_currentToken.type == ParserToken_SeparatorRoundBracketClosed) {
                        break;
                    }

                    if (m_currentToken.type != ParserToken_SeparatorComma) {
                        PARSER_ERROR("Expected ')' or ',' in argument list")
                    }

                    getNextToken();
                }
            }

            getNextToken();
            
            return std::make_shared<NodeStatementFunctionCall>(identifier, std::vector<std::string>(), arguments);
        }
        
        return std::make_shared<NodeVariable>(identifier);
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
        
        if (!isLiteralOrIdentifier(m_currentToken.type)) {
            PARSER_ERROR("expected literal value or variable identifier")
        }
        
        auto condition = parseExpression();

        if (!condition) {
            PARSER_ERROR("cannot parse if condition")
        }

        PARSER_CHECKTOKEN(ParserToken_SeparatorCurlyBracketOpen)

        auto ifBody = parseBody();

        PARSER_CHECKTOKEN(ParserToken_SeparatorCurlyBracketClosed)

        std::vector<std::shared_ptr<Node>> elseBody;
        
        if (getNextToken(false).type == ParserToken_KeywordElse) {
            getNextToken();
            getNextToken();
    
            if (m_currentToken.type == ParserToken_KeywordIf) {
                elseBody.emplace_back(parseIf());

                PARSER_CHECKTOKEN(ParserToken_SeparatorCurlyBracketClosed)
            }
            else {
                PARSER_CHECKTOKEN(ParserToken_SeparatorCurlyBracketOpen)

                elseBody = parseBody();

                PARSER_CHECKTOKEN(ParserToken_SeparatorCurlyBracketClosed)
            }
        }

        return std::make_shared<NodeStatementIf>(condition, ifBody, elseBody);
    }
    
    std::shared_ptr<Node> Parser::parseFor() {
        std::shared_ptr<Node> incrementationValue;
        
        getNextToken();
    
        if (m_currentToken.type != ParserToken_Identifier) {
            PARSER_ERROR("expected variable declaration in for declaration")
        }
        
        auto variable = parseVariableDeclaration(false, false);
    
        if (m_currentToken.type != ParserToken_KeywordUntil) {
            PARSER_ERROR("expected 'until' in for declaration")
        }
        
        getNextToken();
    
        if (!isLiteralOrIdentifier(m_currentToken.type)) {
            PARSER_ERROR("expected literal value or identifier in for declaration")
        }
        
        auto condition = parseExpression();
        
        if (m_currentToken.type == ParserToken_KeywordBy) {
            getNextToken();
    
            if (!isLiteralOrIdentifier(m_currentToken.type)) {
                PARSER_ERROR("expected literal value or identifier in by in for declaration")
            }
    
            incrementationValue = parseExpression();
        }
        
        if (m_currentToken.type != ParserToken_SeparatorCurlyBracketOpen) {
            PARSER_ERROR("expected '{' in for declaration")
        }
    
        auto body = parseBody();
    
        PARSER_CHECKTOKEN(ParserToken_SeparatorCurlyBracketClosed)
        
        return std::make_shared<NodeStatementFor>(std::dynamic_pointer_cast<NodeStatementVariableDeclaration>(variable),
            condition, incrementationValue, body);
    }
    
    std::shared_ptr<Node> Parser::parseWhile(bool doOnce) {
        std::vector<std::shared_ptr<Node>> body;
        std::shared_ptr<Node> condition;
    
        getNextToken();
        
        if (doOnce) {
            PARSER_CHECKTOKEN(ParserToken_SeparatorCurlyBracketOpen)
            
            getNextToken();
            
            body = parseBody();
    
            PARSER_CHECKTOKEN(ParserToken_SeparatorCurlyBracketClosed)
            
            getNextToken();
            
            PARSER_CHECKTOKEN(ParserToken_KeywordWhile)
            
            getNextToken();
    
            condition = parseExpression();
        } else {
            condition = parseExpression();
    
            PARSER_CHECKTOKEN(ParserToken_SeparatorCurlyBracketOpen)
            
            getNextToken();
    
            body = parseBody();
    
            PARSER_CHECKTOKEN(ParserToken_SeparatorCurlyBracketClosed)
        }
        
        return std::make_shared<NodeStatementWhile>(doOnce, condition, body);
    }
    
    std::shared_ptr<Node> Parser::parseBlock() {
        return std::make_shared<NodeStatementBlock>(parseBody());
    }

    std::shared_ptr<Node> Parser::parseVariableDeclaration(bool passVar, bool multiple) {
        std::vector<std::shared_ptr<Variable>> variables;

        while (true) {
            if (passVar) {
                getNextToken();
            }
            
            PARSER_CHECKTOKEN(ParserToken_Identifier)

            auto identifier = m_currentToken.content;
            auto type = std::string();
            std::shared_ptr<Node> value = nullptr;
            
            getNextToken();
            
            if (m_currentToken.type == ParserToken_SeparatorColon) {
                getNextToken();
                PARSER_CHECKTOKEN(ParserToken_Identifier)
    
                type = m_currentToken.content;
    
                getNextToken();
            }

            if (m_currentToken.type == ParserToken_OperatorEqual) {
                getNextToken();
                
                value = parseExpression();
            } else if (type.empty() && value == nullptr) {
                PARSER_ERROR("variable declaration has empty type and value")
            }
    
            if (!type.empty() || value != nullptr) {
                variables.emplace_back(std::make_shared<Variable>(identifier, type, value));
            }
            
            if (!multiple || m_currentToken.type != ParserToken_SeparatorComma)
                break;
        }

        return std::make_shared<NodeStatementVariableDeclaration>(variables);
    }
    
    bool Parser::isLiteral(ParserTokenEnum type) {
        return m_currentToken.type == ParserToken_LiteralValueInteger ||
               m_currentToken.type == ParserToken_LiteralValueDecimal ||
               m_currentToken.type == ParserToken_LiteralValueString ||
               m_currentToken.type == ParserToken_LiteralValueCharacter ||
               m_currentToken.type == ParserToken_LiteralValueBoolean;
    }
    
    bool Parser::isLiteralOrIdentifier(ParserTokenEnum type) {
        return m_currentToken.type == ParserToken_Identifier || isLiteral(type);
    }
    
    Parser &getParser() {
        return Parser::getInstance();
    }
    
} /* Namespace Aryiele. */
