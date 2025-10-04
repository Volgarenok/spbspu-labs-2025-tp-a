#ifndef DICTIONARY_MANAGER_HPP
#define DICTIONARY_MANAGER_HPP

#include <string>
#include <map>

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
  bool addWord(const std::string& dict_name, const std::string& word, int freq = 1);
  bool removeWord(const std::string& dict_name, const std::string& word);
  bool getFreq(const std::string& dict_name, const std::string& word, int& freq) const;
  const std::map< std::string, int >* getDict(const std::string& name) const;
  std::map< std::string, int >* getDictMutable(const std::string& name);
  bool loadFromFile(const std::string& dict_name, const std::string& filename);

 private:
  std::map< std::string, std::map< std::string, int > > dicts_;
};

#endif
