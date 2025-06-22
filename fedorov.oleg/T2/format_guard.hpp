#ifndef FORMAT_GUARD_HPP
#define FORMAT_GUARD_HPP

#include <ios>
#include <iomanip>

namespace fedorov
{
  class IOFormatGuard
  {
  public:
    explicit IOFormatGuard(std::ostream &s);
    ~IOFormatGuard();

    IOFormatGuard(const IOFormatGuard &) = delete;
    IOFormatGuard &operator=(const IOFormatGuard &) = delete;

  private:
    std::ostream &s_;
    std::ios_base::fmtflags flags_;
    std::streamsize precision_;
  };
}
#endif
