#ifndef SPBSPU_LABS_2025_TP_A_KIZHIN_EVGENIY_F0_FREQ_DICT_HPP
#define SPBSPU_LABS_2025_TP_A_KIZHIN_EVGENIY_F0_FREQ_DICT_HPP

#include <iosfwd>
#include <map>
#include <set>
#include <string>

namespace kizhin {
  using WordMap = std::map< std::string, std::size_t >;
  using WordSet = std::set< std::string >;
  using WordAndSize = std::pair< const std::string, std::size_t >;
  struct SizeDescendingComp;
  using SizeSet = std::set< WordAndSize, SizeDescendingComp >;

  struct SizeDescendingComp
  {
    bool operator()(const WordAndSize&, const WordAndSize&) const;
  };

  struct FrequencyDictionary
  {
    WordMap wordMap{};
    WordSet wordSet{};
    SizeSet sizeSet{};
    std::size_t total = 0;
  };

  void expandDictionary(std::istream&, FrequencyDictionary&);
}

#endif

