#ifndef IO_HELPERS_H
#define IO_HELPERS_H
#include <iostream>
#include <cstddef>
#include <string>

namespace belyaev
{
  struct DelimeterIO
  {
    char expected;
    DelimeterIO(char expected_);
  };

  struct DoubleEIO
  {
    double& value;
  };

  struct PairLLIO
  {
    std::pair< long long, unsigned long long >& value;
  };

  struct StringIO
  {
    std::string& value;
  };

  std::istream& operator>>(std::istream& in, const DelimeterIO&& dest);
  std::ostream& operator<<(std::ostream& out, const DoubleEIO&& dest);
  std::ostream& operator<<(std::ostream& out, const PairLLIO& dest);
  std::istream& operator>>(std::istream& in, const DoubleEIO&& dest);
  std::istream& operator>>(std::istream& in, const PairLLIO&& dest);
  std::istream& operator>>(std::istream& in, const StringIO& dest);
}

#endif
