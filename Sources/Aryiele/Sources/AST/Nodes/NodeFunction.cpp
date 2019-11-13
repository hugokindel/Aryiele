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

#include <Aryiele/AST/Nodes/NodeFunction.h>

namespace Aryiele {
    NodeFunction::NodeFunction(const std::string& identifier,
                               const std::string& type,
                               std::vector<Argument> arguments,
                               std::vector<std::shared_ptr<Node>> body) :
        Identifier(identifier), Type(type), Arguments(arguments), Body(body) {

    }

    void NodeFunction::DumpInformations(std::shared_ptr<ParserInformation> parentNode) {
        auto node = std::make_shared<ParserInformation>(parentNode, "Function");
        auto argumentsNode = std::make_shared<ParserInformation>(node, "Arguments:");
        auto valueNode = std::make_shared<ParserInformation>(node, "Body:");

        for (auto& childNode : Body)
            childNode->DumpInformations(valueNode);

        int i = 0;

        for(auto& argument : Arguments) {
            auto argumentNode = std::make_shared<ParserInformation>(argumentsNode, std::to_string(i));

            argumentNode->Children.emplace_back(std::make_shared<ParserInformation>(
                argumentNode, "Identifier: " + argument.Identifier));
            argumentNode->Children.emplace_back(std::make_shared<ParserInformation>(
                argumentNode, "Type: " + argument.Type));

            argumentsNode->Children.emplace_back(argumentNode);

            i++;
        }

        node->Children.emplace_back(std::make_shared<ParserInformation>(node, "Identifier: " + Identifier));
        node->Children.emplace_back(std::make_shared<ParserInformation>(node, "Type: " + Type));
        node->Children.emplace_back(argumentsNode);
        node->Children.emplace_back(valueNode);

        parentNode->Children.emplace_back(node);
    }

    NodeEnum NodeFunction::GetType() {
        return Node_FunctionPrototype;
    }

} /* Namespace Aryiele. */
