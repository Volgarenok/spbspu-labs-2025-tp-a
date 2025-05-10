#ifndef INPUT_HPP
#define INPUT_HPP
#include <istream>
#include <string>

namespace shak
{
  struct ChrLit
  {
    char &ref;
  };
  std::istream &operator>>(std::istream &in, ChrLit &&dest);

  struct RatLsp
  {
    std::pair< long long, unsigned long long > &ref;
  };
  std::istream &operator>>(std::istream &in, RatLsp &&dest);

  struct Str
  {
    std::string &ref;
  };
  std::istream &operator>>(std::istream &in, Str &&dest);
}
#endif
