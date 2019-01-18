#include <Aryiele/Tokenizer/Tokenizer.h>
#include <Aryiele/Tokenizer/TokenizerTable.h>

namespace Aryiele
{
    std::vector<TokenizerToken> Tokenizer::Tokenize(const std::string& filepath)
    {
        std::ifstream file;
        std::string expression;

        file.open(filepath.c_str());

        if (file.fail())
            return m_tokens;

        while (getline(file, expression))
            UseFiniteStateMachine(expression);

        RemoveComments();
        DetailTokens();

        return m_tokens;
    }

    void Tokenizer::UseFiniteStateMachine(std::string expression)
    {
        TokenizerToken currentToken;
        std::vector<TokenizerToken> tokens;
        auto currentTransitionState = TokenizerTokens_Reject;
        auto previousTransitionState = TokenizerTokens_Reject;
        std::string currentTokenExpression;

        for (unsigned x = 0; x < expression.length();)
        {
            const auto currentCharacter = expression[x];
            const auto column = GetTransitionTableColumn(currentCharacter);

            currentTransitionState = static_cast<TokenizerTokens>(TokenizerTable[currentTransitionState][column]);

            if (currentTransitionState == TokenizerTokens_Reject)
            {
                currentToken.Content = currentTokenExpression;
                currentToken.Type = previousTransitionState;
                currentTokenExpression = "";

                tokens.push_back(currentToken);
            }
            else
            {
                currentTokenExpression += currentCharacter;
                ++x;
            }

            previousTransitionState = currentTransitionState;
        }

        if (!currentTokenExpression.empty())
        {
            currentToken.Content = currentTokenExpression;
            currentToken.Type = currentTransitionState;

            tokens.push_back(currentToken);
        }

        currentToken.Content = std::string();
        currentToken.Type = TokenizerTokens_Newline;

        tokens.push_back(currentToken);

        m_tokens.insert(m_tokens.end(), tokens.begin(), tokens.end());
    }

    void Tokenizer::RemoveComments()
    {
        TokenizerToken currentToken;
        std::vector<TokenizerToken> tokens;
        auto isInCommentSingleLine = false;
        auto isInCommentMultiLine = false;

        for (auto& token : m_tokens)
        {
            const auto lastToken = currentToken;
            currentToken = token;

            if (isInCommentSingleLine)
            {
                if (currentToken.Type == TokenizerTokens_Newline)
                    isInCommentSingleLine = false;
            }
            else if (isInCommentMultiLine)
            {
                if (currentToken.Content == "*/")
                    isInCommentMultiLine = false;
            }
            else if (currentToken.Content == "//")
            {
                isInCommentSingleLine = true;
            }
            else if (currentToken.Content == "/*")
            {
                isInCommentMultiLine = true;
            }
            else
            {
                tokens.emplace_back(currentToken);
            }
        }

        m_tokens.clear();

        m_tokens = tokens;
    }

    void Tokenizer::DetailTokens()
    {
        TokenizerToken currentToken;
        std::vector<TokenizerToken> tokens;
        std::string inTextQuote;
        std::string currentText;
        auto isInText = false;

        for (auto& token : m_tokens)
        {
            const auto lastToken = currentToken;
            currentToken = token;

            if (currentToken.Type == TokenizerTokens_Newline)
                continue;
            if (currentToken.Type == TokenizerTokens_Space && !isInText)
                continue;

            if (isInText)
            {
                if (currentToken.Type == TokenizerTokens_StringQuote && currentToken.Content == inTextQuote)
                {
                    if (currentText[currentText.length() - 1] == '\\')
                    {
                        currentText.pop_back();
                        currentText += inTextQuote;
                    }
                    else
                    {
                        currentToken.Content = currentText;
                        currentToken.Type = TokenizerTokens_String;

                        tokens.emplace_back(currentToken);

                        isInText = false;
                        currentText = "";
                        inTextQuote = "";
                    }
                }
                else
                {
                    currentText += currentToken.Content;
                }
            }
            else if (currentToken.Type == TokenizerTokens_StringQuote)
            {
                inTextQuote = currentToken.Content;
                isInText = true;
            }
            else
            {
                tokens.emplace_back(currentToken);
            }
        }

        m_tokens.clear();

        m_tokens = tokens;
    }

    TokenizerTokens Tokenizer::GetTransitionTableColumn(char currentCharacter)
    {
        if (currentCharacter == ';')
        {
            return TokenizerTokens_EOL;
        }
        else if (currentCharacter == '{' || currentCharacter == '}' ||
            currentCharacter == '[' || currentCharacter == ']' ||
            currentCharacter == '(' || currentCharacter == ')')
        {
            return TokenizerTokens_Scope;
        }
        else if (currentCharacter == '\"' || currentCharacter == '\'')
        {
            return TokenizerTokens_StringQuote;
        }
        else if (isspace(currentCharacter))
        {
            return TokenizerTokens_Space;
        }
        else if (isdigit(currentCharacter) || currentCharacter == '.')
        {
            return TokenizerTokens_Number;
        }
        else if (isalpha(currentCharacter))
        {
            return TokenizerTokens_Identifier;
        }
        else if (ispunct(currentCharacter))
        {
            return TokenizerTokens_Operator;
        }

        return TokenizerTokens_Unknown;
    }

    std::string Tokenizer::GetTokenName(TokenizerToken tokenType)
    {
        switch (tokenType.Type)
        {
            case TokenizerTokens_Number:
                return "Number";
            case TokenizerTokens_String:
                return "String";
            case TokenizerTokens_Operator:
                return "Operator";
            case TokenizerTokens_Identifier:
                return "Identifier";
            case TokenizerTokens_Scope:
                return "Scope";
            case TokenizerTokens_Space:
                return "Space";
            case TokenizerTokens_Newline:
                return "Newline";
            case TokenizerTokens_EOL:
                return "EOL";
            case TokenizerTokens_StringQuote:
                return "StringQuote";
            case TokenizerTokens_Unknown:
                return "Unknown";
            default:
                return "Error";
        }
    }

} /* Namespace Aryiele. */
