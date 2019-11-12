//==================================================================================//
//                                                                                  //
//  Copyright (c) 2019 Hugo Kindel <kindelhugo.pro@gmail.com>                       //
//                                                                                  //
//  This file is part of the Aryiele project.                                       //
//  Licensed under MIT License:                                                     //
//                                                                                  //
//  Permission is hereby granted, free of charge, to any person obtaining           //
//  a copy of this software and associated documentation files (the "Software"),    //
//  to deal in the Software without restriction, including without limitation       //
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,        //
//  and/or sell copies of the Software, and to permit persons to whom the           //
//  Software is furnished to do so, subject to the following conditions:            //
//                                                                                  //
//  The above copyright notice and this permission notice shall be included in      //
//  all copies or substantial portions of the Software.                             //
//                                                                                  //
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      //
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        //
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     //
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          //
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   //
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   //
//  SOFTWARE.                                                                       //
//                                                                                  //
//==================================================================================//

#include <llvm/Transforms/Scalar/SimplifyCFG.h>
#include <llvm/Transforms/Scalar/Reassociate.h>
#include <llvm/Transforms/Utils/BasicBlockUtils.h>
#include <Aryiele/CodeGenerator/CodeGenerator.h>
#include <Aryiele/Parser/Parser.h>
#include <Aryiele/AST/Nodes/Node.h>

namespace Aryiele {
    CodeGenerator::CodeGenerator() {
        m_module = std::make_shared<llvm::Module>("Aryiele", m_context);
        m_dataLayout = std::make_shared<llvm::DataLayout>(m_module.get());
        m_blockStack = std::make_shared<BlockStack>();
    }

    void CodeGenerator::GenerateCode(std::vector<std::shared_ptr<Node>> nodes) {
        // PRINTF TODO: Extern
        std::vector<llvm::Type *> Doubles(1, llvm::Type::getInt32Ty(m_context));
        llvm::FunctionType *FT =
            llvm::FunctionType::get(llvm::Type::getInt32Ty(m_context), Doubles, false);

        llvm::Function *F =
            llvm::Function::Create(FT, llvm::Function::ExternalLinkage, "print", m_module.get());

        // Set names for all arguments.
        unsigned Idx = 0;
        for (auto &Arg : F->args())
            Arg.setName("value");
        // --

        m_blockStack->Create();

        for (auto& node : nodes)
            GenerateCode(node);

        m_blockStack->EscapeCurrent();
    }

    std::shared_ptr<llvm::Module> CodeGenerator::GetModule() {
        return m_module;
    }

    llvm::Value *CodeGenerator::CastType(llvm::Value *value, llvm::Type *returnType) {
        if (value->getType()->isIntegerTy() && returnType->isIntegerTy()) {
            auto *ival = (llvm::IntegerType *)value->getType();
            auto *ito  = (llvm::IntegerType *)returnType;

            if(ival->getBitWidth() < ito->getBitWidth())
                return m_builder.CreateZExtOrBitCast(value, returnType);
        }
        else if(value->getType()->isIntegerTy() && returnType->isDoubleTy()) {
            return m_builder.CreateSIToFP(value, returnType);
        }
        else if(value->getType()->isDoubleTy() && returnType->isIntegerTy()) {
            return m_builder.CreateFPToSI(value, returnType);
        }
        else if(returnType->isVoidTy()) {
            return value;
        }

        return m_builder.CreateTruncOrBitCast(value, returnType);
    }
    
    // Definition code from https://llvm.org/docs/tutorial/LangImpl07.html
    llvm::AllocaInst *
    CodeGenerator::CreateEntryBlockAllocation(llvm::Function *function, const std::string &identifier, llvm::Type *type) {
        llvm::IRBuilder<> TmpB(&function->getEntryBlock(), function->getEntryBlock().begin());

        return TmpB.CreateAlloca(type == nullptr ? llvm::Type::getInt32Ty(m_context) : type, nullptr, identifier);
    }

    GenerationError CodeGenerator::GenerateCode(std::shared_ptr<Node> node) {
        auto nodePtr = node.get();

        switch (node->GetType()) {
            case Nodes_Function_Prototype:
                return GenerateCode((NodeFunction*)nodePtr);
            case Nodes_Constant_Double:
                return GenerateCode((NodeConstantDouble*)nodePtr);
            case Nodes_Constant_Integer:
                return GenerateCode((NodeConstantInteger*)nodePtr);
            case Nodes_Variable:
                return GenerateCode((NodeVariable*)nodePtr);
            case Nodes_Operation_Binary:
                return GenerateCode((NodeOperationBinary*)nodePtr);
            case Nodes_Statement_FunctionCall:
                return GenerateCode((NodeStatementFunctionCall*)nodePtr);
            case Nodes_Statement_If:
                return GenerateCode((NodeStatementIf*)nodePtr);
            case Nodes_Statement_Return:
                return GenerateCode((NodeStatementReturn*)nodePtr);
            case Nodes_Statement_Block:
                return GenerateCode((NodeStatementBlock*)nodePtr);
            case Nodes_Statement_VariableDeclaration:
                return GenerateCode((NodeStatementVariableDeclaration*)nodePtr);

            default:
                return GenerationError();
        }
    }

    // TODO: Return + Types
    GenerationError CodeGenerator::GenerateCode(NodeFunction* node) {
        llvm::Function *function = m_module->getFunction(node->Identifier);

        if (!function) {
            std::vector<llvm::Type*> arguments;
            llvm::Type* functionTypeValue;

            for (const auto &argument : node->Arguments) {
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

        m_blockStack->Create();

        llvm::BasicBlock *basicBlock = llvm::BasicBlock::Create(m_context, "entry", function);

        m_builder.SetInsertPoint(basicBlock);

        for (auto &argument : function->args()) {
            llvm::AllocaInst *allocationInstance = CreateEntryBlockAllocation(function, argument.getName(), argument.getType());

            m_builder.CreateStore(&argument, allocationInstance);

            m_blockStack->Current->Variables[argument.getName()] = allocationInstance;
        }

        for (auto& statement : node->Body) {
            auto error = GenerateCode(statement);

            if (!error.Success) {
                function->eraseFromParent();

                LOG_ERROR("cannot generate the body of a function: ", node->Identifier);

                return GenerationError();
            }
        }

        m_blockStack->EscapeCurrent();

        verifyFunction(*function);

        return GenerationError(true, function);
    }

    GenerationError CodeGenerator::GenerateCode(NodeOperationBinary* node) {
        if (node->OperationType == ParserTokens_Operator_Equal) {
            auto lhs = std::static_pointer_cast<NodeVariable>(node->LHS);

            if (!lhs) {
                LOG_ERROR("cannot generate a binary operation: lhs: expecting a variable");

                return GenerationError();
            }

            auto rhsValue = GenerateCode(node->RHS);

            if (!rhsValue.Success) {
                LOG_ERROR("cannot generate a binary operation: rhs: generation failed");

                return GenerationError();
            }

            llvm::Value *variable = m_blockStack->FindVariable(lhs->Identifier);

            if (!variable) {
                LOG_ERROR("cannot generate a binary operation: lhs: unknown variable '" + lhs->Identifier + "'");

                return GenerationError();
            }

            m_builder.CreateStore(rhsValue.Value, variable);

            return GenerationError(true, rhsValue.Value);
        }

        auto lhsValue = GenerateCode(node->LHS);
        auto rhsValue = GenerateCode(node->RHS);

        if (!lhsValue.Success || !rhsValue.Success)
            return GenerationError();

        llvm::Value *value = nullptr;

        // TODO: CODEGENERATOR: Only support integers for now
        // TODO: CODEGENERATOR: Only support signed numbers for now
        switch (node->OperationType) {
            case ParserTokens_Operator_Arithmetic_Plus:
                value = m_builder.CreateAdd(lhsValue.Value, rhsValue.Value, "add");
                break;
            case ParserTokens_Operator_Arithmetic_Minus:
                value = m_builder.CreateSub(lhsValue.Value, rhsValue.Value, "sub");
                break;
            case ParserTokens_Operator_Arithmetic_Multiply:
                value = m_builder.CreateMul(lhsValue.Value, rhsValue.Value, "mul");
                break;
            case ParserTokens_Operator_Arithmetic_Divide:
                value = m_builder.CreateSDiv(lhsValue.Value, rhsValue.Value, "sdiv");
                break;
            case ParserTokens_Operator_Comparison_LessThan:
                value = m_builder.CreateICmpULT(lhsValue.Value, rhsValue.Value, "icmpult");
                break;
            case ParserTokens_Operator_Comparison_LessThanOrEqual:
                value = m_builder.CreateICmpULE(lhsValue.Value, rhsValue.Value, "icmpule");
                break;
            case ParserTokens_Operator_Comparison_GreaterThan:
                value = m_builder.CreateICmpUGT(lhsValue.Value, rhsValue.Value, "icmpugt");
                break;
            case ParserTokens_Operator_Comparison_GreaterThanOrEqual:
                value = m_builder.CreateICmpUGE(lhsValue.Value, rhsValue.Value, "icmpuge");
                break;
            case ParserTokens_Operator_Comparison_Equal:
                value = m_builder.CreateICmpEQ(lhsValue.Value, rhsValue.Value, "icmpeq");
                break;
            case ParserTokens_Operator_Comparison_NotEqual:
                value = m_builder.CreateICmpNE(lhsValue.Value, rhsValue.Value, "icmpeq");
                break;
            default: {
                LOG_ERROR("unknown binary operator: ", Parser::GetTokenName(node->OperationType));

                return GenerationError();
            }
        }

        return GenerationError(true, value);
    }

    GenerationError CodeGenerator::GenerateCode(NodeConstantDouble* node) {
        return GenerationError(true, llvm::ConstantFP::get(m_context, llvm::APFloat(node->Value)));
    }

    GenerationError CodeGenerator::GenerateCode(NodeConstantInteger* node) {
        return GenerationError(true, llvm::ConstantInt::get(m_context, llvm::APInt(32, node->Value)));
    }

    GenerationError CodeGenerator::GenerateCode(NodeVariable* node) {
        llvm::Value *value = m_blockStack->FindVariable(node->Identifier);

        if (!value) {
            LOG_ERROR("unknown variable: ", node->Identifier);
        }

        return GenerationError(true, m_builder.CreateLoad(value, node->Identifier.c_str()));
    }


    GenerationError CodeGenerator::GenerateCode(NodeStatementFunctionCall* node) {
        llvm::Function *calledFunction = m_module->getFunction(node->Identifier);

        if (!calledFunction) {
            LOG_ERROR("unknown function referenced: ", node->Identifier);

            return GenerationError();
        }

        if (calledFunction->arg_size() != node->Arguments.size()) {
            LOG_ERROR("incorrect number of argument passed: ",
                      node->Arguments.size(), " while expecting ", calledFunction->arg_size());

            return GenerationError();
        }

        std::vector<llvm::Value*> argumentsValues;

        for (unsigned i = 0, e = static_cast<unsigned int>(node->Arguments.size()); i != e; ++i) {
            auto error = GenerateCode(node->Arguments[i]);

            if (!error.Success)
                return GenerationError();

            argumentsValues.push_back(error.Value);
        }

        return GenerationError(true, m_builder.CreateCall(calledFunction, argumentsValues, "calltmp"));
    }

    // TODO: CODEGENERATOR: Support for no "Else" statement
    GenerationError CodeGenerator::GenerateCode(NodeStatementIf* node) {
        auto conditionValue = GenerateCode(node->Condition);

        if (!conditionValue.Success)
            return GenerationError();

        llvm::Function *function = m_builder.GetInsertBlock()->getParent();

        llvm::BasicBlock *ifBasicBlock = llvm::BasicBlock::Create(m_context, "if", function);
        llvm::BasicBlock *elseBasicBlock = llvm::BasicBlock::Create(m_context, "else", function);
        llvm::BasicBlock *mergeBasicBlock = llvm::BasicBlock::Create(m_context, "merge", function);

        m_builder.CreateCondBr(conditionValue.Value, ifBasicBlock, elseBasicBlock);
        m_builder.SetInsertPoint(ifBasicBlock);
        m_blockStack->Create();

        for (auto &statement : node->IfBody) {
            auto generatedCode = GenerateCode(statement);

            if (!generatedCode.Success)
                return GenerationError();
        }

        m_blockStack->EscapeCurrent();
        m_builder.CreateBr(mergeBasicBlock);
        m_builder.SetInsertPoint(elseBasicBlock);

        if (!node->ElseBody.empty()) {
            m_blockStack->Create();

            for (auto &statement : node->ElseBody) {
                auto generatedCode = GenerateCode(statement);

                if (!generatedCode.Success)
                    return GenerationError();
            }

            m_blockStack->EscapeCurrent();
        }

        m_builder.CreateBr(mergeBasicBlock);
        m_builder.SetInsertPoint(mergeBasicBlock);

        return GenerationError(true);
    }

    GenerationError CodeGenerator::GenerateCode(NodeStatementReturn* node) {
        auto error = GenerateCode(node->Expression);

        if (!error.Success) {
            LOG_ERROR("cannot generate return value");

            return GenerationError();
        }

        m_builder.CreateRet(error.Value);

        return GenerationError(true, error.Value);
    }

    GenerationError CodeGenerator::GenerateCode(NodeStatementBlock *node) {
        m_blockStack->Create();

        for (auto &statement : node->Body) {
            auto error = GenerateCode(statement);

            if (!error.Success) {
                LOG_ERROR("cannot generate the body of a block in function");
            }
        }

        m_blockStack->EscapeCurrent();

        return GenerationError(true);
    }

    // TODO: CODEGENERATOR: Support other variable types (for now only int32).
    GenerationError CodeGenerator::GenerateCode(NodeStatementVariableDeclaration *node) {
        llvm::Function *function = m_builder.GetInsertBlock()->getParent();

        for (auto &variable : node->Variables) {
            GenerationError error;

            if (variable->Expression) {
                error = GenerateCode(variable->Expression);

                if (!error.Success) {
                    LOG_ERROR("cannot generate declaration of a variable");

                    return GenerationError();
                }
            }
            else {
                error.Value = llvm::ConstantInt::get(m_context, llvm::APInt(32, 0));
            }

            llvm::AllocaInst *allocationInstance = CreateEntryBlockAllocation(function, variable->Identifier);

            m_builder.CreateStore(error.Value, allocationInstance);

            m_blockStack->Current->Variables[variable->Identifier] = allocationInstance;
        }

        return GenerationError(true);
    }
    
    CodeGenerator &GetCodeGenerator() {
        return CodeGenerator::GetInstance();
    }

} /* Namespace Aryiele. */