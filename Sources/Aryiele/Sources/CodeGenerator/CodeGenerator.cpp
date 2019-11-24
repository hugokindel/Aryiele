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
#include <cfloat>

namespace Aryiele {
    CodeGenerator::CodeGenerator() {
        m_module = std::make_shared<llvm::Module>("Aryiele", m_context);
        m_dataLayout = std::make_shared<llvm::DataLayout>(m_module.get());
        m_blockStack = std::make_shared<BlockStack>();
    }

    void CodeGenerator::generateCode(std::vector<std::shared_ptr<Node>> nodes) {
        std::vector<llvm::Type *> Doubles(1, llvm::Type::getInt32Ty(m_context));
        llvm::FunctionType *FT = llvm::FunctionType::get(llvm::Type::getInt32Ty(m_context), Doubles, false);
        llvm::Function *F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, "print", m_module.get());

        // Set names for all arguments.
        unsigned Idx = 0;
        for (auto &Arg : F->args())
            Arg.setName("value");
        // --

        m_blockStack->create();

        for (auto& node : nodes)
            generateCode(node);

        m_blockStack->escapeCurrent();
    }

    std::shared_ptr<llvm::Module> CodeGenerator::getModule() {
        return m_module;
    }
    
    llvm::Type* CodeGenerator::getType(const std::string &type) {
        if (type == "Int8") {
            return llvm::Type::getInt8Ty(m_context);
        } else if (type == "Int16" || type == "UInt8") {
            return llvm::Type::getInt16Ty(m_context);
        } else if (type == "Int32" || type == "UInt16") {
            return llvm::Type::getInt32Ty(m_context);
        } else if (type == "Int64" || type == "Int" || type == "UInt32") {
            return llvm::Type::getInt64Ty(m_context);
        } else if (type == "UInt64" || type == "UInt") {
            return llvm::Type::getInt128Ty(m_context);
        } else if (type == "Float") {
            return llvm::Type::getFloatTy(m_context);
        } else if (type == "Double") {
            return llvm::Type::getDoubleTy(m_context);
        } else if (type == "String" || type == "Character") {
            return llvm::Type::getInt8PtrTy(m_context);
        } else if (type == "Void") {
            return llvm::Type::getVoidTy(m_context);
        } else if (type == "Boolean") {
            return llvm::Type::getIntNTy(m_context, 1);
        }
        
        return nullptr;
    }
    
    llvm::Value *CodeGenerator::getTypeDefaultValue(const std::string &type) {
        if (type == "Int8" || type == "Boolean") {
            return llvm::ConstantInt::get(m_context, llvm::APInt(8, 0));
        } else if (type == "Int16" || type == "UInt8") {
            return llvm::ConstantInt::get(m_context, llvm::APInt(16, 0));
        } else if (type == "Int32" || type == "UInt16") {
            return llvm::ConstantInt::get(m_context, llvm::APInt(32, 0));
        } else if (type == "Int64" || type == "Int" || type == "UInt32") {
            return llvm::ConstantInt::get(m_context, llvm::APInt(64, 0));
        } else if (type == "UInt64" ) {
            return llvm::ConstantInt::get(m_context, llvm::APInt(128, 0));
        } else if (type == "Float") {
            return llvm::ConstantFP::get(m_context, llvm::APFloat(0.0f));
        } else if (type == "Double") {
            return llvm::ConstantFP::get(m_context, llvm::APFloat(0.0));
        } else if (type == "String") {
            return m_builder.CreateGlobalString(llvm::StringRef(""));
        }
    
        return nullptr;
    }

    llvm::Value *CodeGenerator::castType(llvm::Value *value, llvm::Type *returnType, bool isSigned) {
        if (value->getType()->isIntegerTy() && returnType->isIntegerTy()) {
            auto *ival = (llvm::IntegerType *)value->getType();
            auto *ito  = (llvm::IntegerType *)returnType;

            return m_builder.CreateIntCast(value, returnType, isSigned);
        } else if((value->getType()->isIntegerTy() && returnType->isDoubleTy()) ||
                  (value->getType()->isIntegerTy() && returnType->isFloatTy())) {
            return m_builder.CreateSIToFP(value, returnType);
        } else if((value->getType()->isDoubleTy() && returnType->isIntegerTy()) ||
                  (value->getType()->isFloatTy() && returnType->isIntegerTy())) {
            return m_builder.CreateFPToSI(value, returnType);
        }
        
        return value;
    }
    
    // Definition code from https://llvm.org/docs/tutorial/LangImpl07.html
    llvm::AllocaInst *
    CodeGenerator::createEntryBlockAllocation(llvm::Function *function, const std::string &identifier, llvm::Type *type) {
        llvm::IRBuilder<> TmpB(&function->getEntryBlock(), function->getEntryBlock().begin());

        return TmpB.CreateAlloca(type == nullptr ? llvm::Type::getInt32Ty(m_context) : type, nullptr, identifier);
    }

    GenerationError CodeGenerator::generateCode(std::shared_ptr<Node> node) {
        auto nodePtr = node.get();

        switch (node->getType()) {
            case Node_TopFunction:
                return generateCode((NodeTopFunction*)nodePtr);
            case Node_LiteralNumberFloating:
                return generateCode((NodeLiteralNumberFloating*)nodePtr);
            case Node_LiteralNumberInteger:
                return generateCode((NodeLiteralNumberInteger*)nodePtr);
            case Node_StatementVariable:
                return generateCode((NodeStatementVariable*)nodePtr);
            case Node_OperationBinary:
                return generateCode((NodeOperationBinary*)nodePtr);
            case Node_StatementFunctionCall:
                return generateCode((NodeStatementFunctionCall*)nodePtr);
            case Node_StatementIf:
                return generateCode((NodeStatementIf*)nodePtr);
            case Node_StatementReturn:
                return generateCode((NodeStatementReturn*)nodePtr);
            case Node_StatementBlock:
                return generateCode((NodeStatementBlock*)nodePtr);
            case Node_StatementVariableDeclaration:
                return generateCode((NodeStatementVariableDeclaration*)nodePtr);
            default:
                return GenerationError();
        }
    }

    // TODO: Return + Types
    GenerationError CodeGenerator::generateCode(NodeTopFunction* node) {
        llvm::Function *function = m_module->getFunction(node->identifier);

        if (!function) {
            std::vector<llvm::Type*> arguments;
            llvm::Type* functionTypeValue;

            for (const auto &argument : node->arguments) {
                arguments.emplace_back(getType(argument.type));
            }

            functionTypeValue = getType(node->type);

            llvm::FunctionType *functionType = llvm::FunctionType::get(functionTypeValue, arguments, false);

            function = llvm::Function::Create(
                    functionType, llvm::Function::ExternalLinkage, node->identifier, m_module.get());

            unsigned i = 0;

            for (auto &Arg : function->args())
                Arg.setName(node->arguments[i++].identifier);
        }

        m_blockStack->create();

        llvm::BasicBlock *basicBlock = llvm::BasicBlock::Create(m_context, "entry", function);

        m_builder.SetInsertPoint(basicBlock);

        for (auto &argument : function->args()) {
            llvm::AllocaInst *allocationInstance = createEntryBlockAllocation(function, argument.getName(), argument.getType());

            m_builder.CreateStore(&argument, allocationInstance);

            m_blockStack->current->variables[argument.getName()] = allocationInstance;
        }

        for (auto& statement : node->body) {
            auto error = generateCode(statement);

            if (!error.success) {
                function->eraseFromParent();

                LOG_ERROR("cannot generate the body of a function: ", node->identifier)

                return GenerationError();
            }
        }
        
        m_blockStack->escapeCurrent();

        verifyFunction(*function);

        return GenerationError(true, function);
    }

    GenerationError CodeGenerator::generateCode(NodeOperationBinary* node) {
        if (node->operationType == ParserToken_OperatorEqual) {
            auto lhs = std::static_pointer_cast<NodeStatementVariable>(node->lhs);

            if (!lhs) {
                LOG_ERROR("cannot generate a binary operation: lhs: expecting a variable")

                return GenerationError();
            }

            auto rhsValue = generateCode(node->rhs);

            if (!rhsValue.success) {
                LOG_ERROR("cannot generate a binary operation: rhs: generation failed")

                return GenerationError();
            }

            llvm::Value *variable = m_blockStack->findVariable(lhs->identifier);

            if (!variable) {
                LOG_ERROR("cannot generate a binary operation: lhs: unknown variable '" + lhs->identifier + "'")

                return GenerationError();
            }

            m_builder.CreateStore(rhsValue.value, variable);

            return GenerationError(true, rhsValue.value);
        }

        auto lhsValue = generateCode(node->lhs);
        auto rhsValue = generateCode(node->rhs);

        if (!lhsValue.success || !rhsValue.success)
            return GenerationError();

        llvm::Value *value = nullptr;
        
        if (lhsValue.value->getType() != rhsValue.value->getType()) {
            rhsValue.value = castType(rhsValue.value, lhsValue.value->getType());
        }
        
        switch (node->operationType) {
            case ParserToken_OperatorArithmeticPlus:
                value = m_builder.CreateAdd(lhsValue.value, rhsValue.value, "add");
                break;
            case ParserToken_OperatorArithmeticMinus:
                value = m_builder.CreateSub(lhsValue.value, rhsValue.value, "sub");
                break;
            case ParserToken_OperatorArithmeticMultiply:
                value = m_builder.CreateMul(lhsValue.value, rhsValue.value, "mul");
                break;
            case ParserToken_OperatorArithmeticDivide:
                value = m_builder.CreateSDiv(lhsValue.value, rhsValue.value, "sdiv");
                break;
            case ParserToken_OperatorComparisonLessThan:
                value = m_builder.CreateICmpULT(lhsValue.value, rhsValue.value, "icmpult");
                break;
            case ParserToken_OperatorComparisonLessThanOrEqual:
                value = m_builder.CreateICmpULE(lhsValue.value, rhsValue.value, "icmpule");
                break;
            case ParserToken_OperatorComparisonGreaterThan:
                value = m_builder.CreateICmpUGT(lhsValue.value, rhsValue.value, "icmpugt");
                break;
            case ParserToken_OperatorComparisonGreaterThanOrEqual:
                value = m_builder.CreateICmpUGE(lhsValue.value, rhsValue.value, "icmpuge");
                break;
            case ParserToken_OperatorComparisonEqual:
                value = m_builder.CreateICmpEQ(lhsValue.value, rhsValue.value, "icmpeq");
                break;
            case ParserToken_OperatorComparisonNotEqual:
                value = m_builder.CreateICmpNE(lhsValue.value, rhsValue.value, "icmpeq");
                break;
            default: {
                LOG_ERROR("unknown binary operator: ", Parser::getTokenName(node->operationType))

                return GenerationError();
            }
        }

        return GenerationError(true, value);
    }

    GenerationError CodeGenerator::generateCode(NodeLiteralNumberFloating* node) {
        if (node->value >= FLT_MIN && node->value <= FLT_MAX) {
            return GenerationError(true, llvm::ConstantFP::get(m_context, llvm::APFloat((float)node->value)));
        } else if (node->value >= DBL_MIN && node->value <= DBL_MAX) {
            return GenerationError(true, llvm::ConstantFP::get(m_context, llvm::APFloat(node->value)));
        }
        
        return GenerationError(false);
    }

    GenerationError CodeGenerator::generateCode(NodeLiteralNumberInteger* node) {
        if (node->value >= CHAR_MIN && node->value <= CHAR_MAX) {
            return GenerationError(true, llvm::ConstantInt::get(m_builder.getInt8Ty(), node->value));
        } else if (node->value >= SHRT_MIN && node->value <= SHRT_MAX) {
            return GenerationError(true, llvm::ConstantInt::get(m_builder.getInt16Ty(), node->value));
        } else if (node->value >= INT_MIN && node->value <= INT_MAX) {
            return GenerationError(true, llvm::ConstantInt::get(m_builder.getInt32Ty(), node->value));
        } else if (node->value >= LONG_MIN && node->value <= LONG_MAX) {
            return GenerationError(true, llvm::ConstantInt::get(m_builder.getInt64Ty(), node->value));
        }
    
        return GenerationError(false);
    }

    GenerationError CodeGenerator::generateCode(NodeStatementVariable* node) {
        llvm::Value *value = m_blockStack->findVariable(node->identifier);

        if (!value) {
            LOG_ERROR("unknown variable: ", node->identifier)
        }

        return GenerationError(true, m_builder.CreateLoad(value, node->identifier.c_str()));
    }


    GenerationError CodeGenerator::generateCode(NodeStatementFunctionCall* node) {
        llvm::Function *calledFunction = m_module->getFunction(node->identifier);

        if (!calledFunction) {
            LOG_ERROR("unknown function referenced: ", node->identifier)

            return GenerationError();
        }

        if (calledFunction->arg_size() != node->arguments.size()) {
            LOG_ERROR("incorrect number of argument passed: ",
                      node->arguments.size(), " while expecting ", calledFunction->arg_size())

            return GenerationError();
        }

        std::vector<llvm::Value*> argumentsValues;

        for (unsigned i = 0, e = static_cast<unsigned int>(node->arguments.size()); i != e; ++i) {
            auto error = generateCode(node->arguments[i]);

            if (!error.success) {
                return GenerationError();
            }
            
            if (error.value->getType() != calledFunction->getArg(i)->getType()) {
                error.value = castType(error.value, calledFunction->getArg(i)->getType());
            }

            argumentsValues.push_back(error.value);
        }
        
        return GenerationError(true, m_builder.CreateCall(calledFunction, argumentsValues, "calltmp"));
    }

    GenerationError CodeGenerator::generateCode(NodeStatementIf* node) {
        auto conditionValue = generateCode(node->condition);

        if (!conditionValue.success)
            return GenerationError();

        llvm::Function *function = m_builder.GetInsertBlock()->getParent();

        llvm::BasicBlock *ifBasicBlock = llvm::BasicBlock::Create(m_context, "if", function);
        llvm::BasicBlock *elseBasicBlock = llvm::BasicBlock::Create(m_context, "else", function);
        llvm::BasicBlock *mergeBasicBlock = llvm::BasicBlock::Create(m_context, "merge", function);

        m_builder.CreateCondBr(conditionValue.value, ifBasicBlock, elseBasicBlock);
        m_builder.SetInsertPoint(ifBasicBlock);
        m_blockStack->create();

        for (auto &statement : node->ifBody) {
            auto generatedCode = generateCode(statement);

            if (!generatedCode.success)
                return GenerationError();
        }

        m_blockStack->escapeCurrent();
        m_builder.CreateBr(mergeBasicBlock);
        m_builder.SetInsertPoint(elseBasicBlock);

        if (!node->elseBody.empty()) {
            m_blockStack->create();

            for (auto &statement : node->elseBody) {
                auto generatedCode = generateCode(statement);

                if (!generatedCode.success)
                    return GenerationError();
            }

            m_blockStack->escapeCurrent();
        }

        m_builder.CreateBr(mergeBasicBlock);
        m_builder.SetInsertPoint(mergeBasicBlock);

        return GenerationError(true);
    }

    GenerationError CodeGenerator::generateCode(NodeStatementReturn* node) {
        if (node->expression == nullptr) {
            m_builder.CreateRetVoid();
            
            return GenerationError(true);
        }
    
        auto error = generateCode(node->expression);
    
        if (!error.success) {
            LOG_ERROR("cannot generate return value")
        
            return GenerationError();
        }
    
        if (error.value->getType() != m_builder.getCurrentFunctionReturnType()) {
            error.value = castType(error.value, m_builder.getCurrentFunctionReturnType(), true);
        }
    
        m_builder.CreateRet(error.value);
    
        return GenerationError(true, error.value);
    }

    GenerationError CodeGenerator::generateCode(NodeStatementBlock *node) {
        m_blockStack->create();

        for (auto &statement : node->body) {
            auto error = generateCode(statement);

            if (!error.success) {
                LOG_ERROR("cannot generate the body of a block in function")
            }
        }

        m_blockStack->escapeCurrent();

        return GenerationError(true);
    }

    // TODO: CODEGENERATOR: Support other variable types (for now only int32).
    GenerationError CodeGenerator::generateCode(NodeStatementVariableDeclaration *node) {
        llvm::Function *function = m_builder.GetInsertBlock()->getParent();

        for (auto &variable : node->variables) {
            GenerationError error;

            if (variable->expression) {
                error = generateCode(variable->expression);

                if (!error.success) {
                    LOG_ERROR("cannot generate declaration of a variable")

                    return GenerationError();
                }
            }
            else {
                error.value = getTypeDefaultValue(variable->type);
            }

            llvm::AllocaInst *allocationInstance = createEntryBlockAllocation(function, variable->identifier);

            m_builder.CreateStore(error.value, allocationInstance);

            m_blockStack->current->variables[variable->identifier] = allocationInstance;
        }

        return GenerationError(true);
    }
    
    CodeGenerator &getCodeGenerator() {
        return CodeGenerator::getInstance();
    }

} /* Namespace Aryiele. */