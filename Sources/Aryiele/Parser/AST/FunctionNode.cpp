#include <Vanir/StringUtils.h>
#include "FunctionNode.h"

namespace Aryiele
{
    FunctionNode::FunctionNode(std::shared_ptr<FunctionPrototypeNode> prototype,
                               std::vector<std::shared_ptr<ExpressionNode>> implementations) :
        m_prototype(prototype), m_implementations(implementations)
    {

    }

    void FunctionNode::DumpInformations(std::shared_ptr<ParserDumpTreeNode> parentNode)
    {
        auto node = std::make_shared<ParserDumpTreeNode>(parentNode, L"Function");

        node->Informations.emplace_back(L"Name: " + Vanir::StringUtils::StringToWString(m_prototype->GetName()));
        node->Informations.emplace_back(L"Type: " + Vanir::StringUtils::StringToWString(m_prototype->GetType()));

        for (auto& childNode : m_implementations)
            childNode->DumpInformations(node);

        parentNode->Children.emplace_back(node);
    }

} /* Namespace Aryiele. */