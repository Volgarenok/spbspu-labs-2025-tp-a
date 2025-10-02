#ifndef GUARD_H
#define GUARD_H

#include <iostream>

namespace kharlamov
{
  class Guard
  {
  public:
    explicit Guard(std::basic_ios< char >& s);
    Guard(const Guard&) = delete;
    Guard(Guard&&) = delete;
    ~Guard();
  private:
    std::basic_ios< char >& s_;
    std::basic_ios< char >::fmtflags fmt_;
    std::streamsize width_;
    std::streamsize precision_;
    char fill_;
  };
}

#endif
