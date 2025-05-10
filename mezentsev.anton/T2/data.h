#ifndef DATA_H
#define DATA_H
#include <string>
#include <iostream>

namespace mezentsev
{
  struct Data
  {
    long long key1;
    char key2;
    std::string key3;
  };
  struct DelimiterIO
  {
    char exp;
  };
  struct DelimitersIO
  {
    std::string exp;
  };
  struct CharIO
  {
    char& def;
  };
  struct StringIO
  {
    std::string& def;
  };
  struct LongLongIO
  {
    long long& def;
  };
  std::istream& operator>>(std::istream& input, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& input, DelimitersIO&& dest);
  std::istream& operator>>(std::istream& input, LongLongIO&& dest);
  std::istream& operator>>(std::istream& input, CharIO&& dest);
  std::istream& operator>>(std::istream& input, StringIO&& dest);
  std::ostream& operator<<(std::ostream& output, const LongLongIO&& dest);
  std::ostream& operator<<(std::ostream& output, const CharIO&& dest);
  std::ostream& operator<<(std::ostream& output, const StringIO&& dest);
  std::istream& operator>>(std::istream& input, Data& dest);
  std::ostream& operator<<(std::ostream& output, const Data& dest);
  bool compare(const Data& lhs, const Data& rhs);
}
#endif
