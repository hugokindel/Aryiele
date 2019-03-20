#include <Aryiele/CodeGenerator/CodeGenerator.h>
#include <Aryiele/Parser/Parser.h>
#include <Aryiele/AST/Nodes/Node.h>
#include <llvm/Transforms/Scalar/SimplifyCFG.h>
#include <llvm/Transforms/Scalar/Reassociate.h>

namespace Aryiele
{
    void CodeGenerator::Create()
    {
        m_module = std::make_shared<llvm::Module>("Aryiele", m_context);
        m_dataLayout = std::make_shared<llvm::DataLayout>(m_module.get());
    }

    void CodeGenerator::GenerateCode(std::vector<std::shared_ptr<Node>> nodes)
    {
        for (auto& node : nodes)
            GenerateCode(node);
    }

    std::shared_ptr<llvm::Module> CodeGenerator::GetModule()
    {
        return m_module;
    }

    llvm::Value *CodeGenerator::CastType(llvm::Value *value, llvm::Type *returnType)
    {
        if (value->getType()->isIntegerTy() && returnType->isIntegerTy())
        {
            auto *ival = (llvm::IntegerType *)value->getType();
            auto *ito  = (llvm::IntegerType *)returnType;

            if(ival->getBitWidth() < ito->getBitWidth())
                return m_builder.CreateZExtOrBitCast(value, returnType);
        }
        else if(value->getType()->isIntegerTy() && returnType->isDoubleTy())
        {
            return m_builder.CreateSIToFP(value, returnType);
        }
        else if(value->getType()->isDoubleTy() && returnType->isIntegerTy())
        {
            return m_builder.CreateFPToSI(value, returnType);
        }
        else if(returnType->isVoidTy())
        {
            return value;
        }

        return m_builder.CreateTruncOrBitCast(value, returnType);
    }

    llvm::Value *CodeGenerator::GenerateCode(std::shared_ptr<Node> node)
    {
        auto nodePtr = node.get();

        switch (node->GetType())
        {
            case Nodes_Constant_Double:
                return GenerateCode((NodeConstantDouble*)nodePtr);
            case Nodes_Constant_Integer:
                return GenerateCode((NodeConstantInteger*)nodePtr);
            case Nodes_Function_Prototype:
                return GenerateCode((NodeFunctionPrototype*)nodePtr);
            case Nodes_Operation_Binary:
                return GenerateCode((NodeOperationBinary*)nodePtr);
            case Nodes_Statement_FunctionCall:
                return GenerateCode((NodeStatementFunctionCall*)nodePtr);
            case Nodes_Statement_If:
                return GenerateCode((NodeStatementIf*)nodePtr);
            case Nodes_Statement_Return:
                return GenerateCode((NodeStatementReturn*)nodePtr);
            case Nodes_Variable:
                return GenerateCode((NodeVariable*)nodePtr);

            default:
                return nullptr;
        }
    }

    llvm::Value *CodeGenerator::GenerateCode(NodeConstantDouble* node)
    {
        return llvm::ConstantFP::get(m_context, llvm::APFloat(node->Value));
    }

    llvm::Value *CodeGenerator::GenerateCode(NodeConstantInteger* node)
    {
        // TODO: Remove cast ?
        return llvm::ConstantInt::get(m_context, llvm::APInt(32, static_cast<uint64_t>(node->Value)));
    }

    // TODO: Return + Types
    llvm::Value *CodeGenerator::GenerateCode(NodeFunctionPrototype* node)
    {
        llvm::Function *function = m_module->getFunction(node->Identifier);

        if (!function)
        {
            std::vector<llvm::Type*> arguments;
            llvm::Type* functionTypeValue;

            for (const auto &argument : node->Arguments)
            {
                if (argument.Type == "double")
                    arguments.emplace_back(llvm::Type::getDoubleTy(m_context));
                else if (argument.Type == "int")
                    arguments.emplace_back(llvm::Type::getInt32Ty(m_context));
            }

            if (node->Type == "int")
                functionTypeValue = llvm::Type::getInt32Ty(m_context);
            else if (node->Type == "double")
                functionTypeValue = llvm::Type::getDoubleTy(m_context);

            llvm::FunctionType *functionType = llvm::FunctionType::get(functionTypeValue, arguments, false);

            function = llvm::Function::Create(
                functionType, llvm::Function::ExternalLinkage, node->Identifier, m_module.get());

            unsigned i = 0;

            for (auto &Arg : function->args())
                Arg.setName(node->Arguments[i++].Identifier);
        }

        llvm::BasicBlock *basicBlock = llvm::BasicBlock::Create(m_context, "entry", function);

        m_builder.SetInsertPoint(basicBlock);

        m_namedValues.clear();

        for (auto &Arg : function->args())
            m_namedValues[Arg.getName()] = &Arg;

        if (llvm::Value *returnValue = GenerateCode(node->Body[0]))
        {
            m_builder.CreateRet(returnValue);

            verifyFunction(*function);

            return function;
        }

        function->eraseFromParent();

        LOG_ERROR("error generating code for the body of a function: ", node->Identifier);

        return nullptr;
    }

    llvm::Value *CodeGenerator::GenerateCode(NodeOperationBinary* node)
    {
        llvm::Value *leftValue = GenerateCode(node->LHS);
        llvm::Value *rightValue = GenerateCode(node->RHS);

        if (!leftValue || !rightValue)
            return nullptr;

        // TODO: Only support floatant (double) for now
        switch (node->OperationType)
        {
            case ParserTokens_Operator_Arithmetic_Plus:
                return CodeGenerator::GetInstance()->m_builder.CreateAdd(leftValue, rightValue, "add");
            case ParserTokens_Operator_Arithmetic_Minus:
                return CodeGenerator::GetInstance()->m_builder.CreateSub(leftValue, rightValue, "sub");
            case ParserTokens_Operator_Arithmetic_Multiply:
                return CodeGenerator::GetInstance()->m_builder.CreateMul(leftValue, rightValue, "mul");
            case ParserTokens_Operator_Arithmetic_Divide:
                return CodeGenerator::GetInstance()->m_builder.CreateSDiv(leftValue, rightValue, "sdiv");
            case ParserTokens_Operator_Comparison_LessThan:
                return CodeGenerator::GetInstance()->m_builder.CreateICmpULT(leftValue, rightValue, "icmpulttmp");
            case ParserTokens_Operator_Comparison_LessThanOrEqual:
                return CodeGenerator::GetInstance()->m_builder.CreateICmpULE(leftValue, rightValue, "icmpuletmp");
            default:
            {
                LOG_ERROR("unknown binary operator: ", Parser::GetTokenName(node->OperationType));

                return nullptr;
            }
        }
    }

    llvm::Value *CodeGenerator::GenerateCode(NodeStatementFunctionCall* node)
    {
        llvm::Function *calledFunction = CodeGenerator::GetInstance()->m_module->getFunction(node->Identifier);

        if (!calledFunction)
        {
            LOG_ERROR("unknown function referenced: ", node->Identifier);

            return nullptr;
        }

        if (calledFunction->arg_size() != node->Arguments.size())
        {
            LOG_ERROR("incorrect number of argument passed: ",
                calledFunction->arg_size(), " while expecting ", node->Arguments.size());

            return nullptr;
        }

        std::vector<llvm::Value*> argumentsValues;

        for (unsigned i = 0, e = static_cast<unsigned int>(node->Arguments.size()); i != e; ++i)
        {
            argumentsValues.push_back(GenerateCode(node->Arguments[i]));

            if (!argumentsValues.back())
                return nullptr;
        }

        return CodeGenerator::GetInstance()->m_builder.CreateCall(calledFunction, argumentsValues, "calltmp");
    }

    llvm::Value *CodeGenerator::GenerateCode(NodeStatementIf* node)
    {
        llvm::Value *conditionValue = GenerateCode(node->Condition);

        if (!conditionValue)
            return nullptr;

        //conditionValue = CodeGenerator::GetInstance()->m_builder.CreateICmpNE(conditionValue,
        //    llvm::ConstantInt::get(CodeGenerator::GetInstance()->m_context, llvm::APInt(1, static_cast<uint64_t>(0))), "ifcond");

        llvm::Function *function = CodeGenerator::GetInstance()->m_builder.GetInsertBlock()->getParent();

        llvm::BasicBlock *thenBasicBlock = llvm::BasicBlock::Create(CodeGenerator::GetInstance()->m_context, "then", function);
        llvm::BasicBlock *elseBasicBlock = llvm::BasicBlock::Create(CodeGenerator::GetInstance()->m_context, "else");
        llvm::BasicBlock *mergeBasicBlock = llvm::BasicBlock::Create(CodeGenerator::GetInstance()->m_context, "ifcont");

        CodeGenerator::GetInstance()->m_builder.CreateCondBr(conditionValue, thenBasicBlock, elseBasicBlock);

        CodeGenerator::GetInstance()->m_builder.SetInsertPoint(thenBasicBlock);

        llvm::Value *ThenV = GenerateCode(node->IfBody[0]); // TODO: All
        if (!ThenV)
            return nullptr;

        CodeGenerator::GetInstance()->m_builder.CreateBr(mergeBasicBlock);
        thenBasicBlock = CodeGenerator::GetInstance()->m_builder.GetInsertBlock();

        function->getBasicBlockList().push_back(elseBasicBlock);
        CodeGenerator::GetInstance()->m_builder.SetInsertPoint(elseBasicBlock);

        // TODO: Support for no else
        llvm::Value *ElseV = GenerateCode(node->ElseBody[0]); // TODO: All
        if (!ElseV)
            return nullptr;

        CodeGenerator::GetInstance()->m_builder.CreateBr(mergeBasicBlock);
        elseBasicBlock = CodeGenerator::GetInstance()->m_builder.GetInsertBlock();

        function->getBasicBlockList().push_back(mergeBasicBlock);
        CodeGenerator::GetInstance()->m_builder.SetInsertPoint(mergeBasicBlock);
        llvm::PHINode *PN = CodeGenerator::GetInstance()->m_builder.CreatePHI(llvm::Type::getInt32Ty(CodeGenerator::GetInstance()->m_context), 2, "iftmp");

        PN->addIncoming(ThenV, thenBasicBlock);
        PN->addIncoming(ElseV, elseBasicBlock);

        return PN;
    }

    llvm::Value *CodeGenerator::GenerateCode(NodeStatementReturn* node)
    {
        return GenerateCode(node->Expression);
    }

    llvm::Value *CodeGenerator::GenerateCode(NodeVariable* node)
    {
        llvm::Value *value = CodeGenerator::GetInstance()->m_namedValues[node->Identifier];

        if (!value)
        {
            LOG_ERROR("unknown variable: ", node->Identifier);
        }

        return value;
    }

} /* Namespace Aryiele. */