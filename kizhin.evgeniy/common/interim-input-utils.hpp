#ifndef SPBSPU_LABS_2025_TP_A_KIZHIN_EVGENIY_COMMON_INTERIM_INPUT_UTILS_HPP
#define SPBSPU_LABS_2025_TP_A_KIZHIN_EVGENIY_COMMON_INTERIM_INPUT_UTILS_HPP

#include <iosfwd>

namespace kizhin {
  struct Delimiter;
  struct OneOfDelimiters;

  std::istream& operator>>(std::istream&, Delimiter&&);
  std::istream& operator>>(std::istream&, OneOfDelimiters&&);
}

struct kizhin::Delimiter
{
  char val;
};

struct kizhin::OneOfDelimiters
{
  const std::string& val;
};

#endif

