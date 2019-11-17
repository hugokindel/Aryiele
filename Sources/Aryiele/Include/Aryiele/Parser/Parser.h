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

#ifndef ARYIELE_PARSER_PARSER_H
#define ARYIELE_PARSER_PARSER_H

#include <vector>
#include <Vanir/Module/Module.h>
#include <Aryiele/Common.h>
#include <Aryiele/Lexer/LexerToken.h>
#include <Aryiele/AST/Nodes/Node.h>
#include <Aryiele/AST/Nodes/NodeFunction.h>
#include <Aryiele/Parser/ParserToken.h>
#include <Aryiele/Parser/ParserInformation.h>

namespace Aryiele {
class Parser : public Vanir::Module<Parser> {
    public:
        Parser();
    
    std::vector<std::shared_ptr<Node>> parse(std::vector<ParserToken> tokens);
        static std::vector<ParserToken> convertTokens(const std::vector<LexerToken>& tokenizerTokens);
        static std::string getTokenName(ParserTokenEnum tokenType);
        ParserToken getCurrentToken();

    private:
        ParserToken getNextToken(bool incrementCounter = true);
        int getOperatorPrecedence(ParserTokenEnum binaryOperator);
        std::shared_ptr<NodeFunction> parseFunction();
        std::shared_ptr<Node> parsePrimary();
        std::shared_ptr<Node> parseExpression();
        std::shared_ptr<Node> parseBinaryOperation(int expressionPrecedence, std::shared_ptr<Node> leftExpression);
        std::shared_ptr<Node> parseUnaryOperation();
        std::vector<std::shared_ptr<Node>> parseBody();
        std::shared_ptr<Node> parseInteger();
        std::shared_ptr<Node> parseDouble();
        std::shared_ptr<Node> parseString();
        std::shared_ptr<Node> parseCharacter();
        std::shared_ptr<Node> parseBoolean();
        std::shared_ptr<Node> parseIdentifier();
        std::shared_ptr<Node> parseParenthese();
        std::shared_ptr<Node> parseReturn();
        std::shared_ptr<Node> parseIf();
        std::shared_ptr<Node> parseFor();
        std::shared_ptr<Node> parseWhile(bool doOnce);
        std::shared_ptr<Node> parseBlock();
        std::shared_ptr<Node> parseVariableDeclaration(bool passVar = true, bool multiple = true);
        bool isLiteral(ParserTokenEnum type);
        bool isLiteralOrIdentifier(ParserTokenEnum type);
    
        std::map<ParserTokenEnum, int> m_binaryOperatorPrecedence;
        std::vector<std::shared_ptr<Node>> m_nodes;
        std::vector<ParserToken> m_tokens;
        ParserToken m_currentToken;
        int m_currentTokenIndex = -1;
        int m_currentLine = 1;
        int m_currentColumn = 1;
    };
    
    Parser &getParser();

} /* Namespace Aryiele. */

#define PARSER_ERROR(...) { \
    LOG_ERROR(m_currentLine, ":", m_currentColumn, " ", __VA_ARGS__) \
    return nullptr; \
}

#define PARSER_CHECKTOKEN(EXPECTEDTOKENTYPE) \
if (getParser().getCurrentToken().type != EXPECTEDTOKENTYPE) { \
    PARSER_ERROR("wrong token, got '", Parser::getTokenName(m_currentToken.type), "' but expected '", Parser::getTokenName(EXPECTEDTOKENTYPE), "'"); \
}

#define PARSER_CHECKNEXTTOKEN(EXPECTEDTOKENTYPE) { \
    getNextToken(); \
    PARSER_CHECKTOKEN(EXPECTEDTOKENTYPE); \
}

#endif /* ARYIELE_PARSER_PARSER_H. */
