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

#include <Aryiele/AST/Nodes/NodeOperationBinary.h>

namespace Aryiele {
    NodeOperationBinary::NodeOperationBinary(ParserTokenEnum operationType,
        std::shared_ptr<Node> lhs, std::shared_ptr<Node> rhs) :
        operationType(operationType), lhs(lhs), rhs(rhs) {
        children = std::vector<std::shared_ptr<Node>> {lhs, rhs};
    }

    void NodeOperationBinary::dumpAST(std::shared_ptr<ParserInformation> parentNode) {
        auto node = std::make_shared<ParserInformation>(parentNode, "Binary Operation");
        auto operationTypeNode = std::make_shared<ParserInformation>(
            node, "Type: " + ParserToken::getTypeName(operationType));
        auto lhsNode = std::make_shared<ParserInformation>(node, "LHS:");
        auto rhsNode = std::make_shared<ParserInformation>(node, "RHS:");
    
        lhs->dumpAST(lhsNode);
        rhs->dumpAST(rhsNode);

        node->children.emplace_back(operationTypeNode);
        node->children.emplace_back(lhsNode);
        node->children.emplace_back(rhsNode);

        parentNode->children.emplace_back(node);
    }
    
    NodeEnum NodeOperationBinary::getType() {
        return Node_OperationBinary;
    }

} /* Namespace Aryiele. */
