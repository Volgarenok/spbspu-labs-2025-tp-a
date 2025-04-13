#ifndef WRAPPERS_HPP
#define WRAPPERS_HPP

#include <string>

namespace pilugina
{
  struct DelimiterIO
  {
    char exp;
  };
  struct UnsignedLongLongOCT
  {
    unsigned long long &ref;
  };
  struct UnsignedLongLongBIN
  {
    unsigned long long &ref;
  };
  struct StringIO
  {
    std::string &ref;
  };

  std::istream &operator>>(std::istream &in, DelimiterIO &&dest);
  std::istream &operator>>(std::istream &in, UnsignedLongLongOCT &&dest);
  std::istream &operator>>(std::istream &in, UnsignedLongLongBIN &&dest);
  std::istream &operator>>(std::istream &in, StringIO &&dest);
} // namespace pilugina
#endif
