//==================================================================================//
//                                                                                  //
//  Copyright (c) 2019 Hugo Kindel <kindelhugo.pro@gmail.com>                      //
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

#include <Aryiele/AST/Nodes/NodeOperationUnary.h>
#include <Aryiele/Parser/Parser.h>

namespace Aryiele {
    NodeOperationUnary::NodeOperationUnary(ParserTokenEnum operationType, std::shared_ptr<Node> expression, bool left) :
        operationType(operationType), expression(expression), left(left) {
    
    }
    
    void NodeOperationUnary::dumpInformations(std::shared_ptr<ParserInformation> parentNode) {
        auto node = std::make_shared<ParserInformation>(parentNode, "Unary Operation");
        auto sideNode = std::make_shared<ParserInformation>(node, std::string("Side: ") + std::string(left ? "Left" : "Right"));
        auto operationTypeNode = std::make_shared<ParserInformation>(
            node, "Type: " + Parser::getTokenName(operationType));
        auto expressionNode = std::make_shared<ParserInformation>(node, "Operand:");
    
        expression->dumpInformations(expressionNode);
    
        node->children.emplace_back(sideNode);
        node->children.emplace_back(operationTypeNode);
        node->children.emplace_back(expressionNode);
    
        parentNode->children.emplace_back(node);
    }
    
    NodeEnum NodeOperationUnary::getType() {
        return Node_OperationUnary;
    }
} /* Namespace Aryiele. */