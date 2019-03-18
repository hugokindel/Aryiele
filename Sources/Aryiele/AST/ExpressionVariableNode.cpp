#include <Aryiele/AST/ExpressionVariableNode.h>

namespace Aryiele
{
    ExpressionVariableNode::ExpressionVariableNode(const std::string& name) :
            m_name(name)
    {

    }

    llvm::Value* ExpressionVariableNode::GenerateCode()
    {
        llvm::Value *value = CodeGenerator::GetInstance()->NamedValues[m_name];

        if (!value)
        {
            LOG_ERROR("unknown variable: ", m_name);
        }

        return value;
    }

    void ExpressionVariableNode::DumpInformations(std::shared_ptr<ParserInformation> parentNode)
    {
        auto node = std::make_shared<ParserInformation>(parentNode, "Variable");
        auto body = std::make_shared<ParserInformation>(node, "Name: " + m_name);

        node->Children.emplace_back(body);
        parentNode->Children.emplace_back(node);
    }

} /* Namespace Aryiele. */