#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP
#include <string>

namespace ivanova
{
  struct dataStruct
  {
    double key1;
    unsigned long long key2;
    std::string key3;
  };

  std::istream& operator>>(std::istream& in, dataStruct& ds);
  std::ostream& operator<<(std::ostream& out, const dataStruct& ds);
  bool compareDataStructs(const dataStruct& a, const dataStruct& b);
}

#endif
