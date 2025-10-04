#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include <iostream>
#include <map>
#include <vector>

namespace smirnov {

  class Dictionary {
  public:
    void insertWord(const std::string& word);
    void insertFromFile(const std::string& filename);
    bool contains(const std::string& word) const;
    int getFrequency(const std::string& word) const;
    std::string mostFrequent() const;
    void deleteWord(const std::string& word);
    void clear();
    bool empty() const;
    size_t size() const;
    std::vector< std::pair< std::string, size_t > > getSortedByWord() const;
    std::vector< std::pair< std::string, size_t > > getSortedByFrequency() const;
    double getRelativeFrequency(const std::string& word) const;
    std::vector< std::pair< std::string, double > > getTopRelative(size_t N) const;
    std::vector< std::pair< std::string, double > > getBottomRelative(size_t N) const;
    std::vector< std::pair< std::string, double > > getRangeRelative(double min, double max) const;
    std::vector< std::string> medianFrequency() const;

  private:
    std::map< std::string, size_t > data;
  };

  bool compareFrequency(const std::pair< std::string, int >& a, const std::pair< std::string, int >& b);
  bool compareByFrequency(const std::pair< std::string, int >& a, const std::pair< std::string, int >& b);

}

#endif
