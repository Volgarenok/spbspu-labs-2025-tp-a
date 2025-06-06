#ifndef INPUT_OPERATORS_HPP
#define INPUT_OPERATORS_HPP

#include <string>
#include <istream>
#include <iomanip>

namespace cherepkov
{
  struct DelimiterIO
  {
    char exp;
  };

  struct UllLitValue
  {
    unsigned long long& ref;
  };

  struct UllBinValue
  {
    unsigned long long& ref;
  };

  struct StringValue
  {
    std::string& ref;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, UllLitValue&& dest);
  std::istream& operator>>(std::istream& in, UllBinValue&& dest);
  std::istream& operator>>(std::istream& in, StringValue&& dest);
  std::string ULLtoBinary(unsigned long long val);

}

#endif
