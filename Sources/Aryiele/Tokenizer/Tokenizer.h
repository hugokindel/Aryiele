#ifndef ARYIELE_TOKENIZER_H
#define ARYIELE_TOKENIZER_H

#include <vector>
#include <Aryiele/Core/Includes.h>
#include <Aryiele/Tokenizer/TokenizerToken.h>

namespace Aryiele
{
    class Tokenizer : public Vanir::Singleton<Tokenizer>
    {
    public:
        std::vector<TokenizerToken> Tokenize(const std::string& filepath);
        std::string GetTokenName(TokenizerToken tokenType);

    private:
        // First pass of the tokenizer (separate all characters by expression with a finite-state machine).
        void UseFiniteStateMachine(std::string expression);
        // Second pass of the tokenizer (remove all comments as it ain't needed to keep them).
        void RemoveComments();
        // Third pass of the tokenizer (remove all spaces; define literal strings, keywords, booleans).
        void DetailTokens();
        TokenizerTokens GetTransitionTableColumn(char currentCharacter);

        std::vector<TokenizerToken> m_tokens;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_TOKENIZER_H. */
