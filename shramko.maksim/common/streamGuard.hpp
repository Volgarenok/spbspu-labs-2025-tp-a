#ifndef STREAMGUARD_HPP
#define STREAMGUARD_HPP

#include <iostream>

namespace shramko
{
  struct StreamGuard
  {
    explicit StreamGuard(std::basic_ios< char > & s);
    ~StreamGuard();
  private:
    std::basic_ios< char > & s_;
    std::streamsize width_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
    char fill_;
  };
}

#endif
