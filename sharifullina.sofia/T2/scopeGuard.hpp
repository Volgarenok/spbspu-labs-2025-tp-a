#ifndef SCOPEGUARD_HPP
#define SCOPEGUARD_HPP

#include <iomanip>

namespace sharifullina
{
  class IofmtGuard
  {
  public:
    explicit IofmtGuard(std::basic_ios< char > & s);
    ~IofmtGuard();
  private:
    std::basic_ios< char > & s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
  };
}

#endif
