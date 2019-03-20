#ifndef ARYIELE_VARIABLESTACK_H
#define ARYIELE_VARIABLESTACK_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/CodeGenerator/Variable.h>

namespace Aryiele
{
    class VariableStack
    {
    public:
        std::shared_ptr<Variable> Add(std::shared_ptr<Variable> variable);
        std::shared_ptr<Variable> Get(const std::string& identifier);

        std::vector<std::shared_ptr<Variable>> Variables;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_VARIABLESTACK_H. */
