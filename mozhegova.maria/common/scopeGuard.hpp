#ifndef SCOPEGUARD_HPP
#define SCOPEGUARD_HPP

#include <iomanip>

namespace mozhegova
{
  class iofmtguard
  {
  public:
    explicit iofmtguard(std::basic_ios< char > & s);
    ~iofmtguard();
  private:
    std::basic_ios< char > & s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
  };
}

#endif
