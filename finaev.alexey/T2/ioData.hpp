#ifndef OUTPUTDATA_HPP
#define OUTPUTDATA_HPP
#include "dataStruct.hpp"

namespace finaev
{
  std::istream& operator>>(std::istream& in, DataStruct& rhs);
  std::ostream& operator<<(std::ostream& out, const DataStruct& rhs);
}

#endif
