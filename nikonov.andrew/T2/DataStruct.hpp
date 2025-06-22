#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP
#include <string>
#include <istream>
namespace nikonov
{
  struct DataStruct
  {
    double key1_;
    unsigned long long key2_;
    std::string key3_;
  };
  std::istream& operator>>(std::istream& out, DataStruct& dest);
  std::ostream& operator<<(std::ostream& out, const DataStruct& dest);
}
#endif
