#ifndef WRAPPERS_HPP
#define WRAPPERS_HPP

#include <string>

namespace milchevskiy {

  struct DelimiterIO
  {
    char exp;
  };

  struct LITvalue
  {
    unsigned long long& ref;
  };

  struct OCTvalue
  {
    unsigned long long& ref;
  };

  struct STRvalue
  {
    std::string& ref;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, LITvalue&& dest);
  std::istream& operator>>(std::istream& in, OCTvalue&& dest);
  std::istream& operator>>(std::istream& in, STRvalue&& dest);

} // namespace milchevskiy

#endif

