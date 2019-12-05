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

#include <Aryiele/AST/Nodes/NodeStatementSwitch.h>

namespace Aryiele {
    
    NodeStatementSwitch::NodeStatementSwitch(std::shared_ptr<Node> expression,
                                             std::vector<std::shared_ptr<Node>> casesExpression,
                                             std::vector<std::vector<std::shared_ptr<Node>>> casesBody,
                                             std::vector<std::shared_ptr<Node>> defaultBody) :
        expression(expression), casesExpression(casesExpression), casesBody(casesBody), defaultBody(defaultBody) {
        
    }
    
    void NodeStatementSwitch::dumpAST(std::shared_ptr<ParserInformation> parentNode) {
        auto node = std::make_shared<ParserInformation>(parentNode, "Switch");
        auto expressionNode = std::make_shared<ParserInformation>(node, "Expression:");
        auto casesNode = std::make_shared<ParserInformation>(node, "Cases:");
        auto defaultNode = std::make_shared<ParserInformation>(node, "Default:");
    
        expression->dumpAST(expressionNode);
        
        for (int i = 0; i < casesExpression.size(); i++) {
            auto caseNode = std::make_shared<ParserInformation>(casesNode, std::to_string(i) + ": ");
            auto caseExpressionNode = std::make_shared<ParserInformation>(caseNode, "Expression:");
            auto caseBodyNode = std::make_shared<ParserInformation>(caseNode, "Body:");
    
            casesExpression.at(i)->dumpAST(caseExpressionNode);
    
            for (auto& j : casesBody.at(i)) {
                j->dumpAST(caseBodyNode);
            }
    
            caseNode->children.emplace_back(caseExpressionNode);
            caseNode->children.emplace_back(caseBodyNode);
    
            casesNode->children.emplace_back(caseNode);
        }
    
        if (!defaultBody.empty()) {
            for (auto& i : defaultBody) {
                i->dumpAST(defaultNode);
            }
        }
        
        node->children.emplace_back(expressionNode);
        node->children.emplace_back(casesNode);
        
        if (!defaultBody.empty()) {
            node->children.emplace_back(defaultNode);
        }
        
        parentNode->children.emplace_back(node);
    }
    
    NodeEnum NodeStatementSwitch::getType() {
        return Node_StatementSwitch;
    }
} /* Namespace Aryiele. */