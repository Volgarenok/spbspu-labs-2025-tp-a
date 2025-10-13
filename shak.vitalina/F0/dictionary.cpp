#include "dictionary.hpp"

std::string shak::printPair(const std::pair< std::string, size_t > &p)
{
  return p.first + ": " + std::to_string(p.second);
}

bool shak::uniqueOnce(const std::pair< const std::string, size_t > &p)
{
    return p.second == 1;
}

bool shak::pairEqual(const std::pair< const std::string, size_t > &p1, const std::pair< const std::string, size_t > &p2)
{
  return p1.first == p2.first && p1.second == p2.second;
}

bool shak::sortByFreq(const std::pair< std::string, size_t > &p1, const std::pair< std::string, size_t > &p2)
{
  if (p1.second != p2.second) {
    return p1.second > p2.second;
  }
  return p1.first < p2.first;
}

shak::IntersectDict::IntersectDict(const shak::FrequencyDictionary &d):
  dict2(d)
{}

bool shak::IntersectDict::operator()(const std::pair< const std::string, size_t > &entry) const
{
  return dict2.dictionary.count(entry.first) > 0;
}
