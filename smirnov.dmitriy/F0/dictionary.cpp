#include "dictionary.hpp"
#include <algorithm>
#include <fstream>
#include <functional>
#include <iterator>
#include <numeric>
#include <sstream>

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

  struct AccumulateFrequency
  {
    int operator()(int sum, const std::pair< std::string, size_t >& p) const
    {
      return sum + p.second;
    }
  };

  struct TransformToRelativeFrequency
  {
    int total;
    TransformToRelativeFrequency(int t):
      total(t)
    {}

    std::pair< std::string, double > operator()(const std::pair< std::string, size_t >& p) const
    {
      return std::make_pair(p.first, static_cast< double >(p.second) / total);
    }
  };

  struct FilterByRelativeRange
  {
    double min, max;
    int total;
    FilterByRelativeRange(double mmin, double mmax, int ttotal):
      min(mmin),
      max(mmax),
      total(ttotal)
    {}

    std::pair< std::string, double > operator()(const std::pair< std::string, size_t >& p) const
    {
      double rel = static_cast< double >(p.second) / total;
      return (rel >= min && rel <= max) ? std::make_pair(p.first, rel) : std::make_pair(std::string(), -1.0);
    }
  };

  struct IsInvalidPair
  {
    bool operator()(const std::pair< std::string, double >& p) const
    {
      return p.second < 0;
    }
  };

  struct ExtractFrequency
  {
    size_t operator()(const std::pair< std::string, size_t >& p) const
    {
      return p.second;
    }
  };

  struct FilterByMedian
  {
    size_t median;
    FilterByMedian(size_t m):
      median(m)
    {}

    std::string operator()(const std::pair< std::string, size_t >& p) const
    {
      return (p.second == median) ? p.first : std::string();
    }
  };

  struct CompareByRelativeDescending
  {
    bool operator()(const std::pair< std::string, double >& a, const std::pair< std::string, double >& b) const
    {
      return a.second > b.second;
    }
  };

  struct CompareByRelativeAscending
  {
    bool operator()(const std::pair< std::string, double >& a, const std::pair< std::string, double >& b) const
    {
      return a.second < b.second;
    }
  };

  std::string Dictionary::mostFrequent() const
  {
    if (data.empty())
    {
      return "";
    }
    return std::max_element(data.begin(), data.end(), smirnov::compareFrequency)->first;
  }

  void Dictionary::insertWord(const std::string& word)
  {
    data[word]++;
  }

  void Dictionary::insertFromFile(const std::string& filename)
  {
    std::ifstream file(filename);
    if (!file.is_open())
    {
      return;
    }
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
    if (it != data.end())
    {
      return it->second;
    }
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

  std::vector< std::pair< std::string, size_t > > Dictionary::getSortedByWord() const
  {
    return std::vector< std::pair< std::string, size_t > >(data.begin(), data.end());
  }

  std::vector< std::pair< std::string, size_t > > Dictionary::getSortedByFrequency() const
  {
    std::vector< std::pair< std::string, size_t > > vec(data.begin(), data.end());
    std::sort(vec.begin(), vec.end(), smirnov::compareByFrequency);
    return vec;
  }

  double Dictionary::getRelativeFrequency(const std::string& word) const
  {
    if (data.empty() || !contains(word))
    {
      return -1.0;
    }

    int total = std::accumulate(data.begin(), data.end(), 0, AccumulateFrequency());
    return static_cast< double >(data.at(word)) / total;
  }

  std::vector< std::pair< std::string, double > > Dictionary::getTopRelative(size_t N) const
  {
    std::vector< std::pair< std::string, double > > vec;

    if (data.empty())
    {
      return vec;
    }

    int total = std::accumulate(data.begin(), data.end(), 0, AccumulateFrequency());
    vec.resize(data.size());

    std::transform(data.begin(), data.end(), vec.begin(), TransformToRelativeFrequency(total));
    std::partial_sort(vec.begin(), vec.begin() + std::min(N, vec.size()), vec.end(), CompareByRelativeDescending());

    if (vec.size() < N)
    {
      return std::vector< std::pair< std::string, double > >();
    }

    vec.resize(N);
    return vec;
  }

  std::vector< std::pair< std::string, double > > Dictionary::getBottomRelative(size_t N) const
  {
    std::vector< std::pair< std::string, double > > vec;

    if (data.empty())
    {
      return vec;
    }

    int total = std::accumulate(data.begin(), data.end(), 0, AccumulateFrequency());
    vec.resize(data.size());

    std::transform(data.begin(), data.end(), vec.begin(), TransformToRelativeFrequency(total));
    std::partial_sort(vec.begin(), vec.begin() + std::min(N, vec.size()), vec.end(), CompareByRelativeAscending());

    if (vec.size() < N)
    {
      return std::vector< std::pair< std::string, double > >();
    }

    vec.resize(N);
    return vec;
  }

  std::vector< std::pair< std::string, double > > Dictionary::getRangeRelative(double min, double max) const
  {
    std::vector< std::pair< std::string, double > > result;

    if (min > max || data.empty())
    {
      return result;
    }

    int total = std::accumulate(data.begin(), data.end(), 0, AccumulateFrequency());
    result.resize(data.size());

    std::transform(data.begin(), data.end(), result.begin(), FilterByRelativeRange(min, max, total));
    result.erase(std::remove_if(result.begin(), result.end(), IsInvalidPair()), result.end());

    return result;
  }

  std::vector< std::string > Dictionary::medianFrequency() const
  {
    std::vector< std::string > result;

    if (data.empty())
    {
      return result;
    }

    std::vector< size_t > freqs;
    freqs.resize(data.size());
    std::transform(data.begin(), data.end(), freqs.begin(), ExtractFrequency());

    std::sort(freqs.begin(), freqs.end());
    size_t n = freqs.size();
    size_t median;

    if (n % 2 == 1)
    {
      median = freqs[n / 2];
    }
    else
    {
      median = (freqs[n / 2 - 1] + freqs[n / 2]) / 2;
    }

    result.resize(data.size());
    std::transform(data.begin(), data.end(), result.begin(), FilterByMedian(median));
    result.erase(std::remove(result.begin(), result.end(), std::string()), result.end());

    return result;
  }
}
