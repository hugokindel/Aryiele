#include <Aryiele/Tokenizer/Tokenizer.h>
#include <Aryiele/Tokenizer/TokenTable.h>

namespace Aryiele
{
    std::vector<Token> Tokenizer::Tokenize(const std::string& filepath)
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
        Token currentToken;
        std::vector<Token> tokens;
        auto currentTransitionState = Reject;
        auto previousTransitionState = Reject;
        std::string currentTokenExpression;

        for (unsigned x = 0; x < expression.length();)
        {
            const auto currentCharacter = expression[x];
            const auto column = GetTransitionTableColumn(currentCharacter);

            currentTransitionState = static_cast<TokenTypes>(TokenTable[currentTransitionState][column]);

            if (currentTransitionState == Reject)
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

        currentToken.Type = Newline;

        tokens.push_back(currentToken);

        m_tokens.insert(m_tokens.end(), tokens.begin(), tokens.end());
    }

    void Tokenizer::RemoveComments()
    {
        Token currentToken;
        std::vector<Token> tokens;
        auto isInCommentSingleLine = false;
        auto isInCommentMultiLine = false;

        for (auto& token : m_tokens)
        {
            const auto lastToken = currentToken;
            currentToken = token;

            if (isInCommentSingleLine)
            {
                if (currentToken.Type == Newline)
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
        Token currentToken;
        std::vector<Token> tokens;
        auto isInText = false;
        std::string inTextQuote;
        std::string currentText;

        for (auto& token : m_tokens)
        {
            const auto lastToken = currentToken;
            currentToken = token;

            if (currentToken.Type == Newline)
                continue;
            if (currentToken.Type == Space && !isInText)
                continue;

            if (isInText)
            {
                if (currentToken.Type == StringQuote && currentToken.Content == inTextQuote)
                {
                    if (currentText[currentText.length() - 1] == '\\')
                    {
                        currentText.pop_back();
                        currentText += inTextQuote;
                    }
                    else
                    {
                        currentToken.Content = currentText;
                        currentToken.Type = String;

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
            else if (currentToken.Type == StringQuote)
            {
                inTextQuote = currentToken.Content;
                isInText = true;
            }
            else if (currentToken.Type == Identifier)
            {
                if (currentToken.Content == "function" ||
                    currentToken.Content == "var" ||
                    currentToken.Content == "return" ||
                    currentToken.Content == "int")
                    currentToken.Type = Keyword;
                else if (currentToken.Content == "true" ||
                    currentToken.Content == "false")
                    currentToken.Type = Boolean;

                tokens.emplace_back(currentToken);
            }
            else
            {
                tokens.emplace_back(currentToken);
            }
        }

        m_tokens.clear();

        m_tokens = tokens;
    }

    TokenTypes Tokenizer::GetTransitionTableColumn(char currentCharacter)
    {
        if (currentCharacter == ';')
            return EndOfLine;

        if (currentCharacter == '{' ||
            currentCharacter == '}' ||
            currentCharacter == '[' ||
            currentCharacter == ']' ||
            currentCharacter == '(' ||
            currentCharacter == ')')
            return Scope;

        if (currentCharacter == '\"' ||
            currentCharacter == '\'')
            return StringQuote;

        if (isspace(currentCharacter))
        {
            return Space;
        }

        if (isdigit(currentCharacter))
        {
            return Integer;
        }

        if (currentCharacter == '.')
        {
            return Decimal;
        }

        if (isalpha(currentCharacter))
        {
            return Identifier;
        }

        if (ispunct(currentCharacter))
        {
            return Operator;
        }

        return Unknown;
    }

    std::string Tokenizer::GetTokenName(Token tokenType)
    {
        switch (tokenType.Type)
        {
            case Integer:
                return "Integer";
            case Decimal:
                return "Real";
            case String:
                return "String";
            case Boolean:
                return "Boolean";
            case Operator:
                return "Operator";
            case Identifier:
                return "Identifier";
            case Keyword:
                return "Keyword";
            case Scope:
                return "Scope";
            case Unknown:
                return "Unknown";
            case Space:
                return "Space";
            case Newline:
                return "Newline";
            case EndOfLine:
                return "EndOfLine";
            case StringQuote:
                return "StringQuote";
            default:
                return "Error";
        }
    }

} /* Namespace Aryiele. */