#ifndef IO_FMT_GUARD_HPP
#define IO_FMT_GUARD_HPP

#include <iomanip>

namespace maslevtsov {
  class IOFmtGuard
  {
  public:
    explicit IOFmtGuard(std::basic_ios< char >& s);
    ~IOFmtGuard();

  private:
    std::basic_ios< char >& s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
  };
}

#endif
