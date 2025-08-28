#ifndef IO_HELPERS_H
#define IO_HELPERS_H
#include <iostream>
#include <cstddef>
#include <string>

namespace belyaev
{

  struct DoubleEIOIn
  {
    double& value;
  };

  struct DoubleEIOOut
  {
    double value;
  };

  struct PairLLIOIn
  {
    std::pair< long long, unsigned long long >& value;
  };

  struct PairLLIOOut
  {
    std::pair< long long, unsigned long long > value;
  };

  struct StringIO
  {
    std::string& value;
  };

  std::ostream& operator<<(std::ostream& out, const DoubleEIOOut&& dest);
  std::ostream& operator<<(std::ostream& out, const PairLLIOOut&& dest);
  std::istream& operator>>(std::istream& in, const DoubleEIOIn&& dest);
  std::istream& operator>>(std::istream& in, const PairLLIOIn&& dest);
  std::istream& operator>>(std::istream& in, const StringIO& dest);
}

#endif
