#ifndef IO_HELPERS_HPP
#define IO_HELPERS_HPP

#include <complex>
#include <iostream>
#include <string>

namespace elagin
{
    struct HexIOIn
    {
        unsigned long long &value;
    };

    struct HexIOOut
    {
        unsigned long long value;
    };

    struct ComplexIOIn
    {
        std::complex<double> &value;
    };

    struct ComplexIOOut
    {
        std::complex<double> value;
    };

    struct StringIO
    {
        std::string &value;
    };

    std::istream &operator>>(std::istream &in, const HexIOIn &&dest);
    std::istream &operator>>(std::istream &in, const ComplexIOIn &&dest);
    std::istream &operator>>(std::istream &in, const StringIO &dest);

    std::ostream &operator<<(std::ostream &out, const HexIOOut &dest);
    std::ostream &operator<<(std::ostream &out, const ComplexIOOut &dest);
}


#endif //IO_HELPERS_HPP
