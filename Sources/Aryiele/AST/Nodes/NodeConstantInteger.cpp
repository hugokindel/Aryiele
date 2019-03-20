#include <Aryiele/AST/Nodes/NodeConstantInteger.h>

namespace Aryiele
{
    NodeVariableInteger::NodeVariableInteger(int value) :
            m_value(value)
    {

    }

    llvm::Value* NodeVariableInteger::GenerateCode()
    {
        return llvm::ConstantInt::get(CodeGenerator::GetInstance()->Context, llvm::APInt(32, static_cast<uint64_t>(m_value)));
    };

    void NodeVariableInteger::DumpInformations(std::shared_ptr<ParserInformation> parentNode)
    {
        auto node = std::make_shared<ParserInformation>(parentNode, "Integer");
        auto body = std::make_shared<ParserInformation>(node, "Value: " + std::to_string(m_value));

        node->Children.emplace_back(body);
        parentNode->Children.emplace_back(node);
    }

} /* Namespace Aryiele. */