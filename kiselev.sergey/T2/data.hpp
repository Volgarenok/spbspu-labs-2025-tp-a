#ifndef DATA_HPP
#define DATA_HPP
#include <iosfwd>
#include <string>
#include <array>

namespace kiselev
{
  struct DataStruct
  {
    unsigned long long key1;
    char key2;
    std::string key3;
  };
  std::istream& operator>>(std::istream& input, DataStruct& dest);
  std::ostream& operator<<(std::ostream& output, const DataStruct& dest);
  bool compare(const DataStruct& lhs, const DataStruct& rhs);

  struct UllIO
  {
    unsigned long long& ref;
  };

  struct CharIO
  {
    char& ref;
  };

  struct StringIO
  {
    std::string& ref;
  };

  struct KeyIO
  {
    DataStruct& data;
    std::array< int, 3 > keys;
  };
  std::istream& operator>>(std::istream& input, UllIO&& dest);
  std::istream& operator>>(std::istream& input, CharIO&& dest);
  std::istream& operator>>(std::istream& input, StringIO&& dest);
  std::istream& operator>>(std::istream& input, KeyIO& dest);
  std::ostream& operator<<(std::ostream& output, const UllIO&& dest);
  std::ostream& operator<<(std::ostream& output, const CharIO&& dest);
  std::ostream& operator<<(std::ostream& output, const StringIO&& dest);
}
#endif
