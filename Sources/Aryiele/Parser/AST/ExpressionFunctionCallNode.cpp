#include <Aryiele/Parser/AST/ExpressionFunctionCallNode.h>

namespace Aryiele
{

    ExpressionFunctionCallNode::ExpressionFunctionCallNode(const std::string &name,
                                                           std::vector<std::shared_ptr<ExpressionNode>> arguments) :
        m_name(name), m_arguments(arguments)
    {

    }

} /* Namespace Aryiele. */