#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <utility>

class Dictionary
{
public:
  void addWord(const std::string &word, const std::string &translation);
  void removeWord(const std::string &word);
  void editWord(const std::string &word, const std::string &newTranslation);
  bool findWord(const std::string &word, std::string &translation) const;
  std::vector<std::string> listWordsWithPrefix(const std::string &prefix) const;
  std::vector<std::pair<std::string, std::string>> getAllWords() const;
  std::size_t getWordCount() const;
  void clear();
  const std::unordered_map<std::string, std::string> &getData() const;
  void mergeFrom(const Dictionary &other);
  Dictionary intersectWith(const Dictionary &other) const;
  Dictionary diffFrom(const Dictionary &other) const;
  Dictionary extractRange(const std::string &start, const std::string &end) const;
  std::vector<std::string> getWordsByTranslationFragment(const std::string &fragment) const;
  std::pair<std::string, std::string> getRandomPair() const;
  void swapTranslations();

private:
  std::unordered_map<std::string, std::string> data_;
};

#endif
