#ifndef SCOPEGUARD_HPP
#define SCOPEGUARD_HPP

#include <ios>

namespace horoshilov
{
  class Guard
  {
  public:
    explicit Guard(std::basic_ios< char >& s);
    ~Guard();
    Guard(const Guard&) = delete;
    Guard(Guard&&) = delete;
    Guard& operator=(const Guard&) = delete;
    Guard& operator=(Guard&&) = delete;
  private:
    std::basic_ios< char >& s_;
    std::basic_ios< char >::fmtflags fmt_;
    std::streamsize width_;
    std::streamsize precision_;
    char fill_;
  };
}

#endif

