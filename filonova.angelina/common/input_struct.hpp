#ifndef INPUT_STRUCT_HPP
#define INPUT_STRUCT_HPP

#include <string>
#include <complex>

namespace filonova
{
  struct DelimiterValue
  {
    char exp;
  };
  std::istream &operator>>(std::istream &in, DelimiterValue &&dest);

  struct HexValue
  {
    unsigned long long &ref;
  };
  std::istream &operator>>(std::istream &in, HexValue &&dest);

  struct ComplexValue
  {
    std::complex< double > &ref;
  };
  std::istream &operator>>(std::istream &in, ComplexValue &&dest);

  struct StringValue
  {
    std::string &ref;
  };
  std::istream &operator>>(std::istream &in, StringValue &&dest);

}

#endif
