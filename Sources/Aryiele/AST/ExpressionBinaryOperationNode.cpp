#include <Aryiele/AST/ExpressionBinaryOperationNode.h>
#include <Aryiele/Parser/Parser.h>

namespace Aryiele
{

    ExpressionBinaryOperationNode::ExpressionBinaryOperationNode(ParserTokens operationType,
                                                                 std::shared_ptr<ExpressionNode> leftExpression,
                                                                 std::shared_ptr<ExpressionNode> rightExpression) :
            m_operationType(operationType),
            m_leftExpression(leftExpression),
            m_rightExpression(rightExpression)
    {

    }

    llvm::Value* ExpressionBinaryOperationNode::GenerateCode()
    {
        llvm::Value *leftValue = m_leftExpression->GenerateCode();
        llvm::Value *rightValue = m_rightExpression->GenerateCode();

        if (!leftValue || !rightValue)
            return nullptr;

        switch (m_operationType)
        {
            case ParserTokens_Operator_Arithmetic_Plus:
                return CodeGenerator::GetInstance()->Builder.CreateFAdd(leftValue, rightValue, "fadd");
            case ParserTokens_Operator_Arithmetic_Minus:
                return CodeGenerator::GetInstance()->Builder.CreateFSub(leftValue, rightValue, "fsyb");
            case ParserTokens_Operator_Arithmetic_Multiply:
                return CodeGenerator::GetInstance()->Builder.CreateFMul(leftValue, rightValue, "fmul");
            case ParserTokens_Operator_Arithmetic_Divide:
                return CodeGenerator::GetInstance()->Builder.CreateFDiv(leftValue, rightValue, "fdiv");
            default:
            {
                LOG_ERROR("Unknown binary operator: ", Parser::GetTokenName(m_operationType));

                return nullptr;
            }
        }
    };

    void ExpressionBinaryOperationNode::DumpInformations(std::shared_ptr<ParserInformation> parentNode)
    {
        auto node = std::make_shared<ParserInformation>(parentNode, "Binary Operation");
        auto operationType = std::make_shared<ParserInformation>(node, "Operation Type: " + Parser::GetTokenName(m_operationType));
        auto leftExpression = std::make_shared<ParserInformation>(node, "Left Expression:");
        auto rightExpression = std::make_shared<ParserInformation>(node, "Right Expression:");

        m_leftExpression->DumpInformations(leftExpression);
        m_rightExpression->DumpInformations(rightExpression);

        node->Children.emplace_back(operationType);
        node->Children.emplace_back(leftExpression);
        node->Children.emplace_back(rightExpression);

        parentNode->Children.emplace_back(node);
    }

} /* Namespace Aryiele. */