#ifndef SCOPEGUARD_HPP
#define SCOPEGUARD_HPP

#include <ios>

namespace horoshilov
{
  class Guard
  {
  public:
    Guard(std::basic_ios< char >& s);
    ~Guard();
  private:
    std::basic_ios< char >& s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
  };
}

#endif
