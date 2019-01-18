#ifndef ARYIELE_PARSERDUMPTREENODE_H
#define ARYIELE_PARSERDUMPTREENODE_H

#include <string>
#include <vector>
#include <memory>

namespace Aryiele
{
    struct ParserDumpTreeNode
    {
        ParserDumpTreeNode(std::shared_ptr<ParserDumpTreeNode> parent = nullptr, std::wstring name = L"Unnamed", std::vector<std::shared_ptr<ParserDumpTreeNode>> children = std::vector<std::shared_ptr<ParserDumpTreeNode>>(), std::vector<std::wstring> informations = std::vector<std::wstring>());

        std::shared_ptr<ParserDumpTreeNode> Parent;
        std::wstring Name;
        std::vector<std::shared_ptr<ParserDumpTreeNode>> Children;
        std::vector<std::wstring> Informations;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_PARSERDUMPTREENODE_H. */