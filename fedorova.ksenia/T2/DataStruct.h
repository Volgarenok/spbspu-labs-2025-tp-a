#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include <iostream>
#include <delimiter.h>

namespace fedorova
{
  struct DataStruct
  {
    unsigned long long key1;
    unsigned long long key2;
    std::string key3;
  };

  struct ULLLiteralIO
  {
    unsigned long long& ref;
  };

  struct ULLBinaryI
  {
    unsigned long long& ref;
  };

  struct ULLBinaryO
  {
    const unsigned long long& ref;
  };

  struct StringIO
  {
    std::string& ref;
  };

  struct LabelIO
  {
    std::string exp;
  };

  bool compareDataStruct(const DataStruct& a, const DataStruct& b);
  std::istream& operator>>(std::istream& is, ULLLiteralIO&& dest);
  std::istream& operator>>(std::istream& is, ULLBinaryI&& dest);
  std::istream& operator>>(std::istream& is, StringIO&& dest);
  std::istream& operator>>(std::istream& is, LabelIO&& dest);
  std::istream& operator>>(std::istream& is, DataStruct& data);
  std::ostream& operator<<(std::ostream& os, const DataStruct& data);
  std::ostream& operator<<(std::ostream& os, const ULLBinaryO& dest);
};

#endif
