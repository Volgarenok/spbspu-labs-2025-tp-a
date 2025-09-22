#include "Utils.h"

#include <algorithm>

namespace voronina
{
  char ToUpper::operator()(unsigned char c) const
  {
    return static_cast< char >(std::toupper(c));
  }

  std::string toUpper(std::string const& str)
  {
    std::string upper_str = str;
    std::transform(upper_str.begin(), upper_str.end(), upper_str.begin(), ToUpper{});
    return upper_str;
  }
}
