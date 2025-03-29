#ifndef FORMATGUARD_HPP
#define FORMATGUARD_HPP

#include <ios>

namespace puzikov
{
  struct FormatGuard
  {
    FormatGuard(std::basic_ios< char > &s);
    ~FormatGuard();

  private:
    std::basic_ios< char > &s_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
  };
} // namespace puzikov

#endif
