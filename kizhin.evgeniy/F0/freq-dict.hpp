#ifndef SPBSPU_LABS_2025_TP_A_KIZHIN_EVGENIY_F0_FREQ_DICT_HPP
#define SPBSPU_LABS_2025_TP_A_KIZHIN_EVGENIY_F0_FREQ_DICT_HPP

#include <iosfwd>
#include <map>
#include <vector>
#include <string>

namespace kizhin {
  struct FrequencyDictionary
  {
    using WordMap = std::map< std::string, std::size_t >;
    using FreqVector = std::vector< std::pair< std::size_t, std::string > >;
    WordMap byWord{};
    FreqVector byFrequency{};
    std::size_t total = 0;
  };

  FrequencyDictionary loadDictionary(const std::vector< std::string >&);
}

#endif

