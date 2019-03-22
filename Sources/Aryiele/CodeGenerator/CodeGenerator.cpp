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
        m_blockStack = std::make_shared<BlockStack>();
    }

    void CodeGenerator::GenerateCode(std::vector<std::shared_ptr<Node>> nodes)
    {
        // PRINTF TODO: Extern
        std::vector<llvm::Type *> Doubles(1, llvm::Type::getInt32Ty(m_context));
        llvm::FunctionType *FT =
            llvm::FunctionType::get(llvm::Type::getInt32Ty(m_context), Doubles, false);

        llvm::Function *F =
            llvm::Function::Create(FT, llvm::Function::ExternalLinkage, "print", m_module.get());
        // --

        // Set names for all arguments.
        unsigned Idx = 0;
        for (auto &Arg : F->args())
            Arg.setName("value");

        m_blockStack->Create();

        for (auto& node : nodes)
            GenerateCode(node);

        m_blockStack->EscapeCurrent();
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

    // Definition code from https://llvm.org/docs/tutorial/LangImpl07.html
    llvm::AllocaInst *
    CodeGenerator::CreateEntryBlockAllocation(llvm::Function *function, const std::string &identifier, llvm::Type *type)
    {
        llvm::IRBuilder<> TmpB(&function->getEntryBlock(), function->getEntryBlock().begin());

        return TmpB.CreateAlloca(type == nullptr ? llvm::Type::getInt32Ty(m_context) : type, nullptr, identifier);
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
                return GenerateCode((NodeFunction*)nodePtr);
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
        return llvm::ConstantInt::get(m_context, llvm::APInt(32, node->Value));
    }

    // TODO: Return + Types
    llvm::Value *CodeGenerator::GenerateCode(NodeFunction* node)
    {
        llvm::Function *function = m_module->getFunction(node->Identifier);

        if (!function)
        {
            std::vector<llvm::Type*> arguments;
            llvm::Type* functionTypeValue;

            for (const auto &argument : node->Arguments)
            {
                arguments.emplace_back(llvm::Type::getInt32Ty(m_context));
            }

            functionTypeValue = llvm::Type::getInt32Ty(m_context);

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

        for (auto &argument : function->args())
        {
            llvm::AllocaInst *allocationInstance = CreateEntryBlockAllocation(function, argument.getName(), argument.getType());

            m_builder.CreateStore(&argument, allocationInstance);

            m_namedValues[argument.getName()] = allocationInstance;

        }

        for (auto& statement : node->Body)
        {
            auto value = GenerateCode(statement);

            if (!value)
            {
                function->eraseFromParent();

                LOG_ERROR("cannot generate the body of a function: ", node->Identifier);

                return nullptr;
            }
        }

        verifyFunction(*function);

        return function;
    }

    llvm::Value *CodeGenerator::GenerateCode(NodeOperationBinary* node)
    {
        if (node->OperationType == ParserTokens_Operator_Equal)
        {
            auto lhs = std::static_pointer_cast<NodeVariable>(node->LHS);

            if (!lhs)
            {
                LOG_ERROR("cannot generate a binary operation: lhs: expecting a variable");

                return nullptr;
            }

            llvm::Value *rhsValue = GenerateCode(node->RHS);

            if (!rhsValue)
            {
                LOG_ERROR("cannot generate a binary operation: rhs: generation failed");

                return nullptr;
            }

            llvm::Value *variable = m_namedValues[lhs->Identifier];

            if (!variable)
            {
                LOG_ERROR("cannot generate a binary operation: lhs: unknown variable '" + lhs->Identifier + "'");

                return nullptr;
            }

            m_builder.CreateStore(rhsValue, variable);

            return rhsValue;
        }

        llvm::Value *lhsValue = GenerateCode(node->LHS);
        llvm::Value *rhsValue = GenerateCode(node->RHS);

        if (!lhsValue || !rhsValue)
            return nullptr;

        // TODO: Only support integers for now
        switch (node->OperationType)
        {
            case ParserTokens_Operator_Arithmetic_Plus:
                return m_builder.CreateAdd(lhsValue, rhsValue, "add");
            case ParserTokens_Operator_Arithmetic_Minus:
                return m_builder.CreateSub(lhsValue, rhsValue, "sub");
            case ParserTokens_Operator_Arithmetic_Multiply:
                return m_builder.CreateMul(lhsValue, rhsValue, "mul");
            case ParserTokens_Operator_Arithmetic_Divide:
                return m_builder.CreateSDiv(lhsValue, rhsValue, "sdiv");
            case ParserTokens_Operator_Comparison_LessThan:
                return m_builder.CreateICmpULT(lhsValue, rhsValue, "icmpulttmp");
            case ParserTokens_Operator_Comparison_LessThanOrEqual:
                return m_builder.CreateICmpULE(lhsValue, rhsValue, "icmpuletmp");
            default:
            {
                LOG_ERROR("unknown binary operator: ", Parser::GetTokenName(node->OperationType));

                return nullptr;
            }
        }
    }

    llvm::Value *CodeGenerator::GenerateCode(NodeStatementFunctionCall* node)
    {
        llvm::Function *calledFunction = m_module->getFunction(node->Identifier);

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

        return m_builder.CreateCall(calledFunction, argumentsValues, "calltmp");
    }

    llvm::Value *CodeGenerator::GenerateCode(NodeStatementIf* node)
    {
        llvm::Value *conditionValue = GenerateCode(node->Condition);

        if (!conditionValue)
            return nullptr;

        //conditionValue = m_builder.CreateICmpNE(conditionValue,
        //    llvm::ConstantInt::get(m_context, llvm::APInt(1, static_cast<uint64_t>(0))), "ifcond");

        llvm::Function *function = m_builder.GetInsertBlock()->getParent();

        llvm::BasicBlock *thenBasicBlock = llvm::BasicBlock::Create(m_context, "then", function);
        llvm::BasicBlock *elseBasicBlock = llvm::BasicBlock::Create(m_context, "else");
        llvm::BasicBlock *mergeBasicBlock = llvm::BasicBlock::Create(m_context, "ifcont");

        m_builder.CreateCondBr(conditionValue, thenBasicBlock, elseBasicBlock);

        m_builder.SetInsertPoint(thenBasicBlock);

        llvm::Value *ThenV = GenerateCode(node->IfBody[0]); // TODO: All
        if (!ThenV)
            return nullptr;

        m_builder.CreateBr(mergeBasicBlock);
        thenBasicBlock = m_builder.GetInsertBlock();

        function->getBasicBlockList().push_back(elseBasicBlock);
        m_builder.SetInsertPoint(elseBasicBlock);

        // TODO: Support for no else
        llvm::Value *ElseV = GenerateCode(node->ElseBody[0]); // TODO: All
        if (!ElseV)
            return nullptr;

        m_builder.CreateBr(mergeBasicBlock);
        elseBasicBlock = m_builder.GetInsertBlock();

        function->getBasicBlockList().push_back(mergeBasicBlock);
        m_builder.SetInsertPoint(mergeBasicBlock);
        llvm::PHINode *PN = m_builder.CreatePHI(llvm::Type::getInt32Ty(m_context), 2, "iftmp");

        PN->addIncoming(ThenV, thenBasicBlock);
        PN->addIncoming(ElseV, elseBasicBlock);

        return PN;
    }

    llvm::Value *CodeGenerator::GenerateCode(NodeStatementReturn* node)
    {
        auto value = GenerateCode(node->Expression);

        if (!value)
        {
            LOG_ERROR("cannot generate return value");

            return nullptr;
        }

        m_builder.CreateRet(value);

        return value;
    }

    llvm::Value *CodeGenerator::GenerateCode(NodeVariable* node)
    {
        llvm::Value *value = m_namedValues[node->Identifier];

        if (!value)
        {
            LOG_ERROR("unknown variable: ", node->Identifier);
        }

        return m_builder.CreateLoad(value, node->Identifier.c_str());
    }

} /* Namespace Aryiele. */