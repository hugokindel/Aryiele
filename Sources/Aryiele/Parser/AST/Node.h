#ifndef ARYIELE_NODE_H
#define ARYIELE_NODE_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/Parser/ParserDumpTreeNode.h>

namespace Aryiele
{
    class Node
    {
    public:
        virtual ~Node();

        virtual void DumpInformations(std::shared_ptr<ParserDumpTreeNode> parentNode);
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_NODE_H. */
