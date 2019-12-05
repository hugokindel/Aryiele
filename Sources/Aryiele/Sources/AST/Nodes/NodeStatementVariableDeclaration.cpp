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

#include <Aryiele/AST/Nodes/NodeStatementVariableDeclaration.h>

namespace Aryiele {
    NodeStatementVariableDeclaration::NodeStatementVariableDeclaration(
            std::vector<std::shared_ptr<Variable>> variables) :
        variables(variables) {

    }

    void NodeStatementVariableDeclaration::dumpAST(std::shared_ptr<ParserInformation> parentNode) {
        auto node = std::make_shared<ParserInformation>(parentNode, "Variable Declaration");
        auto constantNode = std::make_shared<ParserInformation>(parentNode,
            std::string("Constant") + std::string(variables.size() > 1 ? "s" : "") +
            std::string(": ") + std::string(variables.at(0)->constant ? "true" : "false"));
    
        node->children.emplace_back(constantNode);
        
        auto i = 0;

        for (auto &variable : variables) {
            auto variableNode = std::make_shared<ParserInformation>(node, std::to_string(i));

            variableNode->children.emplace_back(std::make_shared<ParserInformation>(
                    variableNode, "Identifier: " + variable->identifier));
            
            if (!variable->type.empty()) {
                variableNode->children.emplace_back(std::make_shared<ParserInformation>(variableNode, "Type: " + variable->type));
            }

            if (variable->expression) {
                auto valueNode = std::make_shared<ParserInformation>(variableNode, "Expression:");
                variable->expression->dumpAST(valueNode);
                variableNode->children.emplace_back(valueNode);
            }

            node->children.emplace_back(variableNode);

            i++;
        }

        parentNode->children.emplace_back(node);
    }
    
    NodeEnum NodeStatementVariableDeclaration::getType() {
        return Node_StatementVariableDeclaration;
    }

} /* Namespace Aryiele. */