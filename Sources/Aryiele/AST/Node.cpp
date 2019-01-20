#include <Aryiele/AST/Node.h>
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

} /* Namespace Aryiele. */