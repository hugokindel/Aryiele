#include <Aryiele/AST/Nodes/NodeFunctionDefinition.h>

namespace Aryiele
{

    NodeFunctionDefinition::NodeFunctionDefinition(std::shared_ptr<Node> expression) :
        m_expression(expression)
    {

    }

    std::shared_ptr<Node> NodeFunctionDefinition::GetExpression()
    {
        return m_expression;
    }

    llvm::Value *NodeFunctionDefinition::GenerateCode()
    {
        return m_expression->GenerateCode();
    }

    void NodeFunctionDefinition::DumpInformations(std::shared_ptr<ParserInformation> parentNode)
    {
        auto node = std::make_shared<ParserInformation>(parentNode, "Return");
        auto body = std::make_shared<ParserInformation>(node, "Body:");

        m_expression->DumpInformations(body);

        node->Children.emplace_back(body);
        parentNode->Children.emplace_back(node);
    }

} /* Namespace Aryiele. */