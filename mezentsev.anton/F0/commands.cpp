#include "commands.h"
#include <cctype>
#include <cerrno>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <sys/stat.h>

namespace
{
  struct Concatenate
  {
    std::string operator()(const std::string& a, const std::string& b) const
    {
      return a.empty() ? b : a + " " + b;
    }
  };

  struct ConcatenateLines
  {
    std::string operator()(const std::string& a, const std::pair< const std::string, mezentsev::Translations >& word_entry) const
    {
      Concatenate concat;
      std::string translations_str = std::accumulate(word_entry.second.begin(), word_entry.second.end(), std::string(), concat);
      std::string line = word_entry.first + (translations_str.empty() ? "" : " " + translations_str);
      return a.empty() ? line : a + "\n" + line;
    }
  };

  struct IntersectContext
  {
    const mezentsev::Dictionary& dict2;
    mezentsev::Dictionary& result;
  };

  struct IntersectProcessor
  {
    void process(mezentsev::Dictionary::const_iterator it, mezentsev::Dictionary::const_iterator end,
        const IntersectContext& context) const
    {
      if (it == end)
      {
        return;
      }

      auto found = context.dict2.find(it->first);
      if (found != context.dict2.end())
      {
        mezentsev::Translations trans = it->second;
        trans.insert(found->second.begin(), found->second.end());
        context.result[it->first] = trans;
      }

      process(std::next(it), end, context);
    }
  };

  void copyDictionary(mezentsev::Dictionary::const_iterator it, mezentsev::Dictionary::const_iterator end, mezentsev::Dictionary& result)
  {
    if (it == end)
    {
      return;
    }

    result[it->first] = it->second;
    copyDictionary(std::next(it), end, result);
  }

  void mergeDictionary(mezentsev::Dictionary::const_iterator it, mezentsev::Dictionary::const_iterator end, mezentsev::Dictionary& result)
  {
    if (it == end)
    {
      return;
    }

    auto found = result.find(it->first);
    if (found == result.end())
    {
      result[it->first] = it->second;
    }
    else
    {
      found->second.insert(it->second.begin(), it->second.end());
    }

    mergeDictionary(std::next(it), end, result);
  }

  void collectSuggestions(const mezentsev::Dictionary& dict, mezentsev::Dictionary::const_iterator it,
      const std::string& prefix, int n, std::vector< std::string >& result)
  {
    if (n == 0 || it == dict.end() || it->first.substr(0, prefix.size()) != prefix)
    {
      return;
    }

    result.push_back(it->first);
    collectSuggestions(dict, std::next(it), prefix, n - 1, result);
  }

  void collectWordsByPrefix(mezentsev::Dictionary::const_iterator it,
      mezentsev::Dictionary::const_iterator end, const std::string& prefix,
      std::vector< std::pair< std::string, mezentsev::Translations > >& result)
  {
    if (it == end || it->first.substr(0, prefix.size()) != prefix)
    {
      return;
    }

    result.push_back(*it);
    collectWordsByPrefix(std::next(it), end, prefix, result);
  }

  void exportWords(const std::vector< std::pair< std::string, mezentsev::Translations > >::const_iterator it,
      const std::vector< std::pair< std::string, mezentsev::Translations > >::const_iterator end,
      mezentsev::Dictionary& target_dict)
  {
    if (it == end)
    {
      return;
    }

    target_dict[it->first] = it->second;
    exportWords(std::next(it), end, target_dict);
  }

  void findDifferences(mezentsev::Dictionary::const_iterator it,
      mezentsev::Dictionary::const_iterator end, const mezentsev::Dictionary& dict2,
      mezentsev::Dictionary& result)
  {
    if (it == end)
    {
      return;
    }

    if (dict2.find(it->first) == dict2.end())
    {
      result[it->first] = it->second;
    }

    findDifferences(std::next(it), end, dict2, result);
  }

  void copyDictionaryEntries(mezentsev::Dictionary::const_iterator it,
      mezentsev::Dictionary::const_iterator end,
      mezentsev::Dictionary& result)
  {
    if (it == end)
    {
      return;
    }

    result[it->first] = it->second;
    copyDictionaryEntries(std::next(it), end, result);
  }

  void mergeDictionaryEntries(mezentsev::Dictionary::const_iterator it,
      mezentsev::Dictionary::const_iterator end, mezentsev::Dictionary& result)
  {
    if (it == end)
    {
      return;
    }

    auto found = result.find(it->first);
    if (found == result.end())
    {
      result[it->first] = it->second;
    }
    else
    {
      found->second.insert(it->second.begin(), it->second.end());
    }

    mergeDictionaryEntries(std::next(it), end, result);
  }

  void split_recursive(const std::string& s,
      char delim, std::vector< std::string >& tokens,
      size_t start = 0)
  {
    size_t pos = s.find(delim, start);

    if (pos == std::string::npos)
    {
      if (start < s.length())
      {
        tokens.push_back(s.substr(start));
      }
      return;
    }

    if (pos > start)
    {
      tokens.push_back(s.substr(start, pos - start));
    }

    split_recursive(s, delim, tokens, pos + 1);
  }

  void addTranslations(const std::vector< std::string >& words,
      size_t current_index, mezentsev::Translations& translations)
  {
    if (current_index >= words.size())
    {
      return;
    }

    translations.insert(words[current_index]);
    addTranslations(words, current_index + 1, translations);
  }

  void processFileLines(std::ifstream& ifs,
      mezentsev::Dictionary& dict, std::string& line)
  {
    if (!std::getline(ifs, line))
    {
      return;
    }

    if (!line.empty())
    {
      std::vector< std::string > words = mezentsev::split(line, ' ');
      if (!words.empty())
      {
        std::string eng_word = words[0];
        mezentsev::Translations translations;
        addTranslations(words, 1, translations);
        dict[eng_word] = translations;
      }
    }

    processFileLines(ifs, dict, line);
  }
}

std::vector< std::string > mezentsev::split(const std::string& s, char delim)
{
  std::vector< std::string > tokens;
  split_recursive(s, delim, tokens);
  return tokens;
}

void mezentsev::addCommand(DictionarySet& dicts, const std::vector< std::string >& tokens)
{
  if (tokens.size() < 4)
  {
    std::cout << "INVALID COMMAND" << std::endl;
    return;
  }
  auto dict_it = dicts.find(tokens[1]);
  if (dict_it == dicts.end())
  {
    std::cout << "DICTIONARY NOT FOUND" << std::endl;
    return;
  }
  Dictionary& dict = dict_it->second;
  auto word_it = dict.find(tokens[2]);
  if (word_it == dict.end())
  {
    dict[tokens[2]] = Translations{ tokens[3] };
  }
  else
  {
    if (word_it->second.find(tokens[3]) != word_it->second.end())
    {
      std::cout << "ALREADY EXISTS" << std::endl;
    }
    else
    {
      word_it->second.insert(tokens[3]);
    }
  }
}

void mezentsev::removeCommand(DictionarySet& dicts, const std::vector< std::string >& tokens)
{
  if (tokens.size() < 3)
  {
    std::cout << "INVALID COMMAND" << std::endl;
    return;
  }
  auto dict_it = dicts.find(tokens[1]);
  if (dict_it == dicts.end())
  {
    std::cout << "DICTIONARY NOT FOUND" << std::endl;
    return;
  }
  Dictionary& dict = dict_it->second;
  auto word_it = dict.find(tokens[2]);
  if (word_it == dict.end())
  {
    std::cout << "WORD NOT FOUND" << std::endl;
    return;
  }
  if (tokens.size() == 3)
  {
    dict.erase(word_it);
  }
  else
  {
    auto trans_it = word_it->second.find(tokens[3]);
    if (trans_it == word_it->second.end())
    {
      std::cout << "NOT FOUND" << std::endl;
    }
    else
    {
      word_it->second.erase(trans_it);
      if (word_it->second.empty())
      {
        dict.erase(word_it);
      }
    }
  }
}

void mezentsev::translateCommand(DictionarySet& dicts, const std::vector< std::string >& tokens)
{
  if (tokens.size() < 3)
  {
    std::cout << "INVALID COMMAND" << std::endl;
    return;
  }
  auto dict_it = dicts.find(tokens[1]);
  if (dict_it == dicts.end())
  {
    std::cout << "DICTIONARY NOT FOUND" << std::endl;
    return;
  }
  const Dictionary& dict = dict_it->second;
  auto word_it = dict.find(tokens[2]);
  if (word_it == dict.end())
  {
    std::cout << "WORD NOT FOUND" << std::endl;
  }
  else
  {
    Concatenate concat;
    std::string result = std::accumulate(word_it->second.begin(), word_it->second.end(), std::string(), concat);
    std::cout << result << std::endl;
  }
}

void mezentsev::listCommand(DictionarySet& dicts, const std::vector< std::string >& tokens)
{
  if (tokens.size() < 2)
  {
    std::cout << "INVALID COMMAND" << std::endl;
    return;
  }
  auto dict_it = dicts.find(tokens[1]);
  if (dict_it == dicts.end())
  {
    std::cout << "DICTIONARY NOT FOUND" << std::endl;
    return;
  }
  const Dictionary& dict = dict_it->second;
  if (dict.empty())
  {
    std::cout << "EMPTY" << std::endl;
  }
  else
  {
    ConcatenateLines concat_lines;
    std::string result = std::accumulate(dict.begin(), dict.end(), std::string(), concat_lines);
    std::cout << result << std::endl;
  }
}

void mezentsev::saveCommand(DictionarySet& dicts, const std::vector< std::string >& tokens)
{
  if (tokens.size() < 3)
  {
    std::cout << "INVALID COMMAND" << std::endl;
    return;
  }
  auto dict_it = dicts.find(tokens[1]);
  if (dict_it == dicts.end())
  {
    std::cout << "DICTIONARY NOT FOUND" << std::endl;
    return;
  }
  std::ofstream ofs(tokens[2]);
  if (!ofs)
  {
    std::cout << "INVALID COMMAND" << std::endl;
    return;
  }
  ofs << tokens[1] << "\n";
  const Dictionary& dict = dict_it->second;
  ConcatenateLines dict_entry_to_string;
  std::string content = std::accumulate(dict.begin(), dict.end(), std::string(), dict_entry_to_string);
  ofs << content;
}

void mezentsev::loadCommand(DictionarySet& dicts, const std::vector< std::string >& tokens)
{
  if (tokens.size() < 3)
  {
    std::cout << "INVALID COMMAND" << std::endl;
    return;
  }

  std::ifstream ifs(tokens[2]);
  if (!ifs)
  {
    std::cout << "FILE IS INVALID" << std::endl;
    return;
  }

  Dictionary dict;
  std::string header;
  std::getline(ifs, header);

  std::string line;
  processFileLines(ifs, dict, line);

  dicts[tokens[1]] = dict;
}

void mezentsev::countCommand(DictionarySet& dicts, const std::vector< std::string >& tokens)
{
  if (tokens.size() < 2)
  {
    std::cout << "INVALID COMMAND" << std::endl;
    return;
  }
  auto dict_it = dicts.find(tokens[1]);
  if (dict_it == dicts.end())
  {
    std::cout << "DICTIONARY NOT FOUND" << std::endl;
    return;
  }
  std::cout << dict_it->second.size() << std::endl;
}

void mezentsev::clearCommand(DictionarySet& dicts, const std::vector< std::string >& tokens)
{
  if (tokens.size() < 2)
  {
    std::cout << "INVALID COMMAND" << std::endl;
    return;
  }
  auto dict_it = dicts.find(tokens[1]);
  if (dict_it == dicts.end())
  {
    std::cout << "DICTIONARY NOT FOUND" << std::endl;
    return;
  }
  dict_it->second.clear();
}

void mezentsev::suggestCommand(DictionarySet& dicts, const std::vector< std::string >& tokens)
{
  if (tokens.size() < 4)
  {
    std::cout << "INVALID COMMAND" << std::endl;
    return;
  }
  auto dict_it = dicts.find(tokens[1]);
  if (dict_it == dicts.end())
  {
    std::cout << "DICTIONARY NOT FOUND" << std::endl;
    return;
  }
  const Dictionary& dict = dict_it->second;
  std::string prefix = tokens[2];
  int n = std::stoi(tokens[3]);
  std::vector< std::string > suggestions;
  auto it = dict.lower_bound(prefix);
  collectSuggestions(dict, it, prefix, n, suggestions);
  suggestions.resize(n, "<EMPTY>");
  std::copy(suggestions.begin(), suggestions.end(), std::ostream_iterator< std::string >(std::cout, "\n"));
}

void mezentsev::mergeCommand(DictionarySet& dicts, const std::vector< std::string >& tokens)
{
  if (tokens.size() < 4)
  {
    std::cout << "INVALID COMMAND" << std::endl;
    return;
  }

  auto it1 = dicts.find(tokens[1]);
  auto it2 = dicts.find(tokens[2]);

  if (it1 == dicts.end() || it2 == dicts.end())
  {
    std::cout << "DICTIONARY NOT FOUND" << std::endl;
    return;
  }

  Dictionary merged;

  copyDictionaryEntries(it1->second.begin(), it1->second.end(), merged);

  mergeDictionaryEntries(it2->second.begin(), it2->second.end(), merged);

  dicts[tokens[3]] = merged;
  dicts.erase(it1);
  dicts.erase(it2);
}

void mezentsev::diffCommand(DictionarySet& dicts, const std::vector< std::string >& tokens)
{
  if (tokens.size() < 4)
  {
    std::cout << "INVALID COMMAND" << std::endl;
    return;
  }

  auto it1 = dicts.find(tokens[1]);
  auto it2 = dicts.find(tokens[2]);

  if (it1 == dicts.end() || it2 == dicts.end())
  {
    std::cout << "DICTIONARY NOT FOUND" << std::endl;
    return;
  }

  Dictionary diff_dict;

  findDifferences(it1->second.begin(), it1->second.end(), it2->second, diff_dict);

  if (diff_dict.empty())
  {
    std::cout << "<INDENTICAL>" << std::endl;
  }
  else
  {
    dicts[tokens[3]] = diff_dict;
  }
}

void mezentsev::copyCommand(DictionarySet& dicts, const std::vector< std::string >& tokens)
{
  if (tokens.size() < 3)
  {
    std::cout << "INVALID COMMAND" << std::endl;
    return;
  }
  auto source_it = dicts.find(tokens[1]);
  if (source_it == dicts.end())
  {
    std::cout << "SOURCE NOT FOUND" << std::endl;
    return;
  }
  dicts[tokens[2]] = source_it->second;
}

void mezentsev::intersectCommand(DictionarySet& dicts, const std::vector< std::string >& tokens)
{
  if (tokens.size() < 4)
  {
    std::cout << "INVALID COMMAND" << std::endl;
    return;
  }

  auto it1 = dicts.find(tokens[1]);
  auto it2 = dicts.find(tokens[2]);

  if (it1 == dicts.end() || it2 == dicts.end())
  {
    std::cout << "DICTIONARY NOT FOUND" << std::endl;
    return;
  }

  Dictionary intersect_dict;
  IntersectContext context{ it2->second, intersect_dict };
  IntersectProcessor processor;

  processor.process(it1->second.begin(), it1->second.end(), context);

  if (intersect_dict.empty())
  {
    std::cout << "<NO INTERSECTION>" << std::endl;
  }
  else
  {
    dicts[tokens[3]] = intersect_dict;
  }
}

void mezentsev::exportCommand(DictionarySet& dicts, const std::vector< std::string >& tokens)
{
  if (tokens.size() < 4)
  {
    std::cout << "INVALID COMMAND" << std::endl;
    return;
  }

  auto source_it = dicts.find(tokens[2]);
  auto target_it = dicts.find(tokens[3]);

  if (source_it == dicts.end() || target_it == dicts.end())
  {
    std::cout << "DICTIONARY NOT FOUND" << std::endl;
    return;
  }

  const Dictionary& source_dict = source_it->second;
  Dictionary& target_dict = target_it->second;
  std::string prefix = tokens[1];

  std::vector< std::pair< std::string, Translations > > words_to_export;
  auto start_it = source_dict.lower_bound(prefix);

  collectWordsByPrefix(start_it, source_dict.end(), prefix, words_to_export);

  if (words_to_export.empty())
  {
    std::cout << "NO WORDS FOUND" << std::endl;
    return;
  }

  exportWords(words_to_export.begin(), words_to_export.end(), target_dict);
}
