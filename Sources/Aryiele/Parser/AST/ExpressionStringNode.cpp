#include <Aryiele/Parser/AST/ExpressionStringNode.h>

namespace Aryiele
{
    ExpressionStringNode::ExpressionStringNode(const std::string& value) :
            m_value(value)
    {

    }

    void ExpressionStringNode::DumpInformations(std::shared_ptr<ParserInformation> parentNode)
    {
        auto node = std::make_shared<ParserInformation>(parentNode, "String");
        auto body = std::make_shared<ParserInformation>(node, "Value: " + m_value);

        node->Children.emplace_back(body);
        parentNode->Children.emplace_back(node);
    }

} /* Namespace Aryiele. */