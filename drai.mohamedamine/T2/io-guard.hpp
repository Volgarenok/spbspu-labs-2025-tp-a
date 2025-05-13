#ifndef SPBSPU_LABS_2025_TP_A_FIRSTRY_T2_IO_GUARD_HPP
#define SPBSPU_LABS_2025_TP_A_FIRSTRY_T2_IO_GUARD_HPP

#include <ios>

namespace firstry {
  class IoGuard;
}

class firstry::IoGuard
{
public:
  IoGuard(const IoGuard&) = delete;
  explicit IoGuard(std::basic_ios< char >&);
  ~IoGuard();

  IoGuard& operator=(const IoGuard&) = delete;

private:
  std::basic_ios< char >& s_;
  std::streamsize precision_;
  std::streamsize width_;
  std::basic_ios< char >::fmtflags flags_;
  char fill_;
};

#endif