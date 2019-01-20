#include <Aryiele/AST/FunctionNode.h>
#include <Vanir/StringUtils.h>

namespace Aryiele
{
    FunctionNode::FunctionNode(const std::string& name,
                               const std::string& type,
                               std::vector<Argument> arguments,
                               std::vector<std::shared_ptr<ExpressionNode>> implementations) :
        m_name(name), m_type(type), m_arguments(arguments), m_implementations(implementations)
    {

    }

    const std::string& FunctionNode::GetName() const
    {
        return m_name;
    }

    const std::string& FunctionNode::GetType() const
    {
        return m_type;
    }

    const std::vector<Argument>& FunctionNode::GetArguments() const
    {
        return m_arguments;
    }

    llvm::Value* FunctionNode::GenerateCode()
    {
        auto codeGenerator = CodeGenerator::GetInstance();

        llvm::Function *function = codeGenerator->Module->getFunction(m_name);

        if (!function)
        {
            std::vector<llvm::Type*> arguments;
            llvm::Type* functionTypeValue;

            for (const auto &argument : m_arguments)
            {
                if (argument.Type == "double")
                    arguments.emplace_back(llvm::Type::getDoubleTy(codeGenerator->Context));
                else if (argument.Type == "int")
                    arguments.emplace_back(llvm::Type::getInt32Ty(codeGenerator->Context));
            }

            if (m_type == "int")
                functionTypeValue = llvm::Type::getInt32Ty(codeGenerator->Context);
            else if (m_type == "double")
                functionTypeValue = llvm::Type::getDoubleTy(codeGenerator->Context);

            llvm::FunctionType *functionType = llvm::FunctionType::get(functionTypeValue, arguments, false);

            function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, m_name, codeGenerator->Module.get());

            unsigned i = 0;

            for (auto &Arg : function->args())
                Arg.setName(m_arguments[i++].Name);
        }

        llvm::BasicBlock *basicBlock = llvm::BasicBlock::Create(codeGenerator->Context, "entry", function);

        codeGenerator->Builder.SetInsertPoint(basicBlock);

        codeGenerator->NamedValues.clear();

        for (auto &Arg : function->args())
            codeGenerator->NamedValues[Arg.getName()] = &Arg;

        if (llvm::Value *returnValue = m_implementations[0]->GenerateCode())
        {
            codeGenerator->Builder.CreateRet(returnValue);

            verifyFunction(*function);

            return function;
        }

        function->eraseFromParent();

        LOG_ERROR("Error generating code for the body of a function: ", m_name);

        return nullptr;
    }

    void FunctionNode::DumpInformations(std::shared_ptr<ParserInformation> parentNode)
    {
        auto node = std::make_shared<ParserInformation>(parentNode, "Function");
        auto argumentsNode = std::make_shared<ParserInformation>(node, "Arguments:");
        auto valueNode = std::make_shared<ParserInformation>(node, "Body:");

        for (auto& childNode : m_implementations)
            childNode->DumpInformations(valueNode);

        int i = 0;

        for(auto& argument : m_arguments)
        {
            auto argumentNode = std::make_shared<ParserInformation>(argumentsNode, std::to_string(i));

            argumentNode->Children.emplace_back(std::make_shared<ParserInformation>(argumentNode, "Name: " + argument.Name));
            argumentNode->Children.emplace_back(std::make_shared<ParserInformation>(argumentNode, "Type: " + argument.Type));

            argumentsNode->Children.emplace_back(argumentNode);

            i++;
        }

        node->Children.emplace_back(std::make_shared<ParserInformation>(node, "Name: " + m_name));
        node->Children.emplace_back(std::make_shared<ParserInformation>(node, "Type: " + m_type));
        node->Children.emplace_back(argumentsNode);
        node->Children.emplace_back(valueNode);

        parentNode->Children.emplace_back(node);
    }

} /* Namespace Aryiele. */