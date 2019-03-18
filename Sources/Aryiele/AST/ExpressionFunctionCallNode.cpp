#include <Aryiele/AST/ExpressionFunctionCallNode.h>
#include "ExpressionFunctionCallNode.h"


namespace Aryiele
{

    ExpressionFunctionCallNode::ExpressionFunctionCallNode(const std::string &name,
                                                           std::vector<std::shared_ptr<ExpressionNode>> arguments) :
        m_name(name), m_arguments(arguments)
    {

    }

    llvm::Value *ExpressionFunctionCallNode::GenerateCode()
    {
        llvm::Function *calledFunction = CodeGenerator::GetInstance()->Module->getFunction(m_name);

        if (!calledFunction)
        {
            LOG_ERROR("unknown function referenced: ", m_name);

            return nullptr;
        }

        if (calledFunction->arg_size() != m_arguments.size())
        {
            LOG_ERROR("incorrect number of argument passed: ", calledFunction->arg_size(), " while expecting ", m_arguments.size());

            return nullptr;
        }

        std::vector<llvm::Value*> argumentsValues;

        for (unsigned i = 0, e = static_cast<unsigned int>(m_arguments.size()); i != e; ++i)
        {
            argumentsValues.push_back(m_arguments[i]->GenerateCode());

            if (!argumentsValues.back())
                return nullptr;
        }

        return CodeGenerator::GetInstance()->Builder.CreateCall(calledFunction, argumentsValues, "calltmp");
    }

    void ExpressionFunctionCallNode::DumpInformations(std::shared_ptr<ParserInformation> parentNode)
    {
        auto node = std::make_shared<ParserInformation>(parentNode, "Function Call");
        auto name = std::make_shared<ParserInformation>(node, "Identifier: " + m_name);
        // TODO: Add args
        node->Children.emplace_back(name);
        parentNode->Children.emplace_back(node);
    }

} /* Namespace Aryiele. */