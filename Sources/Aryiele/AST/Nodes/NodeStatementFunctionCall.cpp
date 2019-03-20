#include <Aryiele/AST/Nodes/NodeStatementFunctionCall.h>

namespace Aryiele
{

    NodeStatementFunctionCall::NodeStatementFunctionCall(const std::string &name,
                                                           std::vector<std::shared_ptr<Node>> arguments) :
        m_name(name), m_arguments(arguments)
    {

    }

    llvm::Value *NodeStatementFunctionCall::GenerateCode()
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

    void NodeStatementFunctionCall::DumpInformations(std::shared_ptr<ParserInformation> parentNode)
    {
        auto node = std::make_shared<ParserInformation>(parentNode, "Function Call");
        auto name = std::make_shared<ParserInformation>(node, "Identifier: " + m_name);
        auto argumentsNode = std::make_shared<ParserInformation>(node, "Arguments:");

        auto i = 0;

        for(auto& argument : m_arguments)
        {
            auto argumentNode = std::make_shared<ParserInformation>(argumentsNode, std::to_string(i));
            auto body = std::make_shared<ParserInformation>(argumentNode, "Body:");

            argument->DumpInformations(body);

            argumentNode->Children.emplace_back(body);
            argumentsNode->Children.emplace_back(argumentNode);

            i++;
        }

        node->Children.emplace_back(name);
        node->Children.emplace_back(argumentsNode);
        parentNode->Children.emplace_back(node);
    }

} /* Namespace Aryiele. */