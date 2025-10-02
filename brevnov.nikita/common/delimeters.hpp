#ifndef DELIMETERS_HPP
#define DELIMETERS_HPP
#include <string>
namespace brevnov
{
  struct DelimeterIO
  {
    char exp;
  };

  struct DelimetersIO
  {
    const std::string& exp;
  };

  std::istream& operator>>(std::istream& input, DelimeterIO&& dest);
  std::istream& operator>>(std::istream& input, DelimetersIO&& dest);
}
#endif
