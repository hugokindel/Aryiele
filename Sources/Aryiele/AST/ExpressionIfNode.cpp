#include "ExpressionIfNode.h"

namespace Aryiele
{
    ExpressionIfNode::ExpressionIfNode(std::shared_ptr<ExpressionNode> condition,
                                       std::shared_ptr<ExpressionNode> if_body,
                                       std::shared_ptr<ExpressionNode> else_body) :
        m_condition(condition), m_if_body(if_body), m_else_body(else_body)
    {

    }

} /* Namespace Aryiele. */