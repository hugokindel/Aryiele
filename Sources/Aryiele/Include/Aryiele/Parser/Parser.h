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
#include <Aryiele/Core/Includes.h>
#include <Aryiele/Lexer/LexerToken.h>
#include <Aryiele/AST/Nodes/Node.h>
#include <Aryiele/AST/Nodes/NodeFunction.h>
#include <Aryiele/Parser/ParserToken.h>
#include <Aryiele/Parser/ParserInformation.h>

namespace Aryiele {
class Parser : public Vanir::Module<Parser> {
    public:
        Parser();
        
        std::vector<ParserToken> ConvertTokens(std::vector<LexerToken> tokenizerTokens);
        static std::string GetTokenName(ParserTokens tokenType);
        std::vector<std::shared_ptr<Node>> Parse(std::vector<ParserToken> tokens);
        ParserToken GetCurrentToken();

    private:
        ParserToken GetNextToken();
        ParserToken GetPreviousToken();
    int GetOperatorPrecedence(const std::string& binaryOperator);
        std::shared_ptr<NodeFunction> ParseFunction();
        std::shared_ptr<Node> ParsePrimary();
        std::shared_ptr<Node> ParseExpression();
        std::shared_ptr<Node> ParseBinaryOperation(int expressionPrecedence, std::shared_ptr<Node> leftExpression);
        std::vector<std::shared_ptr<Node>> ParseBody();
        std::shared_ptr<Node> ParseInteger();
        std::shared_ptr<Node> ParseDouble();
        std::shared_ptr<Node> ParseIdentifier();
        std::shared_ptr<Node> ParseParenthese();
        std::shared_ptr<Node> ParseReturn();
        std::shared_ptr<Node> ParseIf();
        std::shared_ptr<Node> ParseBlock();
        std::shared_ptr<Node> ParseVariableDeclaration();
    
        std::map<std::string, int> m_binaryOperatorPrecedence;
        std::vector<std::shared_ptr<Node>> m_nodes;
        std::vector<ParserToken> m_tokens;
        ParserToken m_currentToken;
        int m_currentTokenIndex = -1;
    };
    
    Parser &GetParser();

} /* Namespace Aryiele. */

#define PARSER_CHECKTOKEN(EXPECTEDTOKENTYPE) \
if (GetParser().GetCurrentToken().Type != EXPECTEDTOKENTYPE) { \
    LOG_ERROR("wrong token, got '", Parser::GetTokenName(m_currentToken.Type), "' but expected '", Parser::GetTokenName(EXPECTEDTOKENTYPE), "'"); \
    return nullptr; \
}

#define PARSER_CHECKNEXTTOKEN(EXPECTEDTOKENTYPE) { \
    GetNextToken(); \
    PARSER_CHECKTOKEN(EXPECTEDTOKENTYPE); \
}

#endif /* ARYIELE_PARSER_PARSER_H. */
