#include <Aryiele/AST/Nodes/NodeStatementIf.h>

namespace Aryiele
{
    NodeStatementIf::NodeStatementIf(std::shared_ptr<Node> condition,
                                       std::vector<std::shared_ptr<Node>> if_body,
                                       std::vector<std::shared_ptr<Node>> else_body) :
        m_condition(condition), m_if_body(if_body), m_else_body(else_body)
    {

    }

    llvm::Value *NodeStatementIf::GenerateCode()
    {
        llvm::Value *conditionValue = m_condition->GenerateCode();

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

        llvm::Value *ThenV = m_if_body[0]->GenerateCode(); // TODO: All
        if (!ThenV)
            return nullptr;

        CodeGenerator::GetInstance()->Builder.CreateBr(mergeBasicBlock);
        thenBasicBlock = CodeGenerator::GetInstance()->Builder.GetInsertBlock();

        function->getBasicBlockList().push_back(elseBasicBlock);
        CodeGenerator::GetInstance()->Builder.SetInsertPoint(elseBasicBlock);

        // TODO: Support for no else
        llvm::Value *ElseV = m_else_body[0]->GenerateCode(); // TODO: All
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

    void NodeStatementIf::DumpInformations(std::shared_ptr<ParserInformation> parentNode)
    {
        auto node = std::make_shared<ParserInformation>(parentNode, "If/Else");

        // -------------------------------------------------------------------------------------------------------------
        // IF
        // -------------------------------------------------------------------------------------------------------------
        auto ifNode = std::make_shared<ParserInformation>(node, "If");
        auto ifCondition = std::make_shared<ParserInformation>(ifNode, "Condition:");
        auto ifBody = std::make_shared<ParserInformation>(ifNode, "Body:");

        m_condition->DumpInformations(ifCondition);

        for (auto& i : m_if_body)
            i->DumpInformations(ifBody);

        ifNode->Children.emplace_back(ifCondition);
        ifNode->Children.emplace_back(ifBody);
        node->Children.emplace_back(ifNode);

        // -------------------------------------------------------------------------------------------------------------
        // ELSE
        // -------------------------------------------------------------------------------------------------------------
        if (!m_else_body.empty())
        {
            auto elseNode = std::make_shared<ParserInformation>(node, "Else");
            auto elseBody = std::make_shared<ParserInformation>(elseNode, "Body:");

            for (auto& i : m_else_body)
                i->DumpInformations(elseBody);

            elseNode->Children.emplace_back(elseBody);
            node->Children.emplace_back(elseNode);
        }

        parentNode->Children.emplace_back(node);
    }

} /* Namespace Aryiele. */