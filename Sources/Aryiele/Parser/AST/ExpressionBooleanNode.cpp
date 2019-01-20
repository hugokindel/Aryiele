#include <Aryiele/Parser/AST/ExpressionBooleanNode.h>

namespace Aryiele
{
    ExpressionBooleanNode::ExpressionBooleanNode(bool value) :
            m_value(value)
    {

    }

    void ExpressionBooleanNode::DumpInformations(std::shared_ptr<ParserInformation> parentNode)
    {
        auto node = std::make_shared<ParserInformation>(parentNode, "Boolean");
        auto body = std::make_shared<ParserInformation>(node, "Value: " + std::to_string(m_value));

        node->Children.emplace_back(body);
        parentNode->Children.emplace_back(node);
    }

} /* Namespace Aryiele. */