#ifndef DATA_H
#define DATA_H
#include <iosfwd>
#include <string>
#include <array>

namespace ohantsev
{
  struct DataStruct
  {
    double key1;
    unsigned long long key2;
    std::string key3;
    bool operator<(const DataStruct& rhs) const noexcept;
    static constexpr std::size_t FIELDS_COUNT = 3;
    enum class KeyID
    {
      DBL = 1,
      ULL = 2,
      STR = 3
    };
  };

  struct MultDelimiterIO
  {
    const std::string& exp;
  };

  struct DoubleI
  {
    double& ref;
  };

  struct DoubleO
  {
    const double& ref;
  };

  struct UllI
  {
    unsigned long long& ref;
  };

  struct UllO
  {
    const unsigned long long& ref;
  };

  struct StringI
  {
    std::string& ref;
  };

  struct StringO
  {
    const std::string& ref;
  };

  struct LabelIO
  {
    DataStruct::KeyID& ID;
    std::array< bool, DataStruct::FIELDS_COUNT >& filled;
  };

  struct Suffix
  {
    const std::string& lowCase;
    const std::string& highCase;
  };

  struct KeyIO
  {
    DataStruct& data;
    std::array< bool, DataStruct::FIELDS_COUNT >& filled;
  };

  bool TypenameCheck(const std::string& scanned, const Suffix& suffix);
  std::istream& operator>>(std::istream& in, MultDelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, DoubleI&& dest);
  std::istream& operator>>(std::istream& in, UllI&& dest);
  std::istream& operator>>(std::istream& in, StringI&& dest);
  std::istream& operator>>(std::istream& in, LabelIO&& dest);
  std::istream& operator>>(std::istream& in, DataStruct& dest);
  std::istream& operator>>(std::istream& in, KeyIO&& dest);
  std::ostream& operator<<(std::ostream& out, DoubleO&& dest);
  std::ostream& operator<<(std::ostream& out, UllO&& dest);
  std::ostream& operator<<(std::ostream& out, StringO&& dest);
  std::ostream& operator<<(std::ostream& out, const DataStruct& dest);
}
#endif
