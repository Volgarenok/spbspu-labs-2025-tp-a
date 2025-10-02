#ifndef FORMAT_GUARD_HPP
#define FORMAT_GUARD_HPP

#include <ios>

namespace filonova
{
  struct FormatGuard
  {
    explicit FormatGuard(std::basic_ios< char > &s);

    FormatGuard(const FormatGuard &) = delete;
    FormatGuard &operator=(const FormatGuard &) = delete;
    FormatGuard(FormatGuard &&) = delete;
    FormatGuard &operator=(FormatGuard &&) = delete;

    ~FormatGuard();

  private:
    std::basic_ios< char > &s_;
    std::basic_ios< char >::fmtflags fmt_;
    std::streamsize precision_;
    char fill_;
  };

}

#endif
