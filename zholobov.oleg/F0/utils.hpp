#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>

#include "commands.hpp"

namespace zholobov {

  std::istream& operator>>(std::istream& in, zholobov::Words& words);
  std::istream& operator>>(std::istream& in, zholobov::Dictionary& dict);
  std::ostream& operator<<(std::ostream& out, const zholobov::Words& words);
  std::ostream& operator<<(std::ostream& out, const zholobov::Dictionary& dict);

}

#endif
