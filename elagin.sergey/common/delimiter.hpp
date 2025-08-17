#ifndef DELIMITER_HPP
#define DELIMITER_HPP

#include <iostream>

namespace elagin
{
    struct DelimiterIO
    {
        char exp;
    };

    std::istream &operator>>(std::istream &in, DelimiterIO &&dest);
}

#endif // DELIMITER_HPP