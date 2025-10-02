#ifndef IO_STREAM_GUARD_HPP
#define IO_STREAM_GUARD_HPP

#include <iostream>

namespace gavrilova {
  class IOStreamGuard {
  public:
    explicit IOStreamGuard(std::basic_ios< char >& s);
    ~IOStreamGuard();

  private:
    std::basic_ios< char >& s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
  };
}

#endif
