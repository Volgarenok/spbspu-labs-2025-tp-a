#ifndef FORMATGUARD_HPP
#define FORMATGUARD_HPP

#include <ios>

namespace pilugina
{
  class FormatGuard
  {
  public:
    explicit FormatGuard(std::basic_ios< char > &s);
    ~FormatGuard();

  private:
    std::basic_ios< char > &s_;
    std::streamsize width_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
    char fill_;
  };
}
#endif
