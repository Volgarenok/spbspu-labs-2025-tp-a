#ifndef DATA_HPP
#define DATA_HPP
#include <string>

namespace smirnov
{
  struct DataStruct
  {
    double key1;
    unsigned long long key2;
    std::string key3;
  };

  std::istream& operator>>(std::istream& in, DataStruct& value);
  std::ostream& operator<<(std::ostream& out, const DataStruct& value);

  bool compare(const DataStruct& a, const DataStruct& b);

  struct DoubleIO
  {
    double& ref;
  };

  struct UllIO
  {
    unsigned long long& ref;
  };

  struct StringIO
  {
    std::string& ref;
  };

  struct KeyNumIO
  {
    const std::string exp;
  };

  std::istream& operator>>(std::istream& in, DoubleIO&& dest);
  std::istream& operator>>(std::istream& in, UllIO&& dest);
  std::istream& operator>>(std::istream& in, StringIO&& dest);
  std::istream& operator>>(std::istream& in, KeyNumIO&& dest);
  std::ostream& operator<<(std::ostream& out, const DoubleIO& dest);
  std::ostream& operator<<(std::ostream& out, const UllIO& dest);

}
#endif
