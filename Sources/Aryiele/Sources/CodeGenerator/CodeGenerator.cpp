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

#include <llvm/Transforms/Scalar/Reassociate.h>
#include <llvm/Transforms/Utils/BasicBlockUtils.h>
#include <Aryiele/CodeGenerator/CodeGenerator.h>
#include <Aryiele/Parser/Parser.h>
#include <Aryiele/AST/Nodes/Node.h>
#include <Aryiele/AST/Nodes/NodeTopFile.h>
#include <cfloat>

namespace Aryiele {
    CodeGenerator::CodeGenerator(const std::string& filename) {
        m_module = std::make_shared<llvm::Module>(filename, m_context);
        m_dataLayout = std::make_shared<llvm::DataLayout>(m_module.get());
        m_blockStack = std::make_shared<BlockStack>();
    }

    void CodeGenerator::generateCode(std::shared_ptr<NodeRoot> nodeRoot) {
        for (auto& file : nodeRoot->body) {
            auto fileNode = std::dynamic_pointer_cast<NodeTopFile>(file);
            
            for (auto& node : fileNode->body) {
                if (node->getType() == Node_TopFunction) {
                    auto function = std::dynamic_pointer_cast<NodeTopFunction>(node);
            
                    if (!allPathsReturn(node)) {
                        LOG_ERROR("in function '", function->identifier, "': ", "not all code paths return a value")
                    }
                }
            }
        }
        
        std::vector<llvm::Type *> Doubles(1, llvm::Type::getInt32Ty(m_context));
        llvm::FunctionType *FT = llvm::FunctionType::get(llvm::Type::getInt32Ty(m_context), Doubles, false);
        llvm::Function *F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, "print", m_module.get());

        // Set names for all arguments.
        unsigned Idx = 0;
        for (auto &Arg : F->args())
            Arg.setName("value");
        // --

        m_blockStack->create();
    
        for (auto& file : nodeRoot->body) {
            auto fileNode = std::dynamic_pointer_cast<NodeTopFile>(file);
        
            for (auto& node : fileNode->body) {
                generateCode(node);
            }
        }

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
    
    llvm::Value *CodeGenerator::getTypeDefaultValue(llvm::Type* type) {
        if (type->isIntegerTy(1)) {
            return llvm::ConstantInt::get(m_context, llvm::APInt(1, 0));
        } else if (type->isIntegerTy(8)) {
            return llvm::ConstantInt::get(m_context, llvm::APInt(8, 0));
        } else if (type->isIntegerTy(16)) {
            return llvm::ConstantInt::get(m_context, llvm::APInt(16, 0));
        } else if (type->isIntegerTy(32)) {
            return llvm::ConstantInt::get(m_context, llvm::APInt(32, 0));
        } else if (type->isIntegerTy(64)) {
            return llvm::ConstantInt::get(m_context, llvm::APInt(64, 0));
        } else if (type->isIntegerTy(128)) {
            return llvm::ConstantInt::get(m_context, llvm::APInt(128, 0));
        } else if (type->isFloatTy()) {
            return llvm::ConstantFP::get(m_context, llvm::APFloat(0.0f));
        } else if (type->isDoubleTy()) {
            return llvm::ConstantFP::get(m_context, llvm::APFloat(0.0));
        }
    
        return nullptr;
    }
    
    llvm::Value *CodeGenerator::getTypeDefaultValue(const std::string &type) {
        return getTypeDefaultValue(getType(type));
    }
    
    llvm::Value *CodeGenerator::getTypeDefaultStep(llvm::Type* type) {
        if (type->isIntegerTy(8) || (type->isIntOrPtrTy() && type->getNumContainedTypes() > 0 && type->getContainedType(0)->isIntegerTy(8))) {
            return llvm::ConstantInt::get(m_context, llvm::APInt(8, 1));
        } else if (type->isIntegerTy(16) || (type->isIntOrPtrTy() && type->getNumContainedTypes() > 0 && type->getContainedType(0)->isIntegerTy(16))) {
            return llvm::ConstantInt::get(m_context, llvm::APInt(16, 1));
        } else if (type->isIntegerTy(32) || (type->isIntOrPtrTy() && type->getNumContainedTypes() > 0 && type->getContainedType(0)->isIntegerTy(32))) {
            return llvm::ConstantInt::get(m_context, llvm::APInt(32, 1));
        } else if (type->isIntegerTy(64) || (type->isIntOrPtrTy() && type->getNumContainedTypes() > 0 && type->getContainedType(0)->isIntegerTy(64))) {
            return llvm::ConstantInt::get(m_context, llvm::APInt(64, 1));
        } else if (type->isIntegerTy(128) || (type->isIntOrPtrTy() && type->getNumContainedTypes() > 0 && type->getContainedType(0)->isIntegerTy(128))) {
            return llvm::ConstantInt::get(m_context, llvm::APInt(128, 1));
        } else if (type->isFloatTy()) {
            return llvm::ConstantFP::get(m_context, llvm::APFloat(1.0f));
        } else if (type->isDoubleTy()) {
            return llvm::ConstantFP::get(m_context, llvm::APFloat(1.0));
        }
    
        return nullptr;
    }
    
    llvm::Value *CodeGenerator::getTypeDefaultStep(const std::string &type) {
        return getTypeDefaultStep(getType(type));
    }
    
    llvm::Value *CodeGenerator::castType(llvm::Value *value, llvm::Type *returnType, bool isSigned) {
        if (value->getType()->isIntegerTy() && returnType->isIntegerTy()) {
            return m_builder.CreateIntCast(value, returnType, isSigned);
        } else if (value->getType()->isIntegerTy() && returnType->isIntOrPtrTy()) {
            if (returnType->getContainedType(0)->isIntegerTy(8)) {
                return m_builder.CreateIntCast(value, m_builder.getInt8Ty(), isSigned);
            } else if (returnType->getContainedType(0)->isIntegerTy(16)) {
                return m_builder.CreateIntCast(value, m_builder.getInt16Ty(), isSigned);
            } else if (returnType->getContainedType(0)->isIntegerTy(32)) {
                return m_builder.CreateIntCast(value, m_builder.getInt32Ty(), isSigned);
            }
            
            return m_builder.CreateIntCast(value, m_builder.getInt64Ty(), isSigned);
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
            case Node_OperationUnary:
                return generateCode((NodeOperationUnary*)nodePtr);
            case Node_OperationBinary:
                return generateCode((NodeOperationBinary*)nodePtr);
            case Node_OperationTernary:
                return generateCode((NodeOperationTernary*)nodePtr);
            case Node_StatementFunctionCall:
                return generateCode((NodeStatementFunctionCall*)nodePtr);
            case Node_StatementIf:
                return generateCode((NodeStatementIf*)nodePtr);
            case Node_StatementFor:
                return generateCode((NodeStatementFor*)nodePtr);
            case Node_StatementWhile:
                return generateCode((NodeStatementWhile*)nodePtr);
            case Node_StatementSwitch:
                return generateCode((NodeStatementSwitch*)nodePtr);
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

        llvm::BasicBlock *basicBlock = llvm::BasicBlock::Create(m_context, "_start", function);

        m_builder.SetInsertPoint(basicBlock);

        for (auto &argument : function->args()) {
            llvm::AllocaInst *allocationInstance = createEntryBlockAllocation(function, argument.getName(), argument.getType());

            m_builder.CreateStore(&argument, allocationInstance);
            m_blockStack->addVariable(argument.getName(), allocationInstance, false);
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
    
    GenerationError CodeGenerator::generateCode(NodeOperationTernary *node) {
        auto entryBlock = m_builder.GetInsertBlock();
        auto ternaryBasicBlock = llvm::BasicBlock::Create(m_context, "_ternary_start", m_builder.GetInsertBlock()->getParent());
    
        m_builder.CreateBr(ternaryBasicBlock);
        m_builder.SetInsertPoint(ternaryBasicBlock);
        
        auto condition = generateCode(node->condition);
        
        if (!condition.success) {
            return GenerationError();
        }
    
        auto leftBasicBlock = llvm::BasicBlock::Create(m_context, "_ternary_left", m_builder.GetInsertBlock()->getParent());
        auto rightBasicBlock = llvm::BasicBlock::Create(m_context, "_ternary_right", m_builder.GetInsertBlock()->getParent());
        llvm::BasicBlock* endBasicBlock = nullptr;
        llvm::AllocaInst* alloca = nullptr;
        
        if (!allPathsReturn(node->lhs) || !allPathsReturn(node->rhs)) {
            endBasicBlock = llvm::BasicBlock::Create(m_context, "_ternary_end", m_builder.GetInsertBlock()->getParent());
    
            alloca = createEntryBlockAllocation(m_builder.GetInsertBlock()->getParent(), "v_ternary_temp");
            m_builder.CreateStore(castType(getTypeDefaultValue("Boolean"), alloca->getType()), alloca);
        }
    
        m_builder.CreateCondBr(condition.value, leftBasicBlock, rightBasicBlock);
    
        m_builder.SetInsertPoint(leftBasicBlock);
        
        auto left = generateCode(node->lhs).value;
    
        if (!left) {
            return GenerationError();
        }
        
        if (!allPathsReturn(node->lhs)) {
            m_builder.CreateStore(castType(left, alloca->getType()), alloca);
            
            m_builder.CreateBr(endBasicBlock);
        }
        
        m_builder.SetInsertPoint(rightBasicBlock);
        auto right = generateCode(node->rhs).value;
    
        if (!right) {
            return GenerationError();
        }
        
        if (!allPathsReturn(node->rhs)) {
            m_builder.CreateStore(castType(right, alloca->getType()), alloca);
            
            m_builder.CreateBr(endBasicBlock);
        }
        
        llvm::Value* ternaryVariable = nullptr;
    
        if (!allPathsReturn(node->lhs) || !allPathsReturn(node->rhs)) {
            m_builder.SetInsertPoint(endBasicBlock);
    
            ternaryVariable = m_builder.CreateLoad(alloca, "v_ternary_temp");
        } else {
            m_builder.SetInsertPoint(entryBlock);
        }
    
        return GenerationError(true, ternaryVariable);
    }
    
    GenerationError CodeGenerator::generateCode(NodeOperationBinary* node) {
        if (node->operationType == ParserToken_OperatorEqual ||
            node->operationType == ParserToken_OperatorArithmeticPlusEqual ||
            node->operationType == ParserToken_OperatorArithmeticMinusEqual ||
            node->operationType == ParserToken_OperatorArithmeticMultiplyEqual ||
            node->operationType == ParserToken_OperatorArithmeticDivideEqual ||
            node->operationType == ParserToken_OperatorArithmeticRemainderEqual) {
            auto lhs = std::static_pointer_cast<NodeStatementVariable>(node->lhs);

            if (!lhs) {
                LOG_ERROR("cannot generate a binary operation: lhs: expecting a variable")

                return GenerationError();
            }
            
            if (m_blockStack->findVariable(lhs->identifier)->isConstant && isVariableSetAtPath(lhs->identifier, node->parent, node)) {
                LOG_ERROR("cannot redefine a constant")
    
                return GenerationError();
            }

            auto rhsValue = generateCode(node->rhs);

            if (!rhsValue.success) {
                LOG_ERROR("cannot generate a binary operation: rhs: generation failed")

                return GenerationError();
            }

            auto variable = m_blockStack->findVariable(lhs->identifier);

            if (!variable) {
                LOG_ERROR("cannot generate a binary operation: lhs: unknown variable '" + lhs->identifier + "'")

                return GenerationError();
            }
            
            if (node->operationType == ParserToken_OperatorEqual) {
                m_builder.CreateStore(castType(rhsValue.value, variable->instance->getType()), variable->instance);
            } else {
                auto load = m_builder.CreateLoad(variable->instance, lhs->identifier.c_str());
                llvm::Value* result = nullptr;
                
                if (node->operationType == ParserToken_OperatorArithmeticPlusEqual) {
                    result = m_builder.CreateAdd(load, castType(rhsValue.value, load->getType()), "add");
                } else if (node->operationType == ParserToken_OperatorArithmeticMinusEqual) {
                    result = m_builder.CreateSub(load, castType(rhsValue.value, load->getType()), "sub");
                } else if (node->operationType == ParserToken_OperatorArithmeticMultiplyEqual) {
                    result = m_builder.CreateMul(load, castType(rhsValue.value, load->getType()), "mul");
                } else if (node->operationType == ParserToken_OperatorArithmeticDivideEqual) {
                    result = m_builder.CreateSDiv(load, castType(rhsValue.value, load->getType()), "div");
                } else if (node->operationType == ParserToken_OperatorArithmeticRemainderEqual) {
                    result = m_builder.CreateURem(load, castType(rhsValue.value, load->getType()), "urem");
                }
                
                m_builder.CreateStore(castType(result, variable->instance->getType()), variable->instance);
            }
    
            return GenerationError(true);
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
            case ParserToken_OperatorArithmeticRemainder:
                value = m_builder.CreateURem(lhsValue.value, rhsValue.value, "urem");
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
                LOG_ERROR("unknown binary operator: ", ParserToken::getTypeName(node->operationType))

                return GenerationError();
            }
        }

        return GenerationError(true, value);
    }
    
    GenerationError CodeGenerator::generateCode(NodeOperationUnary* node) {
        if (node->expression->getType() == Node_StatementVariable) {
            auto variable = std::dynamic_pointer_cast<NodeStatementVariable>(node->expression);
            
            if (m_blockStack->findVariable(variable->identifier)->isConstant && isVariableSetAtPath(variable->identifier, node->parent, node)) {
                LOG_ERROR("cannot redefine a constant")
    
                return GenerationError();
            }
        }
        llvm::Value *value = nullptr;
        
        if ((!node->left && node->operationType == ParserToken_OperatorUnaryArithmeticIncrement) ||
            (!node->left && node->operationType == ParserToken_OperatorUnaryArithmeticDecrement)) {
            auto lhsValue = generateCode(node->expression);
    
            llvm::AllocaInst* alloca = createEntryBlockAllocation(m_builder.GetInsertBlock()->getParent(),
                node->operationType == ParserToken_OperatorUnaryArithmeticIncrement ? "inc" : "dec");
            auto returnValue = m_builder.CreateStore(castType(lhsValue.value, alloca->getType()), alloca);
    
            if (node->expression->getType() == Node_StatementVariable) {
                llvm::Value* operation = nullptr;
                
                if (node->operationType == ParserToken_OperatorUnaryArithmeticIncrement) {
                    operation = m_builder.CreateAdd(lhsValue.value, getTypeDefaultStep(lhsValue.value->getType()), "add");
                } else {
                    operation = m_builder.CreateSub(lhsValue.value, getTypeDefaultStep(lhsValue.value->getType()), "sub");
                }
    
                m_builder.CreateStore(operation, m_blockStack->findVariable(std::dynamic_pointer_cast<NodeStatementVariable>(node->expression)->identifier)->instance);
            }
            
            return GenerationError(true, returnValue->getValueOperand());
        } else if (node->operationType == ParserToken_OperatorUnaryArithmeticIncrement ||
                   node->operationType == ParserToken_OperatorUnaryArithmeticDecrement) {
            auto lhsValue = generateCode(node->expression);
            llvm::Value* operation = nullptr;
    
            if (node->operationType == ParserToken_OperatorUnaryArithmeticIncrement) {
                operation = m_builder.CreateAdd(lhsValue.value, getTypeDefaultStep(lhsValue.value->getType()), "add");
            } else {
                operation = m_builder.CreateSub(lhsValue.value, getTypeDefaultStep(lhsValue.value->getType()), "sub");
            }
                
            if (node->expression->getType() == Node_StatementVariable) {
                m_builder.CreateStore(operation, m_blockStack->findVariable(std::dynamic_pointer_cast<NodeStatementVariable>(node->expression)->identifier)->instance);
            }
        
            return GenerationError(true, operation);
        }
        
        return GenerationError(false);
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
        auto value = m_blockStack->findVariable(node->identifier);

        if (!value) {
            LOG_ERROR("unknown variable: ", node->identifier)
        }

        return GenerationError(true, m_builder.CreateLoad(value->instance, node->identifier.c_str()));
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
        
        if (calledFunction->getReturnType()->isVoidTy()) {
            return GenerationError(true, m_builder.CreateCall(calledFunction, argumentsValues, ""));
        }
    
        return GenerationError(true, m_builder.CreateCall(calledFunction, argumentsValues, "call"));
    }

    GenerationError CodeGenerator::generateCode(NodeStatementIf* node) {
        auto conditionValue = generateCode(node->condition);

        if (!conditionValue.success)
            return GenerationError();

        llvm::Function *function = m_builder.GetInsertBlock()->getParent();
        llvm::BasicBlock *entryBlock = m_builder.GetInsertBlock();
        llvm::BasicBlock *ifBasicBlock = llvm::BasicBlock::Create(m_context, "_if_start", function);
        llvm::BasicBlock *elseBasicBlock = nullptr;
        llvm::BasicBlock *mergeBasicBlock = nullptr;
        
        if (!node->elseBody.empty()) {
            elseBasicBlock = llvm::BasicBlock::Create(m_context, "_if_else", function);
        }
    
        if (!allPathsReturn(node->ifBody) || node->elseBody.empty() || (!node->elseBody.empty() && !allPathsReturn(node->elseBody))) {
            mergeBasicBlock = llvm::BasicBlock::Create(m_context, "_if_end", function);
        }

        m_builder.CreateCondBr(conditionValue.value, ifBasicBlock, !node->elseBody.empty() ? elseBasicBlock : mergeBasicBlock);
        
        m_builder.SetInsertPoint(ifBasicBlock);
        m_blockStack->create();

        for (auto &statement : node->ifBody) {
            auto generatedCode = generateCode(statement);

            if (!generatedCode.success)
                return GenerationError();
        }

        m_blockStack->escapeCurrent();
        
        if (!allPathsReturn(node->ifBody)) {
            m_builder.CreateBr(mergeBasicBlock);
        }
    
        if (elseBasicBlock) {
            m_builder.SetInsertPoint(elseBasicBlock);
            m_blockStack->create();

            for (auto &statement : node->elseBody) {
                auto generatedCode = generateCode(statement);

                if (!generatedCode.success)
                    return GenerationError();
            }

            m_blockStack->escapeCurrent();
    
            if (!allPathsReturn(node->elseBody)) {
                m_builder.CreateBr(mergeBasicBlock);
            }
        }
    
        if (mergeBasicBlock) {
            m_builder.SetInsertPoint(mergeBasicBlock);
        } else {
            m_builder.SetInsertPoint(entryBlock);
        }

        return GenerationError(true);
    }
    
    GenerationError CodeGenerator::generateCode(NodeStatementFor *node) {
        if (node->body.empty()) {
            return GenerationError(true);
        }
        
        auto function = m_builder.GetInsertBlock()->getParent();
        auto entryBlock = m_builder.GetInsertBlock();
        auto forBasicBlock = llvm::BasicBlock::Create(m_context, "_for_start", function);
        auto forConditionBasicBlock = llvm::BasicBlock::Create(m_context, "_for_condition", function);
        auto bodyForBasicBlock = llvm::BasicBlock::Create(m_context, "_for_body", function);
        llvm::BasicBlock* endForBasicBlock = nullptr;
        
        if (!allPathsReturn(node->body)) {
            endForBasicBlock = llvm::BasicBlock::Create(m_context, "_for_end", function);
        }
    
        m_blockStack->create();
        
        m_builder.CreateBr(forBasicBlock);
        m_builder.SetInsertPoint(forBasicBlock);
    
        
        
        llvm::AllocaInst* alloca = nullptr;
        llvm::Value* startValue = nullptr;
        
        if (node->variable && node->variable->getType() == Node_StatementVariableDeclaration) {
            auto var = std::dynamic_pointer_cast<NodeStatementVariableDeclaration>(node->variable);
            
            alloca = createEntryBlockAllocation(m_builder.GetInsertBlock()->getParent(), var->variables[0]->identifier);
            startValue = generateCode(var->variables[0]->expression).value;
            m_builder.CreateStore(castType(startValue, alloca->getType()), alloca);
        }
        
        m_builder.CreateBr(forConditionBasicBlock);
        m_builder.SetInsertPoint(forConditionBasicBlock);
    
        if (node->variable && node->variable->getType() == Node_StatementVariableDeclaration) {
            auto var = std::dynamic_pointer_cast<NodeStatementVariableDeclaration>(node->variable);
            m_blockStack->addVariable(var->variables[0]->identifier, alloca, false);
        }
    
        llvm::Value* stepValue = nullptr;
    
        if (node->incrementalValue) {
            stepValue = generateCode(node->incrementalValue).value;
        } else if (node->variable && node->variable->getType() == Node_StatementVariableDeclaration) {
            stepValue = getTypeDefaultStep(startValue->getType());
        } else if (node->variable) {
            stepValue = getTypeDefaultStep(m_blockStack->findVariable(std::dynamic_pointer_cast<NodeStatementVariable>(node->variable)->identifier)->instance->getType());
        }
    
        auto endCondition = generateCode(node->condition).value;
        
        if (!allPathsReturn(node->body)) {
            m_builder.CreateCondBr(endCondition, bodyForBasicBlock, endForBasicBlock);
        } else {
            m_builder.CreateBr(bodyForBasicBlock);
        }
    
        m_builder.SetInsertPoint(bodyForBasicBlock);
        
        for (auto& statement : node->body) {
            generateCode(statement);
        }
    
        if (!allPathsReturn(node->body)) {
            if (node->variable) {
                std::string identifier;
                if (node->variable->getType() == Node_StatementVariableDeclaration) {
                    identifier = std::dynamic_pointer_cast<NodeStatementVariableDeclaration>(node->variable)->variables[0]->identifier;
                    auto currentVar = m_builder.CreateLoad(alloca, identifier);
                    auto nextVar = m_builder.CreateAdd(currentVar, castType(stepValue, currentVar->getType()), "v_for_next");
                    m_builder.CreateStore(nextVar, alloca);
                } else {
                    identifier = std::dynamic_pointer_cast<NodeStatementVariable>(node->variable)->identifier;
                    auto currentVar = m_builder.CreateLoad(m_blockStack->findVariable(std::dynamic_pointer_cast<NodeStatementVariable>(node->variable)->identifier)->instance, identifier);
                    auto nextVar = m_builder.CreateAdd(currentVar, castType(stepValue, currentVar->getType()), "v_for_next");
                    m_builder.CreateStore(nextVar, m_blockStack->findVariable(std::dynamic_pointer_cast<NodeStatementVariable>(node->variable)->identifier)->instance);
                }
            }
            
            m_builder.CreateBr(forConditionBasicBlock);
        }
        
        m_builder.SetInsertPoint(endForBasicBlock);
    
        m_blockStack->escapeCurrent();
        
        return GenerationError(true);
    }
    
    GenerationError CodeGenerator::generateCode(NodeStatementWhile *node) {
        if (node->body.empty()) {
            return GenerationError(true);
        }
    
        auto function = m_builder.GetInsertBlock()->getParent();
        auto entryBlock = m_builder.GetInsertBlock();
        llvm::BasicBlock* whileDoBasicBlock = nullptr;
        
        if (node->doOnce) {
            whileDoBasicBlock = llvm::BasicBlock::Create(m_context, "_do_while_start", function);
        }
        
        auto whileBasicBlock = llvm::BasicBlock::Create(m_context, node->doOnce ? "_do_while_condition" : "_while_start", function);
        auto whileBodyBasicBlock = llvm::BasicBlock::Create(m_context, node->doOnce ? "_do_while_body" : "_while_body", function);
        llvm::BasicBlock* whileEndBasicBlock = nullptr;
    
        if (!allPathsReturn(node->body)) {
            whileEndBasicBlock = llvm::BasicBlock::Create(m_context, node->doOnce ? "_do_while_end" : "_while_end", function);
        }
    
        m_blockStack->create();
        
        if (node->doOnce) {
            m_builder.CreateBr(whileDoBasicBlock);
            m_builder.SetInsertPoint(whileDoBasicBlock);
    
            for (auto& statement : node->body) {
                generateCode(statement);
            }
        }
    
        m_builder.CreateBr(whileBasicBlock);
        m_builder.SetInsertPoint(whileBasicBlock);
    
        auto endCondition = generateCode(node->condition).value;
    
        if (!allPathsReturn(node->body)) {
            m_builder.CreateCondBr(endCondition, whileBodyBasicBlock, whileEndBasicBlock);
        } else {
            m_builder.CreateBr(whileBodyBasicBlock);
        }
    
        m_builder.SetInsertPoint(whileBodyBasicBlock);
    
        for (auto& statement : node->body) {
            generateCode(statement);
        }
    
        if (!allPathsReturn(node->body)) {
            m_builder.CreateBr(whileBasicBlock);
        }
    
        m_builder.SetInsertPoint(whileEndBasicBlock);
        
        m_blockStack->escapeCurrent();
        
        return GenerationError(true);
    }
    
    GenerationError CodeGenerator::generateCode(NodeStatementSwitch *node) {
        // TODO
        
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

    GenerationError CodeGenerator::generateCode(NodeStatementVariableDeclaration *node) {
        llvm::Function *function = m_builder.GetInsertBlock()->getParent();
        llvm::Value* value;

        for (auto &variable : node->variables) {
            GenerationError error;

            if (variable->expression) {
                error = generateCode(variable->expression);

                if (!error.success) {
                    LOG_ERROR("cannot generate declaration of a variable")

                    return GenerationError();
                }
            }
            else if (!variable->isConstant) {
                error.value = getTypeDefaultValue(variable->type);
            }

            llvm::AllocaInst *allocationInstance = createEntryBlockAllocation(function, variable->identifier);
            
            if (error.value) {
                m_builder.CreateStore(castType(error.value, allocationInstance->getType()), allocationInstance);
            }
            
            m_blockStack->addVariable(variable->identifier, allocationInstance, variable->isConstant);
        }

        return GenerationError(true);
    }
    
    bool CodeGenerator::allPathsReturn(std::shared_ptr<Node> node) {
        if (node->getType() == Node_TopFunction) {
            auto functionNode = std::dynamic_pointer_cast<NodeTopFunction>(node);
            
            if (functionNode->type == "Void" || allPathsReturn(functionNode->body)) {
                return true;
            }
        } else if (node->getType() == Node_StatementIf) {
            auto ifNode = std::dynamic_pointer_cast<NodeStatementIf>(node);
    
            bool ifReturns = false;
            bool elseReturns = false;
    
            if (allPathsReturn(ifNode->ifBody)) {
                ifReturns = true;
            }
            
            if (allPathsReturn(ifNode->elseBody)) {
                elseReturns = true;
            }
    
            if (ifReturns && elseReturns) {
                return true;
            }
        } else if (node->getType() == Node_StatementFor) {
            auto forNode = std::dynamic_pointer_cast<NodeStatementFor>(node);
    
            if (allPathsReturn(forNode->body)) {
                return true;
            }
        } else if (node->getType() == Node_StatementWhile) {
            auto whileNode = std::dynamic_pointer_cast<NodeStatementWhile>(node);
    
            if (allPathsReturn(whileNode->body)) {
                return true;
            }
        } else if (node->getType() == Node_StatementSwitch) {
            auto switchNode = std::dynamic_pointer_cast<NodeStatementSwitch>(node);
    
            bool allCasesReturns = true;
            
            for (auto& caseNode : switchNode->cases) {
                auto casePtr = std::dynamic_pointer_cast<NodeStatementCase>(caseNode);
                
                if (!allPathsReturn(casePtr->body)) {
                    allCasesReturns = false;
                }
            }
            
            if (allCasesReturns && switchNode->hasDefault()) {
                return true;
            }
        } else if (node->getType() == Node_OperationTernary) {
            auto ternaryNode = std::dynamic_pointer_cast<NodeOperationTernary>(node);
            
            if (allPathsReturn(ternaryNode->lhs) && allPathsReturn(ternaryNode->rhs)) {
                return true;
            }
        } else if (node->getType() == Node_StatementReturn) {
            return true;
        }
    
        return false;
    }
    
    bool CodeGenerator::allPathsReturn(std::vector<std::shared_ptr<Node>> nodes) {
        for (auto& statement : nodes) {
            if (allPathsReturn(statement)) {
                return true;
            }
        }
        
        return false;
    }
    
    bool CodeGenerator::isVariableSetAtPath(const std::string &identifier, std::shared_ptr<Node> currentPath,
        Node* breakPosition) {
        LOG("--")
        LOG("currentPath: ", currentPath->getTypeName())
    
        if (currentPath->getType() == Node_StatementFunctionCall) {
            LOG("pass currentPath")
        }
        
        if (currentPath->getType() != Node_StatementFunctionCall) {
            for (int i = 0; i < currentPath->children.size(); i++) {
                auto statement = currentPath->children[i];
    
                if (!statement) {
                    continue;
                }
    
                LOG("statement: ", statement->getTypeName())
                
                if (i == 0 &&
                    (currentPath->getType() == Node_StatementIf ||
                     currentPath->getType() == Node_StatementSwitch ||
                     currentPath->getType() == Node_StatementFor ||
                     currentPath->getType() == Node_StatementCase)) {
                    LOG("pass")
                    continue;
                }
        
                if (currentPath->getType() == Node_StatementIf) {
                    auto nodeIf = std::dynamic_pointer_cast<NodeStatementIf>(currentPath);
            
                    bool inIf = false;
                    bool inElse = false;
            
                    for (auto& ifStatement : nodeIf->ifBody) {
                        if (ifStatement.get() == breakPosition) {
                            inIf = true;
                        }
                    }
            
                    for (auto& elseStatement : nodeIf->elseBody) {
                        if (elseStatement.get() == breakPosition) {
                            inElse = true;
                        }
                    }
            
                    if ((inIf && i <= nodeIf->ifBody.size()) ||
                        (inElse && i >= nodeIf->ifBody.size())) {
                        break;
                    }
                } else if (currentPath->getType() == Node_StatementSwitch) {
                    auto switchNode = std::dynamic_pointer_cast<NodeStatementSwitch>(currentPath);
    
                    int y = 0;
                    int z = 0;
                    bool found = false;
    
                    for (auto& caseNode : switchNode->cases) {
                        auto casePtr = std::dynamic_pointer_cast<NodeStatementCase>(caseNode);
        
                        for (auto& caseStatement : casePtr->body) {
                            if (caseStatement.get() == breakPosition) {
                                found = true;
                                break;
                            }
                        }
        
                        if (found) {
                            break;
                        }
        
                        y++;
                    }
                }
        
                if (statement.get() == breakPosition) {
                    LOG("break")
                    break;
                } else if (statement->getType() == Node_StatementVariableDeclaration) {
                    auto nodeVariable = std::dynamic_pointer_cast<NodeStatementVariableDeclaration>(statement);
            
                    for (auto& variable : nodeVariable->variables) {
                        if (variable->identifier == identifier && variable->expression) {
                            return true;
                        }
                    }
                } else if (statement->getType() == Node_OperationBinary) {
                    auto nodeBinary = std::dynamic_pointer_cast<NodeOperationBinary>(statement);
                    if ((nodeBinary->operationType == ParserToken_OperatorEqual ||
                         nodeBinary->operationType == ParserToken_OperatorArithmeticPlusEqual ||
                         nodeBinary->operationType == ParserToken_OperatorArithmeticMinusEqual ||
                         nodeBinary->operationType == ParserToken_OperatorArithmeticMultiplyEqual ||
                         nodeBinary->operationType == ParserToken_OperatorArithmeticDivideEqual ||
                         nodeBinary->operationType == ParserToken_OperatorArithmeticRemainderEqual) &&
                        std::dynamic_pointer_cast<NodeStatementVariable>(nodeBinary->lhs)->identifier == identifier) {
                        return true;
                    }
                } else if (statement->getType() == Node_OperationUnary) {
                    auto nodeBinary = std::dynamic_pointer_cast<NodeOperationUnary>(statement);
            
                    if ((nodeBinary->operationType == ParserToken_OperatorUnaryArithmeticIncrement ||
                         nodeBinary->operationType == ParserToken_OperatorUnaryArithmeticDecrement) &&
                        std::dynamic_pointer_cast<NodeStatementVariable>(nodeBinary->expression)->identifier == identifier) {
                        return true;
                    }
                } else if (!statement->children.empty() && isVariableSetAtPath(identifier, statement, breakPosition)) {
                    return true;
                }
            }
        }
        
        if (currentPath->parent && currentPath->parent->getType() != Node_TopFile) {
            return isVariableSetAtPath(identifier, currentPath->parent, currentPath.get());
        }
        
        return false;
    }
    
    CodeGenerator &getCodeGenerator() {
        return CodeGenerator::getInstance();
    }

} /* Namespace Aryiele. */