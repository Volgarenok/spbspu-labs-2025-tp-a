#include "commands.h"
#include "dictionary.h"

namespace
{
  using namespace fedorova;

  WordSet::const_iterator findTranslate(const WordSet& list, const std::string& word)
  {
    return std::find(list.begin(), list.end(), word);
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
