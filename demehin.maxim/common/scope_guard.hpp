#ifndef SCOPE_GUARD_HPP
#define SCOPE_GUARD_HPP
#include <iostream>

namespace demehin
{
  class iofmtguard
  {
  public:
    explicit iofmtguard(std::basic_ios< char >& s);

    iofmtguard(iofmtguard&) = delete;
    iofmtguard(iofmtguard&&) = delete;

    ~iofmtguard();

  private:
    std::basic_ios< char >& s_;
    std::basic_ios< char >::fmtflags fmt_;
    std::streamsize width_;
    std::streamsize precision_;
    char fill_;
  };
}
#endif
