#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP
#include <string>
#include <vector>

namespace karnauhova
{
  struct DataStruct
  {
    double key1;
    unsigned long long key2;
    std::string key3;
  };

  struct UllIO
  {
    unsigned long long& ref;
  };

  struct DoubleIO
  {
    double& ref;
  };

  struct StringIO
  {
    std::string& ref;
  };

  struct LabelIO
  {
    std::string exp;
  };
  struct KeyIO
  {
    int num;
  };

  std::ostream& operator<<(std::ostream& out, const DataStruct& dest);
  std::istream& operator>>(std::istream& in, DataStruct& dest);
  std::istream& operator>>(std::istream& in, DoubleIO&& dest);
  std::istream& operator>>(std::istream& in, KeyIO& dest);
  std::ostream& operator<<(std::ostream& out, const DoubleIO&& dest);
  std::istream& operator>>(std::istream& in, UllIO&& dest);
  std::ostream& operator<<(std::ostream& out, const UllIO&& dest);
  std::istream& operator>>(std::istream& in, StringIO&& dest);
  std::istream& operator>>(std::istream& in, LabelIO& dest);
  bool compare(const DataStruct& l, const DataStruct& r);
}
#endif
