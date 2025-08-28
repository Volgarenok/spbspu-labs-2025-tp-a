#ifndef TARGET_HPP
#define TARGET_HPP

#include <iostream>
#include <string>

namespace lanovenko
{
  struct Target
  {
    std::string type;
    size_t distance;
    size_t height;
    size_t speed;
    std::string unique_code;
  };

  std::istream& operator>>(std::istream& in, Target& rhs);
  std::ostream& operator<<(std::ostream& out, const Target& rhs);
}

#endif
