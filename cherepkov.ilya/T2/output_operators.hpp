#ifndef OUTPUT_OPERATORS_HPP
#define OUTPUT_OPERATORS_HPP

#include <iostream>
#include <string>
#include <algorithm>

namespace cherepkov
{
  struct UllLitOutput
  {
    const unsigned long long& ref;
  };

  struct UllBinOutput
  {
    const unsigned long long& ref;
  };

  std::ostream& operator<<(std::ostream& out, const UllLitOutput& dest);
  std::ostream& operator<<(std::ostream& out, const UllBinOutput& dest);
}

#endif
