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

#include <Aryiele/AST/Nodes/NodeStatementFunctionCall.h>

namespace Aryiele {

    NodeStatementFunctionCall::NodeStatementFunctionCall(const std::string &identifier,
                                                           std::vector<std::shared_ptr<Node>> arguments) :
        Identifier(identifier), Arguments(arguments) {

    }

    void NodeStatementFunctionCall::DumpInformations(std::shared_ptr<ParserInformation> parentNode) {
        auto node = std::make_shared<ParserInformation>(parentNode, "Function Call");
        auto identifier = std::make_shared<ParserInformation>(node, "Identifier: " + Identifier);
        auto argumentsNode = std::make_shared<ParserInformation>(node, "Arguments:");

        auto i = 0;

        for(auto& argument : Arguments) {
            auto argumentNode = std::make_shared<ParserInformation>(argumentsNode, std::to_string(i));
            auto body = std::make_shared<ParserInformation>(argumentNode, "Body:");

            argument->DumpInformations(body);

            argumentNode->Children.emplace_back(body);
            argumentsNode->Children.emplace_back(argumentNode);

            i++;
        }

        node->Children.emplace_back(identifier);
        node->Children.emplace_back(argumentsNode);
        parentNode->Children.emplace_back(node);
    }
    
    NodeEnum NodeStatementFunctionCall::GetType() {
        return Node_StatementFunctionCall;
    }

} /* Namespace Aryiele. */
