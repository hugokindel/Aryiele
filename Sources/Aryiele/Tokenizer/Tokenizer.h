#ifndef ARYIELE_TOKENIZER_H
#define ARYIELE_TOKENIZER_H

#include <vector>
#include <Aryiele/Core/Includes.h>
#include <Aryiele/Tokenizer/Token.h>

namespace Aryiele
{
    class Tokenizer : public Vanir::Singleton<Tokenizer>
    {
    public:
        std::vector<Token> Tokenize(const std::string& filepath);
        std::string GetTokenName(Token tokenType);

    private:
        // First pass of the tokenizer (separate all characters by expression with a finite-state machine).
        void UseFiniteStateMachine(std::string expression);
        // Second pass of the tokenizer (remove all comments as it ain't needed to keep them).
        void RemoveComments();
        // Third pass of the tokenizer (remove all spaces; define literal strings, keywords, booleans).
        void DetailTokens();
        TokenTypes GetTransitionTableColumn(char currentCharacter);

        std::vector<Token> m_tokens;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_TOKENIZER_H. */