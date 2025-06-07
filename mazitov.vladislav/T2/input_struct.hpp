#ifndef INPUT_STRUCT_HPP
#define INPUT_STRUCT_HPP

#include <string>

namespace mazitov
{
  struct DelimiterIO
  {
    char exp;
  };

  struct UnsignedLongLongBinIO
  {
    unsigned long long& ref;
  };

  struct DoubleIO
  {
    double& ref;
  };

  struct StringIO
  {
    std::string& ref;
  };
  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, DoubleIO&& dest);
  std::istream& operator>>(std::istream& in, UnsignedLongLongBinIO&& dest);
  std::istream& operator>>(std::istream& in, StringIO&& dest);
}

#endif
