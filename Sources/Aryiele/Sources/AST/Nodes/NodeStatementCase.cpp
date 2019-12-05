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

#include <Aryiele/AST/Nodes/NodeStatementCase.h>

namespace Aryiele {
    
    NodeStatementCase::NodeStatementCase(std::shared_ptr<Node> expression, std::vector<std::shared_ptr<Node>> body) :
        expression(expression), body(body) {
        children = std::vector<std::shared_ptr<Node>> {expression};
        children.insert(children.end(), body.begin(), body.end());
    }
    
    void NodeStatementCase::dumpAST(std::shared_ptr<ParserInformation> parentNode) {
        auto node = std::make_shared<ParserInformation>(parentNode, expression ? "Case" : "Default");
        std::shared_ptr<ParserInformation> expressionNode = nullptr;
        auto bodyNode = std::make_shared<ParserInformation>(node, "Body:");
        
        if (expression) {
            expressionNode = std::make_shared<ParserInformation>(node, "Expression:");
    
            expression->dumpAST(expressionNode);
    
            node->children.emplace_back(expressionNode);
        }
    
        for (auto& j : body) {
            j->dumpAST(bodyNode);
        }
    
        node->children.emplace_back(bodyNode);
    
        parentNode->children.emplace_back(node);
    }
    
    NodeEnum NodeStatementCase::getType() {
        return Node_StatementCase;
    }
} /* Namespace Aryiele. */