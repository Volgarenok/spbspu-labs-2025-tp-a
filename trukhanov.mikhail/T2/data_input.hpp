#ifndef DATA_INPUT_HPP
#define DATA_INPUT_HPP

#include <iostream>
#include <string>

namespace trukhanov
{
  struct DelimiterIO
  {
    char exp;
  };

  struct LabelIO
  {
    std::string exp;
  };

  struct OctalULLIO
  {
    unsigned long long& ref;
  };

  struct RationalIO
  {
    std::pair<long long, unsigned long long >& ref;
  };

  struct StringIO
  {
    std::string& ref;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, LabelIO&& dest);
  std::istream& operator>>(std::istream& in, OctalULLIO&& dest);
  std::istream& operator>>(std::istream& in, RationalIO&& dest);
  std::istream& operator>>(std::istream& in, StringIO&& dest);
}

#endif
