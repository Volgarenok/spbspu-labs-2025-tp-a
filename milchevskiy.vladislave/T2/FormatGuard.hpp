#ifndef FORMATGUARD_HPP
#define FORMATGUARD_HPP

#include <ios>

namespace milchevskiy {

  class FormatGuard
  {
  public:
    explicit FormatGuard(std::basic_ios<char>& s);
    ~FormatGuard();

    FormatGuard(const FormatGuard&) = delete;
    FormatGuard& operator=(const FormatGuard&) = delete;

  private:
    std::basic_ios<char>& s_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios<char>::fmtflags fmt_;
  };

} // namespace milchevskiy

#endif

