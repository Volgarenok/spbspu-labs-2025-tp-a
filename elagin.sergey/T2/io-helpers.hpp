#ifndef IO_HELPERS_HPP
#define IO_HELPERS_HPP

#include <complex>
#include <iostream>
#include <string>

namespace elagin {
struct HexIOIn {
  unsigned long long &ref;
};

struct HexIOOut {
  unsigned long long val;
};

struct ComplexIOIn {
  std::complex<double> &ref;
};

struct ComplexIOOut {
  std::complex<double> val;
};

struct StringIO {
  std::string &ref;
};

std::istream &operator>>(std::istream &in, const HexIOIn &&dest);
std::istream &operator>>(std::istream &in, const ComplexIOIn &&dest);
std::istream &operator>>(std::istream &in, const StringIO &dest);

std::ostream &operator<<(std::ostream &out, const HexIOOut &dest);
std::ostream &operator<<(std::ostream &out, const ComplexIOOut &dest);
} // namespace elagin

#endif