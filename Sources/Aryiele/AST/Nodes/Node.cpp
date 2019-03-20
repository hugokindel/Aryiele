#include <Aryiele/AST/Nodes/Node.h>
#include "Node.h"


namespace Aryiele
{
    Node::~Node() = default;

    void Node::DumpInformations(std::shared_ptr<ParserInformation> parentNode)
    {

    }

    llvm::Value *Node::GenerateCode()
    {
        return nullptr;
    }

    Nodes Node::GetType()
    {
        return Nodes_Unknown;
    }

} /* Namespace Aryiele. */