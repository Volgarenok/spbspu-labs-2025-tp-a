#ifndef IOFMTGUARD_HPP
#define IOFMTGUARD_HPP

#include <iomanip>

namespace klimova
{
  class Iofmtguard
  {
  public:
    explicit Iofmtguard(std::basic_ios< char >& s);
    ~Iofmtguard();
  private:
    std::basic_ios< char >& s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
  };
}
#endif
