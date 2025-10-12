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

  struct HexUllO
  {
    const unsigned long long& ref;
  };
  std::ostream& operator<<(std::ostream& out, const HexUllO& src);

  struct CharO
  {
    const char& ref;
  };
  std::ostream& operator<<(std::ostream& out, const CharO& src);

  struct StringO
  {
    const std::string& ref;
  };
  std::ostream& operator<<(std::ostream& out, const StringO& src);
}

#endif
