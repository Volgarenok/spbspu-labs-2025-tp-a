#include "commands.h"
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
