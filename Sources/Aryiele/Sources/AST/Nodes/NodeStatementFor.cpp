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

#include <Aryiele/AST/Nodes/NodeStatementFor.h>

namespace Aryiele {
    NodeStatementFor::NodeStatementFor(std::shared_ptr<Node> condition, std::vector<std::shared_ptr<Node>> body,
        std::shared_ptr<NodeStatementVariableDeclaration> variable, std::shared_ptr<Node> incrementalValue) :
        condition(condition), body(body), variable(variable), incrementalValue(incrementalValue) {
        
    }
    
    void NodeStatementFor::dumpInformations(std::shared_ptr<ParserInformation> parentNode) {
        auto node = std::make_shared<ParserInformation>(parentNode, "For");
        auto variableNode = std::make_shared<ParserInformation>(node, "Variable:");
        auto conditionNode = std::make_shared<ParserInformation>(node, "Condition:");
        auto incrementalValueNode = std::make_shared<ParserInformation>(node, "Incremental Value:");
        auto bodyNode = std::make_shared<ParserInformation>(node, "Body:");
    
        variable->dumpInformations(variableNode);
        condition->dumpInformations(conditionNode);
        
        for (auto& i : body) {
            i->dumpInformations(bodyNode);
        }
        
        if (incrementalValue != nullptr) {
            incrementalValue->dumpInformations(incrementalValueNode);
        }
    
        node->children.emplace_back(variableNode);
        node->children.emplace_back(conditionNode);
    
        if (incrementalValue != nullptr) {
            node->children.emplace_back(incrementalValueNode);
        }
        
        node->children.emplace_back(bodyNode);
    
        parentNode->children.emplace_back(node);
    }
    
    NodeEnum NodeStatementFor::getType() {
        return Node_StatementFor;
    }
} /* Namespace Aryiele. */