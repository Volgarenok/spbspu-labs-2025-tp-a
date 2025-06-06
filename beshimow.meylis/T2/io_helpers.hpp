#ifndef IO_HELPERS_HPP
#define IO_HELPERS_HPP

#include <iostream>
#include <string>
#include <utility>

namespace beshimow {
    struct DelimeterIO { char exp; };
    struct DoubleEIOIn { double& ref; };
    struct DoubleEIOOut { const double& ref; };
    struct PairLLIOIn { std::pair<long long, long long>& ref; };
    struct PairLLIOOut { const std::pair<long long, long long>& ref; };
    struct StringIO { std::string& ref; };

    std::istream& operator>>(std::istream& in, DelimeterIO&& dest);
    std::istream& operator>>(std::istream& in, DoubleEIOIn&& dest);
    std::istream& operator>>(std::istream& in, PairLLIOIn&& dest);
    std::istream& operator>>(std::istream& in, StringIO&& dest);

    std::ostream& operator<<(std::ostream& out, DelimeterIO&& dest);
    std::ostream& operator<<(std::ostream& out, DoubleEIOOut&& dest);
    std::ostream& operator<<(std::ostream& out, PairLLIOOut&& dest);
}

#endif
