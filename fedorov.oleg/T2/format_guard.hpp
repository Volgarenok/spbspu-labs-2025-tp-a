#ifndef FORMAT_GUARD
#define FORMAT_GUARD

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

  IOFormatGuard::IOFormatGuard(std::ostream &s):
    s_(s),
    flags_(s.flags()),
    precision_(s.precision())
  {}

  IOFormatGuard::~IOFormatGuard()
  {
    s_.precision(precision_);
    s_.flags(flags_);
  }
}
#endif
