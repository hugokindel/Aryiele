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

#include <Aryiele/AST/Nodes/NodeOperationTernary.h>

namespace Aryiele {
    NodeOperationTernary::NodeOperationTernary(std::shared_ptr<Node> condition, std::shared_ptr<Node> lhs,
                                               std::shared_ptr<Node> rhs) :
        condition(condition), lhs(lhs), rhs(rhs) {
        
    }
    
    void NodeOperationTernary::dumpAST(std::shared_ptr<ParserInformation> parentNode) {
        auto node = std::make_shared<ParserInformation>(parentNode, "Ternary Operation");
        auto conditionNode = std::make_shared<ParserInformation>(node, "Condition:");
        auto lhsNode = std::make_shared<ParserInformation>(node, "LHS:");
        auto rhsNode = std::make_shared<ParserInformation>(node, "RHS:");
        
        condition->dumpAST(conditionNode);
        lhs->dumpAST(lhsNode);
        rhs->dumpAST(rhsNode);
        
        node->children.emplace_back(conditionNode);
        node->children.emplace_back(lhsNode);
        node->children.emplace_back(rhsNode);
        
        parentNode->children.emplace_back(node);
    }
    
    NodeEnum NodeOperationTernary::getType() {
        return Node_OperationTernary;
    }
    
} /* Namespace Aryiele. */
