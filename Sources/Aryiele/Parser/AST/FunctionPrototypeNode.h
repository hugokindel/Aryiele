#ifndef ARYIELE_FUNCTIONPROTOTYPENODE_H
#define ARYIELE_FUNCTIONPROTOTYPENODE_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/Parser/AST/Node.h>
#include <Aryiele/Parser/AST/Argument.h>
#include <memory>
#include <vector>

namespace Aryiele
{
    class FunctionPrototypeNode : public Node
    {
    public:
        FunctionPrototypeNode(const std::string& name, const std::string& type, std::vector<Argument> argumentsName);

        const std::string& GetName() const;
        const std::string& GetType() const;
        const std::vector<Argument>& GetArguments() const;

    protected:
        std::string m_name;
        std::string m_type;
        std::vector<Argument> m_arguments;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_FUNCTIONPROTOTYPENODE_H. */
