//===----- Ariyele Compiler -----------------------------------------------===//
//
// This project is a tool of the Aryiele language. Its main purpose is to
// compile Aryiele to LLVM Intermediate Representation using the Aryiele
// library and Vanir. But it can also serve to translate this code directly
// to assembly for a specified architecture (using LLC), and also generate a
// native executable using a native assembler/linker (GCC, Clang, MSVC, ...).
//
//===----------------------------------------------------------------------===//

#include <arc/ARC.h>

int main(const int argc, char *argv[])
{
    auto arc = std::make_shared<ARC::ARC>();

    return arc->Run(argc, argv);
}