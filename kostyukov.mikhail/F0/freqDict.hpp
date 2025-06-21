#ifndef FREQ_DICT_HPP
#define FREQ_DICT_HPP

#include <iosfwd>
#include <map>
#include <string>

namespace kostyukov
{
  struct FrequencyDictionary
  {
    std::map< std::string, size_t > counts;
    size_t totalWords = 0;
  };

  using FreqDictManager = std::map< std::string, FrequencyDictionary >;

  struct WordFreqPair
  {
    std::string word;
    double freq;
  };

  void printHelp(std::ostream& out);
  void createDict(std::istream& in, std::ostream& out, FreqDictManager& dicts);
  void loadDict(std::istream& in, std::ostream& out, FreqDictManager& dicts);
  void getFreq(std::istream& in, std::ostream& out, const FreqDictManager& dicts);
  void top(std::istream& in, std::ostream& out, const FreqDictManager& dicts);
  void bottom(std::istream& in, std::ostream& out, const FreqDictManager& dicts);
  void listDicts(std::ostream& out, const FreqDictManager& dicts);
  void deleteDict(std::istream& in, std::ostream& out, FreqDictManager& dicts);
  void merge(std::istream& in, std::ostream& out, FreqDictManager& dicts);
  void intersect(std::istream& in, std::ostream& out, FreqDictManager& dicts);
  void clear(std::istream& in, std::ostream& out, FreqDictManager& dicts);
  void findUniq(std::istream& in, std::ostream& out, const FreqDictManager& dicts);
  void findSame(std::istream& in, std::ostream& out, const FreqDictManager& dicts);
  void removeBatch(std::istream& in, std::ostream& out, FreqDictManager& dicts);
  void rangeFreq(std::istream& in, std::ostream& out, const FreqDictManager& dicts);
  void outRangeFreq(std::istream& in, std::ostream& out, const FreqDictManager& dicts);
}
#endif
