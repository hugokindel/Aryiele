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

#include <Aryiele/AST/Nodes/NodeStatementIf.h>

namespace Aryiele {
    NodeStatementIf::NodeStatementIf(std::shared_ptr<Node> condition,
                                     std::vector<std::shared_ptr<Node>> ifBody,
                                     std::vector<std::shared_ptr<Node>> elseBody,
                                     std::vector<std::vector<std::shared_ptr<Node>>> elseIfBody) :
        condition(condition), ifBody(ifBody), elseBody(elseBody), elseIfBody(elseIfBody) {

    }

    void NodeStatementIf::dumpInformations(std::shared_ptr<ParserInformation> parentNode) {
        auto node = std::make_shared<ParserInformation>(parentNode, "If/Else");
        auto ifNode = std::make_shared<ParserInformation>(node, "If");
        auto ifConditionNode = std::make_shared<ParserInformation>(ifNode, "Condition:");
        auto ifBodyNode = std::make_shared<ParserInformation>(ifNode, "Body:");

        condition->dumpInformations(ifConditionNode);

        for (auto& i : ifBody)
            i->dumpInformations(ifBodyNode);

        ifNode->children.emplace_back(ifConditionNode);
        ifNode->children.emplace_back(ifBodyNode);
        node->children.emplace_back(ifNode);

        for (auto elseIfBody : elseIfBody) {
            auto elseNode = std::make_shared<ParserInformation>(node, "Else If");
            auto elseBody = std::make_shared<ParserInformation>(elseNode, "Body:");

            for (auto& i : elseIfBody)
                i->dumpInformations(elseBody);

            elseNode->children.emplace_back(elseBody);
            node->children.emplace_back(elseNode);
        }

        if (!elseBody.empty()) {
            auto elseNode = std::make_shared<ParserInformation>(node, "Else");
            auto elseBodyNode = std::make_shared<ParserInformation>(elseNode, "Body:");

            for (auto& i : elseBody)
                i->dumpInformations(elseBodyNode);

            elseNode->children.emplace_back(elseBodyNode);
            node->children.emplace_back(elseNode);
        }

        parentNode->children.emplace_back(node);
    }
    
    NodeEnum NodeStatementIf::getType() {
        return Node_StatementIf;
    }

} /* Namespace Aryiele. */
