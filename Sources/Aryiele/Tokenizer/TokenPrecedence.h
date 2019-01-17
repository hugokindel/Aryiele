#ifndef ARYIELE_TOKENPRECEDENCE_H
#define ARYIELE_TOKENPRECEDENCE_H

#include <Aryiele/Core/Includes.h>
#include <map>

namespace Aryiele
{
    class TokenPrecedence : public Vanir::Singleton<TokenPrecedence>
    {
    public:
        TokenPrecedence();
        ~TokenPrecedence();

        int GetPrecedence(const std::string& binaryOperator);

        std::map<std::string, int> BinaryOperatorPrecedence;
    };
}

#endif /* ARYIELE_TOKENPRECEDENCE_H. */