#ifndef ARYIELE_PARSER_H
#define ARYIELE_PARSER_H

#include <vector>
#include <Aryiele/Core/Includes.h>
#include <Aryiele/Parser/ParserToken.h>
#include <Aryiele/AST/Nodes/Node.h>
#include <Aryiele/Lexer/LexerToken.h>
#include <Aryiele/Parser/ParserPrecedence.h>
#include <Aryiele/AST/Nodes/NodeFunctionPrototype.h>
#include <Aryiele/Parser/ParserInformation.h>

namespace Aryiele
{
    class Parser : public Vanir::Singleton<Parser>
    {
    public:
        std::vector<ParserToken> ConvertTokens(std::vector<LexerToken> tokenizerTokens);
        static std::string GetTokenName(ParserTokens tokenType);
        std::vector<std::shared_ptr<Node>> Parse(std::vector<ParserToken> tokens);
        ParserToken GetCurrentToken();

    private:
        ParserToken GetNextToken();
        ParserToken GetPreviousToken();
        std::vector<std::shared_ptr<Node>> ParseBody();
        std::shared_ptr<Node> ParseInteger();
        std::shared_ptr<Node> ParseDouble();
        std::shared_ptr<Node> ParseIdentifier();
        std::shared_ptr<Node> ParseParenthese();
        std::shared_ptr<Node> ParseReturn();
        std::shared_ptr<Node> ParseIf();
        std::shared_ptr<Node> ParseBinaryOperationLeft();
        std::shared_ptr<Node> ParseBinaryOperationRight(int expressionPrecedence, std::shared_ptr<Node> leftExpression);
        std::shared_ptr<Node> ParseExpression();

        std::shared_ptr<NodeFunctionPrototype> ParseFunction();

        std::vector<std::shared_ptr<Node>> m_nodes;
        std::vector<ParserToken> m_tokens;
        ParserToken m_currentToken;
        int m_currentTokenIndex = -1;
    };

} /* Namespace Aryiele. */

#define PARSER_CHECKTOKEN(EXPECTEDTOKENTYPE) \
if (Parser::GetInstance()->GetCurrentToken().Type != EXPECTEDTOKENTYPE) \
{ \
    LOG_ERROR("wrong token, got '", Parser::GetTokenName(m_currentToken.Type), "' but expected '", Parser::GetTokenName(EXPECTEDTOKENTYPE), "'"); \
    return nullptr; \
}

#endif /* ARYIELE_PARSER_H. */
