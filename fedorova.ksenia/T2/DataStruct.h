#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <iterator>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <bitset>

namespace fedorova
{
  struct DataStruct
  {
    unsigned long long key1;
    std::string key2;
    std::string key3;
  };

  struct DelimiterIO
  {
    char exp;
  };

  struct ULLLiteralIO
  {
    unsigned long long& ref;
    std::string suffix;
  };

  struct ULLBinaryIO
  {
    std::string& ref;
  };

  struct StringIO
  {
    std::string& ref;
  };

  struct LabelIO
  {
    std::string exp;
  };

  class iofmtguard {
  public:
    explicit iofmtguard(std::basic_ios<char>& s);
    ~iofmtguard();
  private:
    std::basic_ios<char>& s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios<char>::fmtflags fmt_;
  };

  std::istream& operator>>(std::istream& is, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& is, ULLLiteralIO&& dest);
  std::istream& operator>>(std::istream& is, ULLBinaryIO&& dest);
  std::istream& operator>>(std::istream& is, StringIO&& dest);
  std::istream& operator>>(std::istream& is, LabelIO&& dest);
  std::istream& operator>>(std::istream& is, DataStruct& data);
  std::ostream& operator<<(std::ostream& os, const DataStruct& data);
};
#endif // !DATA_STRUCT_H

