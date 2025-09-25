#ifndef GUARD_HPP
#define GUARD_HPP
#include <iostream>

namespace guseynov
{
  class StreamGuard
  {
  public:
    explicit StreamGuard(std::basic_ios< char > & s);
    ~StreamGuard();
  private:
    std::basic_ios< char > & s_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
  };
}
#endif
