#include <Aryiele/AST/Nodes/NodeConstantDouble.h>

namespace Aryiele
{
    NodeVariableDouble::NodeVariableDouble(double value) :
        m_value(value)
    {

    }

    llvm::Value* NodeVariableDouble::GenerateCode()
    {
        return llvm::ConstantFP::get(CodeGenerator::GetInstance()->Context, llvm::APFloat(m_value));
    }

    void NodeVariableDouble::DumpInformations(std::shared_ptr<ParserInformation> parentNode)
    {
        auto node = std::make_shared<ParserInformation>(parentNode, "Double");
        auto body = std::make_shared<ParserInformation>(node, "Value: " + std::to_string(m_value));

        node->Children.emplace_back(body);
        parentNode->Children.emplace_back(node);
    }

} /* Namespace Aryiele. */