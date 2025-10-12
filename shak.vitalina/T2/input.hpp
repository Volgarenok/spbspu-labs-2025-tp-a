#ifndef INPUT_HPP
#define INPUT_HPP
#include <istream>
#include <string>

namespace shak
{
  struct ChrLitIO
  {
    char &ref;
  };
  std::istream &operator>>(std::istream &in, ChrLitIO &&dest);

  struct RatLspIO
  {
    std::pair< long long, unsigned long long > &ref;
  };
  std::istream &operator>>(std::istream &in, RatLspIO &&dest);

  struct StrIO
  {
    std::string &ref;
  };
  std::istream &operator>>(std::istream &in, StrIO &&dest);
}
#endif
