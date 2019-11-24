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

#include <Aryiele/AST/Nodes/NodeTopFunction.h>

namespace Aryiele {
    NodeTopFunction::NodeTopFunction(const std::string& identifier,
                                     const std::string& type,
                                     std::vector<Argument> arguments,
                                     std::vector<std::shared_ptr<Node>> body) :
        identifier(identifier), type(type), arguments(arguments), body(body) {

    }

    void NodeTopFunction::dumpInformations(std::shared_ptr<ParserInformation> parentNode) {
        auto node = std::make_shared<ParserInformation>(parentNode, "Function");
        auto argumentsNode = std::make_shared<ParserInformation>(node, "Arguments:");
        auto valueNode = std::make_shared<ParserInformation>(node, "Body:");

        for (auto& childNode : body)
            childNode->dumpInformations(valueNode);

        int i = 0;

        for(auto& argument : arguments) {
            auto argumentNode = std::make_shared<ParserInformation>(argumentsNode, std::to_string(i));

            argumentNode->children.emplace_back(std::make_shared<ParserInformation>(
                argumentNode, "Identifier: " + argument.identifier));
            argumentNode->children.emplace_back(std::make_shared<ParserInformation>(
                argumentNode, "Type: " + argument.type));

            argumentsNode->children.emplace_back(argumentNode);

            i++;
        }

        node->children.emplace_back(std::make_shared<ParserInformation>(node, "Identifier: " + identifier));
        node->children.emplace_back(std::make_shared<ParserInformation>(node, "Type: " + type));
        
        if (!arguments.empty()) {
            node->children.emplace_back(argumentsNode);
        }
        
        node->children.emplace_back(valueNode);

        parentNode->children.emplace_back(node);
    }

    NodeEnum NodeTopFunction::getType() {
        return Node_TopFunction;
    }

} /* Namespace Aryiele. */
