#ifndef WRAPPERS_HPP
#define WRAPPERS_HPP

#include <string>

namespace pilugina
{
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

  std::istream &operator>>(std::istream &in, UnsignedLongLongOCT &&dest);
  std::istream &operator>>(std::istream &in, UnsignedLongLongBIN &&dest);
  std::istream &operator>>(std::istream &in, StringIO &&dest);

  namespace output
  {
    struct UnsignedLongLongOCT
    {
      const unsigned long long &ref;
    };
    struct UnsignedLongLongBIN
    {
      const unsigned long long &ref;
    };
    std::ostream &operator<<(std::ostream &in, const UnsignedLongLongOCT &dest);
    std::ostream &operator<<(std::ostream &in, const UnsignedLongLongBIN &dest);
  }
}
#endif
