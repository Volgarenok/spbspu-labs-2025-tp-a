#ifndef DICTIONARY_MANAGER_HPP
#define DICTIONARY_MANAGER_HPP

#include <string>
#include <map>
#include <vector>
#include <regex>

class DictionaryManager
{
public:
  DictionaryManager() = default;
  ~DictionaryManager() = default;

  DictionaryManager(const DictionaryManager&) = delete;
  DictionaryManager& operator=(const DictionaryManager&) = delete;

  DictionaryManager(DictionaryManager&&) noexcept = default;
  DictionaryManager& operator=(DictionaryManager&&) noexcept = default;

  bool createDict(const std::string& name);
  bool deleteDict(const std::string& name);
  bool pushDict(const std::string& name);
  bool popDict();
  std::vector< std::string > getStack() const;

  bool addWord(const std::string& word, int freq_val = 1);
  bool removeWord(const std::string& word);
  bool getFreq(const std::string& word, int& freq) const;

  bool mergeDictionaries(const std::string& otherName, const std::string& newDictName);
  bool intersectDictionaries(const std::string& otherName, const std::string& newDictName);
  bool filterByPattern(const std::string& regex_str, const std::string& resultDictName);

  bool loadFromFile(const std::string& filename);
  bool saveToFile(const std::string& filename) const;

  std::map< std::string, int >* getCurrentDictMutable();
  const std::map< std::string, int >* getCurrentDict() const;
  const std::map< std::string, int >* getDictByName(const std::string& name) const;

private:
  std::map< std::string, std::map< std::string, int > > dicts_;
  std::vector< std::string > stack_;
};

#endif

