#ifndef INPUT_STRUCT_HPP
#define INPUT_STRUCT_HPP

#include <string>

namespace shiryaeva
{
  struct DelimiterIO
  {
    char exp;
  };
  std::istream &operator>>(std::istream &in, DelimiterIO &&dest);

  struct HexUllIO
  {
    unsigned long long &ref;
  };
  std::istream &operator>>(std::istream &in, HexUllIO &&dest);

  struct CharIO
  {
    char &ref;
  };
  std::istream &operator>>(std::istream &in, CharIO &&dest);

  struct StringIO
  {
    std::string &ref;
  };
  std::istream &operator>>(std::istream &in, StringIO &&dest);
}

#endif
