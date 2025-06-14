#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include <iostream>

namespace fedorova
{
  struct DataStruct
  {
    unsigned long long key1;
    unsigned long long key2;
    bool key2_has_leading_zero = false;
    std::string key3;
  };

  struct DelimiterIO
  {
    char exp;
  };

  struct ULLLiteralIO
  {
    unsigned long long& ref;
    char suffix[3];
  };

  struct ULLBinaryIO
  {
    unsigned long long& ref;
    bool& has_leading_zero;
    char prefix[2];
  };

  struct StringIO
  {
    std::string& ref;
  };

  struct LabelIO
  {
    std::string exp;
  };

  class IoGuard
  {
  public:
    explicit IoGuard(std::basic_ios<char>& s);
    ~IoGuard();
  private:
    std::basic_ios<char>& s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios<char>::fmtflags fmt_;
  };

  bool compareDataStruct(const DataStruct& a, const DataStruct& b);
  std::istream& operator>>(std::istream& is, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& is, ULLLiteralIO&& dest);
  std::istream& operator>>(std::istream& is, ULLBinaryIO&& dest);
  std::istream& operator>>(std::istream& is, StringIO&& dest);
  std::istream& operator>>(std::istream& is, LabelIO&& dest);
  std::istream& operator>>(std::istream& is, DataStruct& data);
  std::ostream& operator<<(std::ostream& os, const DataStruct& data);
};

#endif
