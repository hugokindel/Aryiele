#ifndef ARYIELE_NODEFUNCTIONPROTOTYPE_H
#define ARYIELE_NODEFUNCTIONPROTOTYPE_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/AST/Nodes/Node.h>
#include <Aryiele/AST/Argument.h>

namespace Aryiele
{
    class NodeFunctionPrototype : public Node
    {
    public:
        NodeFunctionPrototype(const std::string& identifier,
                     const std::string& type,
                     std::vector<Argument> argumentsName,
                     std::vector<std::shared_ptr<Node>> body);

        void DumpInformations(std::shared_ptr<ParserInformation> parentNode) override;
        Nodes GetType() override;

        std::string Identifier;
        std::string Type;
        std::vector<Argument> Arguments;
        std::vector<std::shared_ptr<Node>> Body;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_NODEFUNCTIONPROTOTYPE_H. */
