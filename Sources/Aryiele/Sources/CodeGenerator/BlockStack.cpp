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

#include <Aryiele/CodeGenerator/BlockStack.h>

namespace Aryiele {
    std::shared_ptr<Block> BlockStack::Create(bool setAsCurrent) {
        auto block = std::make_shared<Block>();

        if (Current) {
            block->Parent = Current;
            Current->Children.emplace_back(block);
        }

        if (setAsCurrent)
            Current = block;

        return Current;
    }

    std::shared_ptr<Block> BlockStack::EscapeCurrent() {
        if (Current->Parent)
            Current = Current->Parent;
        else
            Current = nullptr;

        return Current;
    }

    llvm::AllocaInst* BlockStack::FindVariable(const std::string& identifier) {
        auto block = Current;
        llvm::AllocaInst* variable = nullptr;

        if (block->Variables.find(identifier) != block->Variables.end())
            variable = Current->Variables[identifier];

        while (!variable) {
            block = block->Parent;

            if (!block)
                break;

            if (block->Variables.find(identifier) != block->Variables.end())
                variable = block->Variables[identifier];
        }

        return variable;
    }

} /* Namespace Aryiele. */