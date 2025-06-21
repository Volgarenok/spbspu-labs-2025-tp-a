#ifndef FORMAT_GUARD_HPP
#define FORMAT_GUARD_HPP

#include <ios>

namespace shiryaeva
{
  class FormatGuard
  {
  public:
    explicit FormatGuard(std::basic_ios< char > &s);
    ~FormatGuard();

    FormatGuard(const FormatGuard &) = delete;
    FormatGuard &operator=(const FormatGuard &) = delete;
    FormatGuard(FormatGuard &&) = delete;
    FormatGuard &operator=(FormatGuard &&) = delete;

  private:
    std::basic_ios< char > &s_;
    std::streamsize width_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
    char fill_;
  };
}

#endif
