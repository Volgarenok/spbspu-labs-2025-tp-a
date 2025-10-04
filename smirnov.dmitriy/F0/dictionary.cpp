#include "dictionary.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <numeric>

namespace smirnov
{
  bool compareFrequency(const std::pair< std::string, int >& a, const std::pair< std::string, int >& b)
  {
    return a.second < b.second;
  }

  bool compareByFrequency(const std::pair< std::string, int >& a, const std::pair< std::string, int >& b)
  {
    return a.second > b.second;
  }

  std::string Dictionary::mostFrequent() const
  {
    if (data.empty()) return "";
    return std::max_element(
      data.begin(), data.end(), smirnov::compareFrequency
    )->first;
  }

  void Dictionary::insertWord(const std::string& word)
  {
    data[word]++;
  }

  void Dictionary::insertFromFile(const std::string& filename)
  {
    std::ifstream file(filename);
    if (!file.is_open()) return;
    std::string word;
    while (file >> word)
    {
      insertWord(word);
    }
  }

  bool Dictionary::contains(const std::string& word) const
  {
    return data.find(word) != data.cend();
  }

  int Dictionary::getFrequency(const std::string& word) const
  {
    auto it = data.find(word);
    if (it != data.end()) return it->second;
    return -1;
  }

  void Dictionary::deleteWord(const std::string& word)
  {
    data.erase(word);
  }

  void Dictionary::clear()
  {
    data.clear();
  }

  bool Dictionary::empty() const
  {
    return data.empty();
  }

  size_t Dictionary::size() const
  {
    return data.size();
  }

  std::vector< std::pair< std::string, int > > Dictionary::getSortedByWord() const
  {
    std::vector< std::pair< std::string, int > > vec(data.begin(), data.end());
    return vec;
  }

  std::vector< std::pair< std::string, int > > Dictionary::getSortedByFrequency() const
  {
    std::vector< std::pair< std::string, int > > vec(data.begin(), data.end());
    std::sort(vec.begin(), vec.end(), smirnov::compareByFrequency);
    return vec;
  }

  double Dictionary::getRelativeFrequency(const std::string& word) const
  {
    if (data.empty() || !contains(word)) return -1.0;
    int total = 0;
    for (auto& p : data) total += p.second;
    return static_cast<double>(data.at(word)) / total;
  }

  std::vector< std::pair<  std::string, double > > Dictionary::getTopRelative(size_t N) const
  {
    std::vector< std::pair< std::string, double > > vec;
    if (data.empty()) return vec;
    int total = 0;
    for (auto& p : data) total += p.second;
    for (auto& p : data)
      vec.push_back({ p.first, static_cast<double>(p.second) / total });
    std::sort(vec.begin(), vec.end(), smirnov::compareByFrequency);
    if (static_cast<int>(vec.size()) < N) return {};
    vec.resize(N);
    return vec;
  }

  std::vector< std::pair< std::string, double > > Dictionary::getBottomRelative(int N) const
  {
    std::vector< std::pair< std::string, double > > vec;
    if (data.empty()) return vec;
    int total = 0;
    for (auto& p : data) total += p.second;
    for (auto& p : data)
      vec.push_back({ p.first, static_cast<double>(p.second) / total });
    std::sort(vec.begin(), vec.end(), smirnov::compareFrequency);
    if (static_cast<int>(vec.size()) < N) return {};
    vec.resize(N);
    return vec;
  }

  std::vector< std::pair< std::string, double > > Dictionary::getRangeRelative(double min, double max) const
  {
    std::vector< std::pair< std::string, double > > result;
    if (min > max || data.empty()) return result;
    int total = 0;
    for (auto& p : data) total += p.second;
    for (auto& p : data)
    {
      double rel = static_cast<double>(p.second) / total;
      if (rel >= min && rel <= max)
      {
        result.push_back({ p.first, rel });
      }
    }
    return result;
  }

  std::vector< std::string > Dictionary::medianFrequency() const
  {
    std::vector< std::string > result;
    if (data.empty()) return result;
    std::vector< int> freqs;
    for (auto& p : data) freqs.push_back(p.second);
    std::sort(freqs.begin(), freqs.end());
    size_t n = freqs.size();
    int median;
    if (n % 2 == 1)
      median = freqs[n / 2];
    else
      median = (freqs[n / 2 - 1] + freqs[n / 2]) / 2;
    for (auto& p : data)
      if (p.second == median)
        result.push_back(p.first);
    return result;
  }
}

