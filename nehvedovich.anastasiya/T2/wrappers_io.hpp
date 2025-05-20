#ifndef WRAPPERS_IO_HPP
#define WRAPPERS_IO_HPP

#include <string>

namespace nehvedovich
{
  struct DelimiterIO
  {
    char exp;
  };
  struct UnsignedLongLongIO
  {
    unsigned long long &ref;
  };
  struct CharIO
  {
    char &ref;
  };
  struct StringIO
  {
    std::string &ref;
  };

  std::istream &operator>>(std::istream &in, DelimiterIO &&dest);
  std::istream &operator>>(std::istream &in, UnsignedLongLongIO &&dest);
  std::istream &operator>>(std::istream &in, CharIO &&dest);
  std::istream &operator>>(std::istream &in, StringIO &&dest);
}
#endif
