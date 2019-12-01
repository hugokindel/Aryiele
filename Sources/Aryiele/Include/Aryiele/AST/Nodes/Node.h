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

#ifndef ARYIELE_AST_NODES_NODE_H
#define ARYIELE_AST_NODES_NODE_H

#include <Aryiele/Common.h>
#include <Aryiele/Parser/ParserInformation.h>

namespace Aryiele {
    enum NodeEnum {
        Node_TopFunction,
        Node_TopNamespace,
    
        Node_LiteralArray,
        Node_LiteralBoolean,
        Node_LiteralCharacter,
        Node_LiteralNumberFloating,
        Node_LiteralNumberInteger,
        Node_LiteralString,
        
        Node_OperationBinary,
        Node_OperationUnary,
    
        Node_StatementArrayCall,
        Node_StatementBlock,
        Node_StatementBreak,
        Node_StatementContinue,
        Node_StatementFor,
        Node_StatementFunctionCall,
        Node_StatementIf,
        Node_StatementReturn,
        Node_StatementSwitch,
        Node_StatementVariable,
        Node_StatementVariableDeclaration,
        Node_StatementWhile
    };
    
    struct Node {
        virtual void dumpInformations(std::shared_ptr<ParserInformation> parentNode) = 0;
        virtual NodeEnum getType() = 0;
        virtual std::string getTypeName();
        
        static std::string getTypeName(NodeEnum nodeType);
    };
    
} /* Namespace Aryiele. */

#endif /* ARYIELE_AST_NODES_NODE_H. */
