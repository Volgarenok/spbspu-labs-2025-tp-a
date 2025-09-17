#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include <map>
#include <string>
#include <vector>

namespace smirnov {

  class Dictionary {
  public:
    Dictionary() = default;

    void insertWord(const std::string& word);
    void insertFromFile(const std::string& filename);
    bool contains(const std::string& word) const;
    int getFrequency(const std::string& word) const;
    std::string mostFrequent() const;
    void deleteWord(const std::string& word);
    void clear();
    bool empty() const;
    size_t size() const;

    std::vector<std::pair<std::string,int>> getSortedByWord() const;
    std::vector<std::pair<std::string,int>> getSortedByFrequency() const;

    double getRelativeFrequency(const std::string& word) const;
    std::vector<std::pair<std::string,double>> getTopRelative(int N) const;
    std::vector<std::pair<std::string,double>> getBottomRelative(int N) const;
    std::vector<std::pair<std::string,double>> getRangeRelative(double min, double max) const;

    std::vector<std::string> medianFrequency() const;

  private:
    std::map<std::string,int> data;
  };

}

#endif

