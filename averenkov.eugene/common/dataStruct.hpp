#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP

#include <iostream>
#include <string>
#include <utility>
#include "dataIO.hpp"

namespace averenkov
{
  struct DataStruct
  {
    char key1;
    std::pair< long long, unsigned long long > key2;
    std::string key3;
  };

  std::istream& operator>>(std::istream& in, DataStruct& dest);
  std::ostream& operator<<(std::ostream& out, const DataStruct& dest);
  bool operator<(const DataStruct& a, const DataStruct& b);
}

#endif
