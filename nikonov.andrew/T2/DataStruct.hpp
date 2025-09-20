#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP
#include <string>
#include <istream>
namespace nikonov
{
  struct DataStruct
  {
    double key1;
    unsigned long long key2;
    std::string key3;
  };
  std::istream& operator>>(std::istream& out, DataStruct& dest);
  std::ostream& operator<<(std::ostream& out, const DataStruct& dest);
}
#endif
