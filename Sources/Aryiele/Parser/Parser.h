#ifndef ARYIELE_PARSER_H
#define ARYIELE_PARSER_H

#include <vector>
#include <Aryiele/Core/Includes.h>
#include <Aryiele/Parser/ParserToken.h>
#include <Aryiele/Parser/AST/ExpressionNode.h>
#include <Aryiele/Lexer/LexerToken.h>
#include <Aryiele/Parser/ParserPrecedence.h>
#include <Aryiele/Parser/AST/FunctionNode.h>
#include <Aryiele/Parser/ParserInformation.h>

namespace Aryiele
{
    class Parser
    {
    public:
        Parser();

        std::vector<ParserToken> ConvertTokens(std::vector<LexerToken> tokenizerTokens);
        static std::string GetTokenName(ParserTokens tokenType);
        void Parse(std::vector<ParserToken> tokens);

    private:
        ParserToken GetNextToken();
        std::shared_ptr<ExpressionNode> ParseString();
        std::shared_ptr<ExpressionNode> ParseBoolean();
        std::shared_ptr<ExpressionNode> ParseInteger();
        std::shared_ptr<ExpressionNode> ParseDouble();
        std::shared_ptr<ExpressionNode> ParseIdentifier();
        std::shared_ptr<ExpressionNode> ParseParenthese();
        std::shared_ptr<ExpressionNode> ParseReturn();
        std::shared_ptr<ExpressionNode> ParseBinaryOperationLeft();
        std::shared_ptr<ExpressionNode> ParseBinaryOperationRight(int expressionPrecedence, std::shared_ptr<ExpressionNode> leftExpression);
        std::shared_ptr<ExpressionNode> ParseExpression();

        std::shared_ptr<FunctionNode> ParseFunction();

        void DumpInformations(const std::shared_ptr<ParserInformation>& node, std::string indent = "") const;

        std::vector<std::shared_ptr<Node>> m_node;
        std::vector<ParserToken> m_tokens;
        ParserToken m_currentToken;
        int m_currentTokenIndex;
        std::shared_ptr<ParserInformation> m_dumpNode;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_PARSER_H. */
