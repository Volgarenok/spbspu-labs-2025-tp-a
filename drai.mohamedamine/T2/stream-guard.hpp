#ifndef SPBSPU_LABS_2025_TP_A_FIRSTRY_T2_STREAM_GUARD_HPP
#define SPBSPU_LABS_2025_TP_A_FIRSTRY_T2_STREAM_GUARD_HPP

#include <ios>

namespace firstry {
  class StreamGuard;
}

class firstry::StreamGuard
{
public:
  StreamGuard(const StreamGuard&) = delete;
  explicit StreamGuard(std::basic_ios< char >&);
  ~StreamGuard();

  StreamGuard& operator=(const StreamGuard&) = delete;

private:
  std::basic_ios< char >& s_;
  std::streamsize precision_;
  std::streamsize width_;
  std::basic_ios< char >::fmtflags flags_;
  char fill_;
};

#endif
