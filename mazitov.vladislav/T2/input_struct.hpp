#ifndef INPUT_STRUCT_HPP
#define INPUT_STRUCT_HPP

#include <string>

namespace mazitov
{
  struct DelimiterInput
  {
    char exp;
  };

  struct UnsignedLongLongBinInput
  {
    unsigned long long &ref;
  };

  struct DoubleInput
  {
    double &ref;
  };

  struct StringInput
  {
    std::string &ref;
  };
  std::istream &operator>>(std::istream &in, DelimiterInput &&dest);
  std::istream &operator>>(std::istream &in, DoubleInput &&dest);
  std::istream &operator>>(std::istream &in, UnsignedLongLongBinInput &&dest);
  std::istream &operator>>(std::istream &in, StringInput &&dest);
}

#endif
