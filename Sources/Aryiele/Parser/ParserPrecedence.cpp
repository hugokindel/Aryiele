#include "ParserPrecedence.h"

namespace Aryiele
{
    /* TODO: Add operators:
     * Bitwise operator ?
     * Ternary operator ?
     * Negative logical operator ? */
    ParserPrecedence::ParserPrecedence()
    {
        m_binaryOperatorPrecedence[";"] = -1;

        m_binaryOperatorPrecedence["&&"] = 10;
        m_binaryOperatorPrecedence["||"] = 10;

        m_binaryOperatorPrecedence["<"] = 20;
        m_binaryOperatorPrecedence[">"] = 20;
        m_binaryOperatorPrecedence["<="] = 20;
        m_binaryOperatorPrecedence[">="] = 20;
        m_binaryOperatorPrecedence["=="] = 20;
        m_binaryOperatorPrecedence["!="] = 20;

        m_binaryOperatorPrecedence["+"] = 30;
        m_binaryOperatorPrecedence["-"] = 30;

        m_binaryOperatorPrecedence["%"] = 40;
        m_binaryOperatorPrecedence["*"] = 40;
        m_binaryOperatorPrecedence["/"] = 40;
    }

    int ParserPrecedence::GetPrecedence(const std::string& binaryOperator)
    {
        if(m_binaryOperatorPrecedence.find(binaryOperator) == m_binaryOperatorPrecedence.end())
            return -1;
        else
            return m_binaryOperatorPrecedence[binaryOperator] <= 0 ? -1 : m_binaryOperatorPrecedence[binaryOperator];
    }

} /* Namespace Aryiele. */