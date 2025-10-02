#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace voronina
{
  struct ToUpper
  {
    char operator()(unsigned char c) const;
  };

  std::string toUpper(const std::string& str);
}

#endif
