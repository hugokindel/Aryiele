#include <Aryiele/Parser/AST/FunctionPrototypeNode.h>

namespace Aryiele
{
    FunctionPrototypeNode::FunctionPrototypeNode(const std::string& name,
                                                 const std::string& type,
                                                 std::vector<Argument> arguments) :
        m_name(name), m_type(type), m_arguments(arguments)
    {

    }

    const std::string& FunctionPrototypeNode::GetName() const
    {
        return m_name;
    }

    const std::string& FunctionPrototypeNode::GetType() const
    {
        return m_type;
    }

    const std::vector<Argument>& FunctionPrototypeNode::GetArguments() const
    {
        return m_arguments;
    }

} /* Namespace Aryiele. */