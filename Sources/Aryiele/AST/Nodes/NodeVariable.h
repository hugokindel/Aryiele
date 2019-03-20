#ifndef ARYIELE_NODEVARIABLE_H
#define ARYIELE_NODEVARIABLE_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/AST/Nodes/Node.h>

namespace Aryiele
{
    class NodeVariable : public Node
    {
    public:
        explicit NodeVariable(const std::string& identifier);

        void DumpInformations(std::shared_ptr<ParserInformation> parentNode) override;
        Nodes GetType() override;

        std::string Identifier;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_NODEVARIABLE_H. */
