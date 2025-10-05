#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <exception>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>
#include <commands.h>
#include "dictionary.h"

namespace
{
  using namespace fedorova;

  struct IsInRange
  {
    std::string start;
    std::string end;
    bool operator()(const std::pair<std::string, WordSet>& pair) const
    {
      return pair.first >= start && pair.first <= end;
    }
  };

  struct IsNotInRange
  {
    std::string start;
    std::string end;
    bool operator()(const std::pair<std::string, WordSet>& pair) const
    {
      return pair.first < start || pair.first > end;
    }
  };

  struct StartsWith
  {
    std::string prefix;
    bool operator()(const std::pair<std::string, WordSet>& pair) const
    {
      return pair.first.find(prefix) == 0;
    }
  };

  WordSet::const_iterator findTranslate(const WordSet& list, const std::string& word)
  {
    return std::find(list.begin(), list.end(), word);
  }

  bool hasSubword(const std::string& word, const std::string& subword)
  {
    return word.find(subword) != std::string::npos;
  }

  bool hasWord(const WordSet& list, const std::string& word)
  {
    return std::find(list.begin(), list.end(), word) != list.end();
  }

  bool hasAnyWords(const WordSet& translates, const std::pair< std::string, WordSet >& pair)
  {
    return std::any_of(translates.begin(), translates.end(), std::bind(hasWord, pair.second, _1));
  }

  bool hasKey(const Dictionary& dict, const std::string& key)
  {
    return dict.find(key) != dict.end();
  }

  bool compareKeys(const std::pair< std::string, WordSet>& pair1, const std::pair< std::string, WordSet>& pair2)
  {
    return pair1.first < pair2.first;
  }

  template< class T >
  std::string returnName(const std::pair< std::string, T >& p)
  {
    return p.first;
  }

  Dictionary returnDictionary(const DictSet& set, const std::string& name)
  {
    return set.at(name);
  }

  WordSet returnWordSet(const Dictionary& dict, const std::string& word)
  {
    return dict.at(word);
  }

  WordSet unionLists(WordSet& list1, WordSet& list2)
  {
    list1.sort();
    list2.sort();
    WordSet res;
    std::set_union(list1.begin(), list1.end(), list2.begin(), list2.end(), std::back_inserter(res));
    return res;
  }

  WordSet intersectLists(WordSet& list1, WordSet& list2)
  {
    list1.sort();
    list2.sort();
    WordSet res;
    std::set_intersection(list1.begin(), list1.end(), list2.begin(), list2.end(), std::back_inserter(res));
    return res;
  }

  std::pair< std::string, WordSet > unionListDict(const std::pair< std::string, WordSet >& pair,
    const Dictionary& unioned)
  {
    WordSet list1 = pair.second;
    WordSet list2 = unioned.at(pair.first);
    WordSet res = unionLists(list1, list2);
    return { pair.first, res };
  }

  std::pair< std::string, WordSet > intersectListDict(const std::pair< std::string, WordSet >& pair, const Dictionary& intersected)
  {
    WordSet list1 = pair.second;
    WordSet list2 = intersected.at(pair.first);
    WordSet res = intersectLists(list1, list2);
    return { pair.first, res };
  }
}

void fedorova::add_word(std::istream& is, std::ostream& os, DictSet& set)
{
  std::string word, translate, name;
  is >> word >> translate >> name;
  if (!is)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }

  Dictionary& dict = set.at(name);
  auto it_word = dict.find(word);
  if (it_word == dict.end())
  {
    WordSet translates{ translate };
    dict.emplace(word, translates);
    os << "<WORD AND TRANSLATION HAVE ALREADY BEEN ADDED>";
    return;
  }

  WordSet& translates = it_word->second;
  if (findTranslate(translates, translate) == translates.cend())
  {
    translates.push_back(translate);
    os << "<TRANSLATE WAS ADDED>";
    return;
  }

  os << "<WORD AND TRANSLATE WERE ADDED>";
}

void fedorova::delete_word(std::istream& is, std::ostream& os, DictSet& set)
{
  std::string word, name;
  is >> word >> name;
  if (!is)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }

  Dictionary& dict = set.at(name);
  dict.at(word);
  dict.erase(word);

  os << "<SUCCESSFULLY DELETED>";
}

void fedorova::translate(std::istream& is, std::ostream& os, const DictSet& set)
{
  std::string word, name;
  is >> word >> name;
  if (!is)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }

  const Dictionary& dict = set.at(name);

  os << dict.at(word);
}

void fedorova::see_dict(std::istream& is, std::ostream& os, const DictSet& set)
{
  std::string name;
  is >> name;
  if (!is)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }

  const Dictionary& dict = set.at(name);
  if (dict.empty())
  {
    return;
  }

  WordSet words;
  std::transform(dict.begin(), dict.end(), std::back_inserter(words), returnName< WordSet >);
  DictContent word_letters;
  for (const auto& word : words)
  {
    if (!word.empty())
    {
      word_letters[word[0]].push_back(word);
    }
  }

  os << word_letters;
}

void fedorova::start_with(std::istream& is, std::ostream& os, const DictSet& set)
{
  std::string name, prefix;
  is >> name >> prefix;
  if (!is)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }

  const Dictionary& dict = set.at(name);
  Dictionary result;

  StartsWith predicate{ prefix };
  std::copy_if(dict.begin(), dict.end(), std::inserter(result, result.end()), predicate);

  if (result.empty())
  {
    os << "<WORD WAS NOT FOUND>";
    return;
  }

  os << result;
}

void fedorova::change_translation(std::istream& is, std::ostream& os, DictSet& set)
{
  std::string word, new_word, name;
  is >> word >> new_word >> name;
  if (!is)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }

  Dictionary& dict = set.at(name);
  dict[new_word] = dict.at(word);
  dict.erase(word);

  os << "<SUCCESSFULLY CHANGED>";
}

void fedorova::delete_translate(std::istream& is, std::ostream& os, DictSet& set)
{
  std::string word, translate, name;
  is >> word >> translate >> name;
  if (!is)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }

  Dictionary& dict = set.at(name);
  WordSet& translates = dict.at(word);
  auto it = findTranslate(translates, translate);
  if (it == translates.cend())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }

  translates.erase(it);

  os << "<SUCCESSFULLY DELETED>";
}

void fedorova::merge(std::istream& is, std::ostream& os, DictSet& set)
{
  std::string newname, name1, name2;
  is >> newname >> name1 >> name2;
  if (!is)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }

  const Dictionary& dict1 = set.at(name1);
  const Dictionary& dict2 = set.at(name2);
  Dictionary difference, tmp, unioned;

  auto d_first = std::inserter(difference, difference.end());
  std::set_difference(dict1.begin(), dict1.end(), dict2.begin(), dict2.end(), d_first, compareKeys);

  d_first = std::inserter(tmp, tmp.end());
  std::set_intersection(dict1.begin(), dict1.end(), dict2.begin(), dict2.end(), d_first, compareKeys);
  std::transform(tmp.begin(), tmp.end(), std::inserter(unioned, unioned.end()), std::bind(unionListDict, _1, dict2));

  unioned.insert(difference.begin(), difference.end());
  set[newname] = unioned;

  os << "<SUCCESSFULLY COMBINED>";
}

void fedorova::export_range(std::istream& is, std::ostream& os, const DictSet& set)
{
  std::string start, end, name;
  is >> start >> end >> name;
  if (!is)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }

  const Dictionary& dict = set.at(name);
  Dictionary result;

  IsInRange predicate{ start, end };
  std::copy_if(dict.begin(), dict.end(), std::inserter(result, result.end()), predicate);

  os << result;
}

void fedorova::translate_in_dicts(std::istream& is, std::ostream& os, const DictSet& set)
{
  WordSet names;
  std::string word;
  is >> word >> names;
  if (!is)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }

  std::list< Dictionary > dicts, suitable;
  std::transform(names.begin(), names.end(), std::back_inserter(dicts), std::bind(returnDictionary, set, _1));

  auto d_first1 = std::inserter(suitable, suitable.end());
  std::copy_if(dicts.begin(), dicts.end(), d_first1, std::bind(hasKey, _1, word));

  std::list< WordSet > translate_list;
  auto d_first2 = std::back_inserter(translate_list);
  std::transform(suitable.begin(), suitable.end(), d_first2, std::bind(returnWordSet, _1, word));

  WordSet translates;
  translates = std::accumulate(translate_list.begin(), translate_list.end(), translates, unionLists);
  if (translates.empty())
  {
    os << "<WORD WAS NOT FOUND>";
    return;
  }

  os << translates;
}

void fedorova::create_dict(std::istream& is, std::ostream& os, DictSet& set)
{
  std::string name;
  is >> name;
  if (!is)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }

  if (set.find(name) != set.end())
  {
    os << "<DICTIONARY IS ALREADY EXISTING>";
    return;
  }

  Dictionary dict;
  set.emplace(name, dict);

  os << "<SUCCESSFULLY CREATED>";
}

void fedorova::delete_dict(std::istream& is, std::ostream& os, DictSet& set)
{
  std::string name;
  is >> name;
  if (!is)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }

  set.at(name);
  set.erase(name);

  os << "<SUCCESSFULLY DELETED>";
}

void fedorova::list_dicts(std::ostream& os, const DictSet& set)
{
  if (set.empty())
  {
    return;
  }

  WordSet names;
  std::transform(set.begin(), set.end(), std::back_inserter(names), returnName< Dictionary >);

  os << names;
}

void fedorova::size_of_dict(std::istream& is, std::ostream& os, const DictSet& set)
{
  std::string name;
  is >> name;
  if (!is)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }

  const Dictionary& dict = set.at(name);
  if (dict.size() == 0)
  {
    os << "<DICTIONARY IS EMPTY>";
    return;
  }

  os << dict.size();
}

void fedorova::contains(std::istream& is, std::ostream& os, const DictSet& set)
{
  std::string subword, name;
  is >> subword >> name;
  if (!is)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }

  const Dictionary& dict = set.at(name);
  WordSet words, suitable;
  std::transform(dict.begin(), dict.end(), std::back_inserter(words), returnName< WordSet >);
  std::copy_if(words.begin(), words.end(), std::back_inserter(suitable), std::bind(hasSubword, _1, subword));
  if (suitable.empty())
  {
    os << "<WORD WAS NOT FOUND>";
    return;
  }

  os << suitable;
}

void fedorova::add_translate(std::istream& is, std::ostream& os, DictSet& set)
{
  std::string word, translate, name;
  is >> word >> translate >> name;
  if (!is)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }

  Dictionary& dict = set.at(name);
  WordSet& translates = dict.at(word);
  if (findTranslate(translates, translate) != translates.cend())
  {
    os << "<TRANSLATE WAS ALREADY ADDED>";
    return;
  }

  translates.push_back(translate);

  os << "<TRANSLATE HAS ALREADY BEEN ADDED>";
}

void fedorova::export_no_range(std::istream& is, std::ostream& os, const DictSet& set)
{
  std::string start, end, name;
  is >> start >> end >> name;
  if (!is)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }

  const Dictionary& dict = set.at(name);
  Dictionary result;

  IsNotInRange predicate{ start, end };
  std::copy_if(dict.begin(), dict.end(), std::inserter(result, result.end()), predicate);

  os << result;
}

void fedorova::print_help(std::ostream& os)
{
  os << std::setw(65) << std::left << "create <dict> " << "create new empty dictionary\n";
  os << std::setw(65) << std::left << "list_dict" << "print list of current dictionaries\n";
  os << std::setw(65) << std::left << "size_of_dict <name> " << "print dictionary size\n";
  os << std::setw(65) << std::left << "delete_dict <name> " << "remove existing dictionary\n";
  os << std::setw(65) << std::left << "add_word <word> <translate> <dict>";
  os << "add the word and its translation to the dictionary\n";
  os << std::setw(65) << std::left << "change_translation <rus_translation< <new_rus_translation> <dict> ";
  os << "replace word is dictionary by new_word\n";
  os << std::setw(65) << std::left << "translate <word> <dict>";
  os << "print the translation of a word from the dictionary\n";
  os << std::setw(65) << std::left << "contains <letters> <dict>";
  os << "print words that contain subword is dictionary\n";
  os << std::setw(65) << std::left << "delete_word <word> <dict>";
  os << "delete an existing word from an existing dictionary\n";
  os << std::setw(65) << std::left << "add_translate <word> <translate> <dict> ";
  os << "add new translate to existing word is dictionary\n";
  os << std::setw(65) << std::left << "start_with <prefix> <dict> ";
  os << "finds all English words that start with prefix\n";
  os << std::setw(65) << std::left << "delete_translate <word> <translate> <dict> ";
  os << "remove existing translate of existing word is existing dictionary\n";
  os << std::setw(65) << std::left << "see_dict <dict> ";
  os << "print out the contents of the dictionary by letter\n";
  os << std::setw(65) << std::left << "translate_in_dicts <dict_count> <dict_1> ... <dict_n> <word> ";
  os << "print word translates from dictionaries\n";
  os << std::setw(65) << std::left << "merge <dict1> <dict2> <union_dict> ";
  os << "create new dictionary as union of dictionary1, dictionary2\n";
  os << std::setw(65) << std::left << "export_range <start> <end> <dict> ";
  os << "displays all words from the dictionary in alphabetical order from start to end\n";
  os << std::setw(65) << std::left << "export_no_range <start> <end> <dict> ";
  os << "displays all words from the dictionary in alphabetical order NOT from start to end\n";
  os << "\nIf dictionaries were load from file, result of programm will be saved is this file.\n";
  os << "is the opposite case result of programm will be saved in file \"dictionaries\".";
}
