#ifndef STREAMGUARD_HPP
#define STREAMGUARD_HPP

#include <iomanip>

namespace kushekbaev
{
  struct StreamGuard
  {
  public:
    explicit StreamGuard(std::basic_ios< char >& s);
    ~StreamGuard();
  private:
    std::basic_ios< char >& s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
  };
}

#endif
