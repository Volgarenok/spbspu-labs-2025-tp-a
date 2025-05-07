#ifndef IO_FMTGUARD_H
#define IO_FMTGUARD_H
#include <iostream>

namespace ohantsev
{
  class iofmtguard
  {
  public:
    explicit iofmtguard(std::basic_ios< char >& s);
    ~iofmtguard();
    iofmtguard(const iofmtguard&) = delete;
    iofmtguard(iofmtguard&&) = delete;
    iofmtguard& operator=(const iofmtguard&) = delete;
    iofmtguard& operator=(iofmtguard&&) = delete;
  private:
    std::streamsize precision_;
    std::streamsize width_;
    std::basic_ios< char >& s_;
    std::basic_ios< char >::fmtflags fmt_;
    char fill_;
  };
}
#endif
