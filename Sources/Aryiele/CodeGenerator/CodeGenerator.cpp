#include <Aryiele/CodeGenerator/CodeGenerator.h>
#include <Aryiele/Parser/Parser.h>
#include <Aryiele/AST/Nodes/Node.h>
#include <llvm/Transforms/Scalar/SimplifyCFG.h>
#include <llvm/Transforms/Scalar/Reassociate.h>

namespace Aryiele
{
    void CodeGenerator::Create()
    {
        Module = std::make_shared<llvm::Module>("Aryiele", Context);
        FunctionPassManager = std::make_shared<llvm::legacy::FunctionPassManager>(Module.get());

        FunctionPassManager->add(llvm::createInstructionCombiningPass());
        FunctionPassManager->add(llvm::createReassociatePass());
        FunctionPassManager->add(llvm::createGVNPass());
        FunctionPassManager->add(llvm::createCFGSimplificationPass());
    }

    void CodeGenerator::GenerateCode(std::vector<std::shared_ptr<Node>> nodes)
    {
        for (auto& node : nodes)
            GenerateCode(node);
    }

    llvm::Value *CodeGenerator::GenerateCode(std::shared_ptr<Node> node)
    {
        switch (node->GetType())
        {
            case Nodes_Constant_Double:
                return GenerateCode((NodeConstantDouble*)node.get());
            case Nodes_Constant_Integer:
                return GenerateCode((NodeConstantInteger*)node.get());
            case Nodes_Function_Prototype:
                return GenerateCode((NodeFunctionPrototype*)node.get());
            case Nodes_Operation_Binary:
                return GenerateCode((NodeOperationBinary*)node.get());
            case Nodes_Statement_FunctionCall:
                return GenerateCode((NodeStatementFunctionCall*)node.get());
            case Nodes_Statement_If:
                return GenerateCode((NodeStatementIf*)node.get());
            case Nodes_Statement_Return:
                return GenerateCode((NodeStatementReturn*)node.get());
            case Nodes_Variable:
                return GenerateCode((NodeVariable*)node.get());

            default:
                return nullptr;
        }
    }

    llvm::Value *CodeGenerator::GenerateCode(NodeConstantDouble* node)
    {
        return llvm::ConstantFP::get(Context, llvm::APFloat(node->Value));
    }

    llvm::Value *CodeGenerator::GenerateCode(NodeConstantInteger* node)
    {
        // TODO: Remove cast ?
        return llvm::ConstantInt::get(Context, llvm::APInt(32, static_cast<uint64_t>(node->Value)));
    }

    llvm::Value *CodeGenerator::GenerateCode(NodeStatementReturn* node)
    {
        return GenerateCode(node->Expression);
    }

    // TODO: Return + Types
    llvm::Value *CodeGenerator::GenerateCode(NodeFunctionPrototype* node)
    {
        llvm::Function *function = Module->getFunction(node->Identifier);

        if (!function)
        {
            std::vector<llvm::Type*> arguments;
            llvm::Type* functionTypeValue;

            for (const auto &argument : node->Arguments)
            {
                if (argument.Type == "double")
                    arguments.emplace_back(llvm::Type::getDoubleTy(Context));
                else if (argument.Type == "int")
                    arguments.emplace_back(llvm::Type::getInt32Ty(Context));
            }

            if (node->Type == "int")
                functionTypeValue = llvm::Type::getInt32Ty(Context);
            else if (node->Type == "double")
                functionTypeValue = llvm::Type::getDoubleTy(Context);

            llvm::FunctionType *functionType = llvm::FunctionType::get(functionTypeValue, arguments, false);

            function = llvm::Function::Create(
                functionType, llvm::Function::ExternalLinkage, node->Identifier, Module.get());

            unsigned i = 0;

            for (auto &Arg : function->args())
                Arg.setName(node->Arguments[i++].Identifier);
        }

        llvm::BasicBlock *basicBlock = llvm::BasicBlock::Create(Context, "entry", function);

        Builder.SetInsertPoint(basicBlock);

        NamedValues.clear();

        for (auto &Arg : function->args())
            NamedValues[Arg.getName()] = &Arg;

        if (llvm::Value *returnValue = GenerateCode(node->Body[0]))
        {
            Builder.CreateRet(returnValue);

            verifyFunction(*function);

            FunctionPassManager->run(*function);

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
                return CodeGenerator::GetInstance()->Builder.CreateAdd(leftValue, rightValue, "add");
            case ParserTokens_Operator_Arithmetic_Minus:
                return CodeGenerator::GetInstance()->Builder.CreateSub(leftValue, rightValue, "sub");
            case ParserTokens_Operator_Arithmetic_Multiply:
                return CodeGenerator::GetInstance()->Builder.CreateMul(leftValue, rightValue, "mul");
            case ParserTokens_Operator_Arithmetic_Divide:
                return CodeGenerator::GetInstance()->Builder.CreateSDiv(leftValue, rightValue, "sdiv");
            case ParserTokens_Operator_Comparison_LessThan:
                return CodeGenerator::GetInstance()->Builder.CreateICmpULT(leftValue, rightValue, "icmpulttmp");
            case ParserTokens_Operator_Comparison_LessThanOrEqual:
                return CodeGenerator::GetInstance()->Builder.CreateICmpULE(leftValue, rightValue, "icmpuletmp");
            default:
            {
                LOG_ERROR("unknown binary operator: ", Parser::GetTokenName(node->OperationType));

                return nullptr;
            }
        }
    }

    llvm::Value *CodeGenerator::GenerateCode(NodeStatementFunctionCall* node)
    {
        llvm::Function *calledFunction = CodeGenerator::GetInstance()->Module->getFunction(node->Identifier);

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

        return CodeGenerator::GetInstance()->Builder.CreateCall(calledFunction, argumentsValues, "calltmp");
    }

    llvm::Value *CodeGenerator::GenerateCode(NodeStatementIf* node)
    {
        llvm::Value *conditionValue = GenerateCode(node->Condition);

        if (!conditionValue)
            return nullptr;

        //conditionValue = CodeGenerator::GetInstance()->Builder.CreateICmpNE(conditionValue,
        //    llvm::ConstantInt::get(CodeGenerator::GetInstance()->Context, llvm::APInt(1, static_cast<uint64_t>(0))), "ifcond");

        llvm::Function *function = CodeGenerator::GetInstance()->Builder.GetInsertBlock()->getParent();

        llvm::BasicBlock *thenBasicBlock = llvm::BasicBlock::Create(CodeGenerator::GetInstance()->Context, "then", function);
        llvm::BasicBlock *elseBasicBlock = llvm::BasicBlock::Create(CodeGenerator::GetInstance()->Context, "else");
        llvm::BasicBlock *mergeBasicBlock = llvm::BasicBlock::Create(CodeGenerator::GetInstance()->Context, "ifcont");

        CodeGenerator::GetInstance()->Builder.CreateCondBr(conditionValue, thenBasicBlock, elseBasicBlock);

        CodeGenerator::GetInstance()->Builder.SetInsertPoint(thenBasicBlock);

        llvm::Value *ThenV = GenerateCode(node->IfBody[0]); // TODO: All
        if (!ThenV)
            return nullptr;

        CodeGenerator::GetInstance()->Builder.CreateBr(mergeBasicBlock);
        thenBasicBlock = CodeGenerator::GetInstance()->Builder.GetInsertBlock();

        function->getBasicBlockList().push_back(elseBasicBlock);
        CodeGenerator::GetInstance()->Builder.SetInsertPoint(elseBasicBlock);

        // TODO: Support for no else
        llvm::Value *ElseV = GenerateCode(node->ElseBody[0]); // TODO: All
        if (!ElseV)
            return nullptr;

        CodeGenerator::GetInstance()->Builder.CreateBr(mergeBasicBlock);
        elseBasicBlock = CodeGenerator::GetInstance()->Builder.GetInsertBlock();

        function->getBasicBlockList().push_back(mergeBasicBlock);
        CodeGenerator::GetInstance()->Builder.SetInsertPoint(mergeBasicBlock);
        llvm::PHINode *PN = CodeGenerator::GetInstance()->Builder.CreatePHI(llvm::Type::getInt32Ty(CodeGenerator::GetInstance()->Context), 2, "iftmp");

        PN->addIncoming(ThenV, thenBasicBlock);
        PN->addIncoming(ElseV, elseBasicBlock);

        return PN;
    }

    llvm::Value *CodeGenerator::GenerateCode(NodeVariable* node)
    {
        llvm::Value *value = CodeGenerator::GetInstance()->NamedValues[node->Identifier];

        if (!value)
        {
            LOG_ERROR("unknown variable: ", node->Identifier);
        }

        return value;
    }

} /* Namespace Aryiele. */