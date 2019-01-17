#include <Aryiele/Tokenizer/TokenPrecedence.h>

namespace Aryiele
{
    TokenPrecedence::TokenPrecedence()
    {
        BinaryOperatorPrecedence[";"] = -1;
        BinaryOperatorPrecedence["="] = 2;
        BinaryOperatorPrecedence["<"] = 10;
        BinaryOperatorPrecedence["+"] = 20;
        BinaryOperatorPrecedence["-"] = 20;
        BinaryOperatorPrecedence["/"] = 40;
        BinaryOperatorPrecedence["*"] = 40;
    }

    TokenPrecedence::~TokenPrecedence()
    {

    }

    int TokenPrecedence::GetPrecedence(const std::string& binaryOperator)
    {
        const auto precedence = BinaryOperatorPrecedence[binaryOperator];

        if (precedence <= 0) 
            return -1;

        return precedence;
    }

} /* Namespace Aryiele. */