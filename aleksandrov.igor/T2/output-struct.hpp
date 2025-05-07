#ifndef OUTPUT_STRUCT_HPP
#define OUTPUT_STRUCT_HPP

#include "data-struct.hpp"

namespace aleksandrov
{
  struct DoubleO
  {
    const double& ref;
  };

  struct RationalO
  {
    const std::pair< long long, unsigned long long >& ref;
  };

  struct StringO
  {
    const std::string& ref;
  };

  std::ostream& operator<<(std::ostream&, const DoubleO&);
  std::ostream& operator<<(std::ostream&, const RationalO&);
  std::ostream& operator<<(std::ostream&, const StringO&);
  std::ostream& operator<<(std::ostream&, const DataStruct&);
}

#endif

