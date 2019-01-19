#include <Aryiele/Parser/ParserInformation.h>

namespace Aryiele
{
    ParserInformation::ParserInformation(const std::shared_ptr<ParserInformation> parent, const std::string name,
                                         const std::vector<std::shared_ptr<ParserInformation>> children) :
            Parent(parent), Name(name), Children(children)
    {

    }

} /* Namespace Aryiele. */