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

#include <Aryiele/AST/Nodes/Node.h>

namespace Aryiele {
    Node::Node(std::vector<std::shared_ptr<Node>> children, std::shared_ptr<Node> parent) :
        children(children), parent(parent) {
        
    }
    
    std::string Node::getTypeName() {
        return getTypeName(getType());
    }
    
    std::string Node::getTypeName(NodeEnum nodeType) {
        switch (nodeType) {
            case Node_TopFunction:
                return "TopFunction";
            case Node_TopNamespace:
                return "TopNamespace";
            case Node_LiteralArray:
                return "LiteralArray";
            case Node_LiteralBoolean:
                return "LiteralBoolean";
            case Node_LiteralCharacter:
                return "LiteralCharacter";
            case Node_LiteralNumberFloating:
                return "LiteralNumberFloating";
            case Node_LiteralNumberInteger:
                return "LiteralNumberInteger";
            case Node_LiteralString:
                return "LiteralString";
            case Node_OperationUnary:
                return "OperationUnary";
            case Node_OperationBinary:
                return "OperationBinary";
            case Node_OperationTernary:
                return "OperationTernary";
            case Node_StatementArrayCall:
                return "StatementArrayCall";
            case Node_StatementBlock:
                return "StatementBlock";
            case Node_StatementBreak:
                return "StatementBreak";
            case Node_StatementContinue:
                return "StatementContinue";
            case Node_StatementFor:
                return "StatementFor";
            case Node_StatementFunctionCall:
                return "StatementFunctionCall";
            case Node_StatementIf:
                return "StatementIf";
            case Node_StatementReturn:
                return "StatementReturn";
            case Node_StatementSwitch:
                return "StatementSwitch";
            case Node_StatementVariable:
                return "StatementVariable";
            case Node_StatementVariableDeclaration:
                return "StatementVariableDeclaration";
            case Node_StatementWhile:
                return "StatementWhile";
                
            default:
                return "Error";
        }
    }
    
} /* Namespace Aryiele. */