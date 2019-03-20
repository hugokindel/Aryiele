#ifndef ARYIELE_NODEFUNCTION_H
#define ARYIELE_NODEFUNCTION_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/AST/Nodes/Node.h>
#include <Aryiele/AST/Argument.h>

namespace Aryiele
{
    class NodeFunction : public Node
    {
    public:
        NodeFunction(const std::string& identifier,
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

#endif /* ARYIELE_NODEFUNCTION_H. */
