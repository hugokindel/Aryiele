#include <utility>
#include <Aryiele/Parser/Parser.h>
#include <Vanir/StringUtils.h>
#include <llvm/ADT/STLExtras.h>
#include <Aryiele/Parser/AST/ExpressionDoubleNode.h>
#include <iostream>
#include <fcntl.h>

namespace Aryiele
{
    Parser::Parser() :
        m_currentTokenIndex(-1)
    {

    }

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
                    else if (token.Content == "+" && lastToken.Type != LexerTokens_Number)
                        tokens.emplace_back(token.Content, ParserTokens_Operator_Arithmetic_UnaryPlus);
                    else if (token.Content == "+" && lastToken.Type == LexerTokens_Number)
                        tokens.emplace_back(token.Content, ParserTokens_Operator_Arithmetic_Plus);
                    else if (token.Content == "-" && lastToken.Type != LexerTokens_Number)
                        tokens.emplace_back(token.Content, ParserTokens_Operator_Arithmetic_UnaryMinus);
                    else if (token.Content == "-" && lastToken.Type == LexerTokens_Number)
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
                    else if (token.Content == ",")
                        tokens.emplace_back(token.Content, ParserTokens_Separator);
                    else
                        tokens.emplace_back(token.Content, ParserTokens_Unknown);
                    break;
                case LexerTokens_Scope:
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
                case LexerTokens_Identifier:
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
                    else
                        tokens.emplace_back(token.Content, ParserTokens_Identifier);
                    break;
                case LexerTokens_EOL:
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
            case ParserTokens_Identifier:
                return "Identifier";
            case ParserTokens_TypeDefiner:
                return "TypeDefiner";
            case ParserTokens_Separator:
                return "Separator";
            case ParserTokens_EOL:
                return "EOL";
            case ParserTokens_EOF:
                return "EOF";
            default:
                return "Unknown";
        }
    }

    void Parser::Parse(std::vector<ParserToken> tokens)
    {
        m_tokens = std::move(tokens);
        m_tokens.emplace_back("", ParserTokens_EOF);

        while (true)
        {
            GetNextToken();

            if (m_currentToken.Type == ParserTokens_EOF)
                break;
            if (m_currentToken.Type == ParserTokens_Keyword_Function)
                m_node.emplace_back(ParseFunction());
        }

        m_dumpNode = std::make_unique<ParserDumpTreeNode>(nullptr, L"File");

        for (auto& node : m_node)
            node->DumpInformations(m_dumpNode);

        _setmode(_fileno(stdout), _O_WTEXT);
        DumpInformations(m_dumpNode);
        _setmode(_fileno(stdout), _O_TEXT);
    }

    ParserToken Parser::GetNextToken()
    {
        m_currentToken = m_tokens[++m_currentTokenIndex];

        return m_currentToken;
    }

    std::shared_ptr<ExpressionNode> Parser::ParseExpressionDouble()
    {
        auto result = std::make_shared<ExpressionDoubleNode>(std::stod(m_currentToken.Content));

        GetNextToken();

        return std::move(result);
    }

    std::shared_ptr<FunctionNode> Parser::ParseFunction()
    {
        std::string name;
        std::string type;
        std::vector<Argument> arguments;
        std::vector<std::shared_ptr<ExpressionNode>> expressions;

        GetNextToken();

        if (m_currentToken.Type == ParserTokens_Identifier)
            name = m_currentToken.Content;
        else
        {
            LOG_ERROR("Expected an identifier.");

            return nullptr;
        }

        GetNextToken();

        if (m_currentToken.Type != ParserTokens_Scope_RoundBracket_Open)
        {
            LOG_ERROR("Expected an opened round bracket.");

            return nullptr;
        }

        while (true)
        {
            GetNextToken();

            if (m_currentToken.Type == ParserTokens_Scope_RoundBracket_Closed)
                break;
            else if (m_currentToken.Type == ParserTokens_Identifier)
            {
                Argument argument(m_currentToken.Content);

                GetNextToken();

                if (m_currentToken.Type != ParserTokens_TypeDefiner)
                {
                    LOG_ERROR("Expected a type definer separator.");

                    return nullptr;
                }

                GetNextToken();

                if (m_currentToken.Type == ParserTokens_Identifier)
                    argument.Type = m_currentToken.Content;
                else
                {
                    LOG_ERROR("Expected a type name.");

                    return nullptr;
                }

                arguments.emplace_back(argument);
            }
            else if (m_currentToken.Type == ParserTokens_Separator)
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

        if (m_currentToken.Type != ParserTokens_TypeDefiner)
        {
            LOG_ERROR("Expected a type definer separator.");

            return nullptr;
        }

        GetNextToken();

        if (m_currentToken.Type == ParserTokens_Identifier)
            type = m_currentToken.Content;
        else
        {
            LOG_ERROR("Expected a type name.");

            return nullptr;
        }

        GetNextToken();

        if (m_currentToken.Type != ParserTokens_Scope_CurlyBracket_Open)
        {
            LOG_ERROR("Expected an opened curly bracket.");

            return nullptr;
        }

        while (true)
        {
            GetNextToken();

            if (m_currentToken.Type == ParserTokens_Scope_CurlyBracket_Closed)
                break;

            //auto expression = ParseBase();

            //if (expression != nullptr)
            //    implementation.emplace_back(expression);
        }

        return std::make_shared<FunctionNode>(std::make_shared<FunctionPrototypeNode>(name, type, arguments), expressions);
    }

    void Parser::DumpInformations(const std::shared_ptr<ParserDumpTreeNode>& node, std::wstring indent) const
    {
        const auto isRoot = node->Parent == nullptr;
        const auto hasChildren = !node->Children.empty();
        const auto hasInformations = !node->Informations.empty();
        auto isLastSibling = true;


        if (!isRoot)
            isLastSibling = static_cast<int>(std::distance(node->Parent->Children.begin(), std::find(node->Parent->Children.begin(), node->Parent->Children.end(), node))) == static_cast<int>(node->Parent->Children.size()) - 1;

        if (isRoot)
        {
            std::wcout << node->Name << std::endl;
        }
        else
        {
            std::wcout << indent + (isLastSibling ? L"└╴" : L"├╴") << node->Name << std::endl;
            indent += isLastSibling ? L"  " : L"│ ";

        }

        if (hasChildren)
        {
            std::wcout << indent + (hasInformations ? L"├╴Children" : L"└╴Children") << std::endl;
            indent += hasInformations ? L"│ " : L"  ";

            for (auto& nodeChild : node->Children)
                DumpInformations(nodeChild, indent);
        }

        if (hasInformations)
        {
            if (hasChildren)
            {
                Vanir::StringUtils::ReverseWString(indent);
                indent = indent.erase(0, 2);
                Vanir::StringUtils::ReverseWString(indent);
            }

            std::wcout << indent + L"└╴Informations" << std::endl;

            indent += L"  ";

            for (auto& information : node->Informations)
            {
                bool isLastInformation = isLastSibling && (static_cast<int>(std::distance(node->Informations.begin(), std::find(node->Informations.begin(), node->Informations.end(), information))) == static_cast<int>(node->Informations.size()) - 1);

                std::wcout << (indent + (isLastInformation ? L"└╴" : L"├╴")).append(information) << std::endl;
            }
        }
    }

} /* Namespace Aryiele. */
