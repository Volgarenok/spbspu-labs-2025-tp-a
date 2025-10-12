#ifndef FORMATGUARD_HPP
#define FORMATGUARD_HPP

#include <ios>

namespace fedorov
{
  struct FormatGuard
  {
    explicit FormatGuard(std::basic_ios< char > &s);
    ~FormatGuard();
    FormatGuard(const FormatGuard &) = delete;
    FormatGuard(FormatGuard &&) = delete;

  private:
    std::basic_ios< char > &s_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
    char fill_;
  };
}

#endif
