#ifndef ARYIELE_PARSERDUMPTREENODE_H
#define ARYIELE_PARSERDUMPTREENODE_H

#include <string>
#include <vector>
#include <memory>

namespace Aryiele
{
    struct ParserInformation
    {
        explicit ParserInformation(std::shared_ptr<ParserInformation> parent = nullptr,
                std::string name = "Unnamed",
                std::vector<std::shared_ptr<ParserInformation>> children = std::vector<std::shared_ptr<ParserInformation>>());

        std::shared_ptr<ParserInformation> Parent;
        std::string Name;
        std::vector<std::shared_ptr<ParserInformation>> Children;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_PARSERDUMPTREENODE_H. */