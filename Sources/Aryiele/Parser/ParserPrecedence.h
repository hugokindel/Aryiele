#ifndef ARYIELE_PARSERPRECEDENCE_H
#define ARYIELE_PARSERPRECEDENCE_H

#include <Aryiele/Core/Includes.h>
#include <map>

namespace Aryiele
{
    class ParserPrecedence : public Vanir::Singleton<ParserPrecedence>
    {
    public:
        ParserPrecedence();
        int GetPrecedence(const std::string& binaryOperator);

    private:
        std::map<std::string, int> m_binaryOperatorPrecedence;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_PARSERPRECEDENCE_H. */
