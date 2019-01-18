#include <Aryiele/Parser/ParserDumpTreeNode.h>

namespace Aryiele
{
    ParserDumpTreeNode::ParserDumpTreeNode(const std::shared_ptr<ParserDumpTreeNode> parent, const std::wstring name,
                                           const std::vector<std::shared_ptr<ParserDumpTreeNode>> children,
                                           const std::vector<std::wstring> informations) :
            Parent(parent), Name(name), Children(children), Informations(informations)
    {

    }

} /* Namespace Aryiele. */