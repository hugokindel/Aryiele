#include <Aryiele/CodeGenerator/VariableStack.h>

namespace Aryiele
{
    std::shared_ptr<Variable> VariableStack::Add(std::shared_ptr<Variable> variable)
    {
        Variables.push_back(variable);

        return Variables.back();
    }

    std::shared_ptr<Variable> VariableStack::Get(const std::string &identifier)
    {
        for (auto& variable : Variables)
        {
            if (variable->Identifier == identifier)
                return variable;
        }

        return nullptr;
    }

} /* Namespace Aryiele. */