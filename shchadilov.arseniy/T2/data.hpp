#ifndef DATA_HPP
#define DATA_HPP

#include <iostream>
#include <string>
#include <array>
#include <delimiter.hpp>

namespace shchadilov
{
  struct DataStruct
  {
    double key1;
    unsigned long long key2;
    std::string key3;
  };

  struct DoubleI
  {
    double& ref;
  };

  struct UllIO
  {
    unsigned long long& ref;
  };

  struct StringI
  {
    std::string& ref;
  };

  struct DoubleO
  {
    const double& ref;
  };

  struct UllO
  {
    const unsigned long long& ref;
  };

  struct StringO
  {
    const std::string& ref;
  };

  struct KeyIO
  {
    KeyIO(DataStruct& data, std::array< bool, 3 > used = { false, false, false });
    DataStruct& data;
    std::array< bool, 3 > used;
  };

  struct WordI
  {
    const int length;
    const std::string& ref;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, DoubleI&& dest);
  std::istream& operator>>(std::istream& in, UllIO&& dest);
  std::istream& operator>>(std::istream& in, StringI&& dest);
  std::istream& operator>>(std::istream& in, KeyIO& dest);
  std::istream& operator>>(std::istream& in, DataStruct& dest);
  std::istream& operator>>(std::istream& in, WordI&&);
  std::ostream& operator<<(std::ostream& out, const DoubleO& dest);
  std::ostream& operator<<(std::ostream& out, const UllO& dest);
  std::ostream& operator<<(std::ostream& out, const StringO& dest);
  std::ostream& operator<<(std::ostream& out, const DataStruct& dest);

  bool compare(const DataStruct& lhs, const DataStruct& rhs);
}

#endif
