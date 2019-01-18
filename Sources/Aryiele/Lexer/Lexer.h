#ifndef ARYIELE_LEXER_H
#define ARYIELE_LEXER_H

#include <vector>
#include <Aryiele/Core/Includes.h>
#include <Aryiele/Lexer/LexerToken.h>

namespace Aryiele
{
    class Lexer : public Vanir::Singleton<Lexer>
    {
    public:
        std::vector<LexerToken> Tokenize(const std::string& filepath);
        std::string GetTokenName(LexerToken tokenType);

    private:
        // First pass of the Lexer (separate all characters by expression with a finite-state machine).
        void UseFiniteStateMachine(std::string expression);
        // Second pass of the Lexer (remove all comments as it ain't needed to keep them).
        void RemoveComments();
        // Third pass of the Lexer (remove all spaces; define literal strings, keywords, booleans).
        void DetailTokens();
        LexerTokens GetTransitionTableColumn(char currentCharacter);

        std::vector<LexerToken> m_tokens;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_LEXER_H. */
