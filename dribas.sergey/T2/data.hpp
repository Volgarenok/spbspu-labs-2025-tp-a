#ifndef DATA_HPP
#define DATA_HPP

#include <string>

namespace dribas
{
  struct DataStruct
  {
    double key1;
    unsigned long long key2;
    std::string key3;
  };

  struct UllI
  {
    unsigned long long& ref;
  };

  struct DoubleI
  {
    double& ref;
  };

  struct StringI
  {
    std::string& ref;
  };

  struct UllO
  {
    const unsigned long long& ref;
  };

  struct DoubleO
  {
    const double& ref;
  };

  struct StringO
  {
    const std::string& ref;
  };

  struct KeyI
  {
    std::string& key;
    const std::string& expected;
    bool wasVerified = false;
  };

  std::istream& operator>>(std::istream&, DataStruct&);
  std::istream& operator>>(std::istream&, DoubleI&&);
  std::istream& operator>>(std::istream&, UllI&&);
  std::istream& operator>>(std::istream&, StringI&&);
  std::istream& operator>>(std::istream&, KeyI&&);

  std::ostream& operator<<(std::ostream&, const DoubleO&&);
  std::ostream& operator<<(std::ostream&, const UllO&&);
  std::ostream& operator<<(std::ostream&, const StringO&&);
  std::ostream& operator<<(std::ostream&, const DataStruct&);

  bool compare(const DataStruct&, const DataStruct&) noexcept;
}


#endif
