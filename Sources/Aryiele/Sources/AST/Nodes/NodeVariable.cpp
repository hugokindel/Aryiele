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

#include <Aryiele/AST/Nodes/NodeVariable.h>

namespace Aryiele {
    NodeVariable::NodeVariable(const std::string& identifier, std::shared_ptr<Node> subExpression) :
            NodeParentIdentifier(identifier), subExpression(subExpression) {

    }

    void NodeVariable::dumpInformations(std::shared_ptr<ParserInformation> parentNode) {
        auto node = std::make_shared<ParserInformation>(parentNode, "Variable");
        auto bodyNode = std::make_shared<ParserInformation>(node, "Identifier: " + identifier);
        auto subExpressionNode = std::make_shared<ParserInformation>(node, "Subexpression:");

        node->children.emplace_back(bodyNode);
        
        if (subExpression) {
            subExpression->dumpInformations(subExpressionNode);
            node->children.emplace_back(subExpressionNode);
        }
        
        parentNode->children.emplace_back(node);
    }
    
    NodeEnum NodeVariable::getType() {
        return Node_Variable;
    }

} /* Namespace Aryiele. */
