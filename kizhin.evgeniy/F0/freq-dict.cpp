#include "freq-dict.hpp"
#include <algorithm>
#include <fstream>

kizhin::FrequencyDictionary kizhin::loadDictionary(
    const std::vector< std::string >& files)
{
  // TODO: Refactor
  FrequencyDictionary::WordMap byWord;
  FrequencyDictionary::Words words;
  for (const auto& file: files) {
    std::ifstream fin(file);
    if (!fin.is_open()) {
      // TODO: What to do here?
      //      std::cerr << "Failed to load file: " << file << '\n';
      continue;
    }
    std::string current;
    while (fin >> current) {
      if (!byWord.count(current)) {
        byWord[current] = 0;
      }
      ++byWord[current];
      words.insert(current);
    }
  }
  FrequencyDictionary::FreqVector byFreq;
  byFreq.reserve(byWord.size());
  std::size_t total = 0;
  for (const auto& v: byWord) {
    total += v.second;
    byFreq.emplace_back(v.second, v.first);
  }
  auto pred = [](const auto& a, const auto& b)
  {
    if (a.first != b.first) {
      return a.first > b.first;
    }
    return a.second < b.second;
  };
  std::sort(byFreq.begin(), byFreq.end(), pred);
  return FrequencyDictionary{ byWord, words, byFreq, total };
}

