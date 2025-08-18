#ifndef IO_HELPERS_HPP
#define IO_HELPERS_HPP

#include <complex>
#include <iostream>
#include <string>

namespace elagin
{
  struct HexIO
  {
    unsigned long long& value;
  };

  struct ComplexIO
  {
    std::complex<double>& value;
  };

  struct StringIO
  {
    std::string& value;
  };

  std::istream& operator>>(std::istream& in, const HexIO&& dest);
  std::istream& operator>>(std::istream& in, const ComplexIO&& dest);
  std::istream& operator>>(std::istream& in, const StringIO& dest);

  std::ostream& operator<<(std::ostream& out, const HexIO& dest);
  std::ostream& operator<<(std::ostream& out, const ComplexIO& dest);
}

#endif
