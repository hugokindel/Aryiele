#ifndef ARYIELE_PARSER_H
#define ARYIELE_PARSER_H

#include <vector>
#include <Aryiele/Core/Includes.h>
#include <Aryiele/Parser/ParserToken.h>
#include <Aryiele/Parser/AST/ExpressionNode.h>
#include <Aryiele/Lexer/LexerToken.h>
#include <Aryiele/Parser/ParserPrecedence.h>
#include <Aryiele/Parser/AST/FunctionNode.h>
#include <Aryiele/Parser/ParserDumpTreeNode.h>

namespace Aryiele
{
    class Parser
    {
    public:
        Parser();

        std::vector<ParserToken> ConvertTokens(std::vector<LexerToken> tokenizerTokens);
        std::string GetTokenName(ParserToken tokenType);
        void Parse(std::vector<ParserToken> tokens);

    private:
        ParserToken GetNextToken();
        std::shared_ptr<ExpressionNode> ParseExpressionDouble();
        std::shared_ptr<FunctionNode> ParseFunction();

        void DumpInformations(const std::shared_ptr<ParserDumpTreeNode>& node, std::wstring indent = L"") const;

        std::vector<std::shared_ptr<Node>> m_node;
        std::vector<ParserToken> m_tokens;
        ParserToken m_currentToken;
        int m_currentTokenIndex;
        std::shared_ptr<ParserDumpTreeNode> m_dumpNode;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_PARSER_H. */
