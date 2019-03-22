#include <Aryiele/CodeGenerator/Block.h>

namespace Aryiele
{
    Block::Block(std::shared_ptr<Block> parent, std::map<std::string, std::shared_ptr<Variable>> variables,
                 std::vector<std::shared_ptr<Block>> children) :
        Parent(parent), Variables(variables), Children(children)
    {

    }

} /* Namespace Aryiele. */