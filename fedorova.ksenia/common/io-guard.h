#ifndef IO_GUARD_H
#define IO_GUARD_H

#include <ios>

namespace fedorova
{
  class IoGuard
  {
  public:
    explicit IoGuard(std::basic_ios< char >& s);
    ~IoGuard();
  private:
    std::basic_ios< char >& s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
  };
}
#endif

