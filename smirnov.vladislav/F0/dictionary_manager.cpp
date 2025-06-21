#include "dictionary_manager.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <stdexcept>

const std::map< std::string, int >* DictionaryManager::getDictByName(const std::string& name) const
{
  auto it = dicts_.find(name);
  return it == dicts_.end() ? nullptr : &(it->second);
}

std::map< std::string, int >* DictionaryManager::getCurrentDictMutable()
{
  if (stack_.empty())
  {
    return nullptr;
  }
  auto it = dicts_.find(stack_.back());
  return it == dicts_.end() ? nullptr : &(it->second);
}

const std::map<std::string, int>* DictionaryManager::getCurrentDict() const
{
  if (stack_.empty())
  {
    return nullptr;
  }
  auto it = dicts_.find(stack_.back());
  return it == dicts_.end() ? nullptr : &(it->second);
}

bool DictionaryManager::createDict(const std::string& name)
{
  auto inserted = dicts_.emplace(name, std::map< std::string, int >{});
  return inserted.second;
}

bool DictionaryManager::deleteDict(const std::string& name)
{
  return dicts_.erase(name) > 0;
}

bool DictionaryManager::pushDict(const std::string& name)
{
  if (dicts_.find(name) == dicts_.end())
  {
    return false;
  }
  stack_.push_back(name);
  return true;
}

bool DictionaryManager::popDict()
{
  if (stack_.empty())
  {
    return false;
  }
  stack_.pop_back();
  return true;
}

std::vector< std::string > DictionaryManager::getStack() const
{
  return stack_;
}

bool DictionaryManager::addWord(const std::string& word, int freq_val)
{
  auto dict = getCurrentDictMutable();
  if (!dict)
  {
    return false;
  }
  (*dict)[word] += freq_val;
  return true;
}

bool DictionaryManager::removeWord(const std::string& word)
{
  auto dict = getCurrentDictMutable();
  if (!dict)
  {
    return false;
  }

  auto it = dict->find(word);
  if (it == dict->end())
  {
    return false;
  }

  dict->erase(it);
  return true;
}

bool DictionaryManager::getFreq(const std::string& word, int& freq) const
{
  auto dict = getCurrentDict();
  if (!dict)
  {
    return false;
  }

  auto it = dict->find(word);
  if (it == dict->end())
  {
    return false;
  }

  freq = it->second;
  return true;
}

bool DictionaryManager::mergeDictionaries(const std::string& otherName, const std::string& newDictName)
{
  const auto* currentDict = getCurrentDict();
  if (!currentDict)
  {
    return false;
  }

  const auto* otherDict = getDictByName(otherName);
  if (!otherDict)
  {
    return false;
  }

  if (dicts_.count(newDictName))
  {
    return false;
  }

  std::map< std::string, int > mergedDict = *currentDict;

  std::for_each(otherDict->begin(), otherDict->end(),
    [&mergedDict](const std::pair< const std::string, int >& p)
    {
      mergedDict[p.first] += p.second;
    });

  dicts_[newDictName] = std::move(mergedDict);
  return true;
}

bool DictionaryManager::intersectDictionaries(const std::string& otherName, const std::string& newDictName)
{
  const auto* currentDict = getCurrentDict();
  if (!currentDict)
  {
    return false;
  }

  const auto* otherDict = getDictByName(otherName);
  if (!otherDict)
  {
    return false;
  }

  if (dicts_.count(newDictName))
  {
    return false;
  }

  std::map< std::string, int > intersectionDict;

  std::for_each(currentDict->begin(), currentDict->end(),
    [&otherDict, &intersectionDict](const std::pair<const std::string, int>& p) {
      auto oit = otherDict->find(p.first);
      if (oit != otherDict->end())
      {
        intersectionDict[p.first] = std::min(p.second, oit->second);
      }
    });

  dicts_[newDictName] = std::move(intersectionDict);
  return true;
}
bool DictionaryManager::loadFromFile(const std::string & filename)
{
  std::map< std::string, int >* currentDict = getCurrentDictMutable();
  if (!currentDict)
  {
    return false;
  }

  std::ifstream file(filename);
  if (!file.is_open())
  {
    return false;
  }

  currentDict->clear();

  std::string word;
  while (file >> word)
  {
    std::transform(word.begin(), word.end(), word.begin(),
      [](unsigned char c) { return static_cast< char >(std::tolower(c)); });
    word.erase(std::remove_if(word.begin(), word.end(),
      [](unsigned char c) { return !std::isalpha(c); }),
      word.end());


    if (!word.empty())
    {
      (*currentDict)[word]++;
    }
  }
  file.close();

  if (!file.eof() && file.fail())
  {
    return false;
  }

  return true;
}

bool DictionaryManager::saveToFile(const std::string& filename) const
{
  const auto* dict = getCurrentDict();
  if (!dict)
  {
    return false;
  }

  if (dict->empty())
  {
    return false;
  }

  std::ofstream fout(filename.c_str());
  if (!fout)
  {
    return false;
  }

  std::for_each(dict->begin(), dict->end(),
    [&fout](const std::pair< const std::string, int >& p) {
      fout << p.first << " " << p.second << "\n";
    });
  return true;
}

bool DictionaryManager::filterByPattern(const std::string& regex_str, const std::string& resultDictName)
{
  const auto* currentDict = getCurrentDict();
  if (!currentDict)
  {
    return false;
  }

  if (dicts_.count(resultDictName))
  {
    return false;
  }

  std::regex pattern;
  try
  {
    pattern = std::regex(regex_str);
  }
  catch (const std::regex_error&)
  {
    throw;
  }

  std::map< std::string, int > filteredDict;

  std::copy_if(currentDict->begin(), currentDict->end(),
    std::inserter(filteredDict, filteredDict.begin()),
    [&pattern](const std::pair< const std::string, int >& p) {
      return std::regex_match(p.first, pattern);
    });

  dicts_[resultDictName] = std::move(filteredDict);
  return true;
}

