#ifndef SPBSPU_LABS_2025_TP_A_KIZHIN_EVGENIY_F0_FREQ_DICT_HPP
#define SPBSPU_LABS_2025_TP_A_KIZHIN_EVGENIY_F0_FREQ_DICT_HPP

#include <iosfwd>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace kizhin {
  struct FrequencyDictionary
  {
    using WordMap = std::map< std::string, std::size_t >;
    // tODO: Add some container with freqs (floats) or append to existing one
    using Words = std::set< std::string >;
    using FreqVector = std::vector< std::pair< std::size_t, std::string > >;
    WordMap byWord{};
    Words words{};
    FreqVector byFrequency{};
    std::size_t total = 0;
  };

  FrequencyDictionary loadDictionary(const std::vector< std::string >&);
}

#endif

