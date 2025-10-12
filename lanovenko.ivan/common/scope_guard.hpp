#ifndef SCOPE_GUARD_HPP
#define SCOPE_GUARD_HPP

#include <iomanip>

namespace lanovenko
{
  class StreamGuard
  {
  public:
    explicit StreamGuard(std::basic_ios< char >& s);
    ~StreamGuard();
  private:
    std::basic_ios< char >& s_;
    char fill_;
    std::streamsize width_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags flags_;
  };

}

#endif
