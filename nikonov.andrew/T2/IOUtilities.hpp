#ifndef IOUTILITIES_HPP
#define IOUTILITIES_HPP
#include <string>
#include <istream>
#include "DataStruct.hpp"
namespace nikonov
{
  struct DoubleI
  {
    double& ref;
  };
  struct UnsignedLongLongI
  {
    unsigned long long& ref;
  };
  struct StringI
  {
    std::string& ref;
  };
  struct LabelI
  {
    std::string& ref;
  };

  struct DoubleO
  {
    const double& ref;
  };
  struct UnsignedLongLongO
  {
    const unsigned long long& ref;
  };
  struct StringO
  {
    const std::string& ref;
  };

  std::istream& operator>>(std::istream& in, DoubleI&& dest);
  std::istream& operator>>(std::istream& in, UnsignedLongLongI&& dest);
  std::istream& operator>>(std::istream& in, StringI&& dest);
  std::istream& operator>>(std::istream& in, LabelI&& dest);

  std::ostream& operator<<(std::ostream& out, const DoubleO& dest);
  std::ostream& operator<<(std::ostream& out, const UnsignedLongLongO& dest);
  std::ostream& operator<<(std::ostream& out, const StringO& dest);
}
#endif
