#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP
#include <cstddef>
#include <string>

namespace ivanova
{
  struct DoubleScienceT
  {
    double key;
  };

  struct UllBinT
  {
    unsigned long long key;
    std::size_t prefix_zeroes;
  };

  struct StringT
  {
    std::string key;
  };

  struct ExpectCharT
  {
    char ch;
  };

  struct dataStruct
  {
    double key1;
    unsigned long long key2;
    std::size_t key2_zeroes;
    std::string key3;

    bool operator<(const dataStruct& other);
  };

  std::ostream& operator<<(std::ostream& out, const DoubleScienceT& x);
  std::ostream& operator<<(std::ostream& out, const UllBinT& x);
  std::ostream& operator<<(std::ostream& out, const StringT& x);
  std::ostream& operator<<(std::ostream& out, const dataStruct& x);

  std::istream& operator>>(std::istream& in, ExpectCharT&& x);
  std::istream& operator>>(std::istream& in, DoubleScienceT& x);
  std::istream& operator>>(std::istream& in, UllBinT& x);
  std::istream& operator>>(std::istream& in, StringT& x);
  std::istream& operator>>(std::istream& in, dataStruct& ds);
}

#endif
