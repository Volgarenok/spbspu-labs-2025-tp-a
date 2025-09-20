#ifndef PANTSIR_HPP
#define PANTSIR_HPP

#include <iostream>

namespace lanovenko
{
  struct Pantsir
  {
    size_t missiles;
    size_t ammunition;
    size_t barell_wear;
    size_t opening_hours;
    size_t targets_destroyed;
  };

  std::istream& operator>>(std::istream& in, Pantsir& rhs);
  std::ostream& operator<<(std::ostream& out, const Pantsir& rhs);
}

#endif
