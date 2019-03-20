#ifndef ARYIELE_ARGUMENT_H
#define ARYIELE_ARGUMENT_H

#include <Aryiele/Core/Includes.h>

namespace Aryiele
{
    struct Argument
    {
        Argument(const std::string& identifier, const std::string& type);

        std::string Identifier;
        std::string Type;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_ARGUMENT_H. */
