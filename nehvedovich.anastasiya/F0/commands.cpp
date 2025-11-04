#include "commands.hpp"
#include <iterator>
#include <algorithm>
#include <stdexcept>
#include "support.hpp"

void nehvedovich::createDict(std::istream& in, DictionarySet& dicts)
{
  std::string name;
  in >> name;
  if (!isValidName(name))
  {
    throw std::runtime_error("<INVALID NAME>");
  }
  dicts[name] = Dictionary{};
}

void nehvedovich::deleteDict(std::istream& in, DictionarySet& dicts)
{
  std::string name;
  in >> name;
  if (!isValidName(name))
  {
    throw std::runtime_error("<INVALID NAME>");
  }

  auto it = dicts.find(name);
  if (it == dicts.end())
  {
    throw std::runtime_error("<DICT NOT FOUND>");
  }

  dicts.erase(it);
}

void nehvedovich::add(std::istream& in, DictionarySet& dicts)
{
  std::string dictName, english, russian;
  in >> dictName >> english >> russian;
  if (!in || dictName.empty() || english.empty() || russian.empty())
  {
    throw std::runtime_error("<INVALID INPUT>");
  }

  auto dictIt = dicts.find(dictName);
  if (dictIt == dicts.end())
  {
    throw std::runtime_error("<DICT NOT FOUND>");
  }

  Dictionary& dict = dictIt->second;
  auto& translations = dict[english];

  if (std::find(translations.begin(), translations.end(), russian) != translations.end())
  {
    throw std::runtime_error("<INVALID INPUT>");
  }

  AddTranslation{ translations }(russian);
}

void nehvedovich::remove(std::istream& in, DictionarySet& dicts)
{
  std::string dictName, english;
  in >> dictName >> english;
  if (!in || dictName.empty() || english.empty())
  {
    throw std::runtime_error("<INVALID INPUT>");
  }

  auto dictIt = dicts.find(dictName);
  if (dictIt == dicts.end())
  {
    throw std::runtime_error("<DICT NOT FOUND>");
  }

  Dictionary& dict = dictIt->second;
  auto wordIt = dict.find(english);
  if (wordIt == dict.end())
  {
    throw std::runtime_error("<WORD NOT FOUND>");
  }

  dict.erase(wordIt);
}

void nehvedovich::translate(std::istream& in, const DictionarySet& dicts, std::ostream& out)
{
  std::string dictName, english;
  in >> dictName >> english;

  if (!in || dictName.empty() || english.empty())
  {
    throw std::runtime_error("<INVALID INPUT>");
  }

  auto dictIt = dicts.find(dictName);
  if (dictIt == dicts.end())
  {
    throw std::runtime_error("<DICT NOT FOUND>");
  }

  const Dictionary& dict = dictIt->second;
  auto wordIt = dict.find(english);
  if (wordIt == dict.end())
  {
    throw std::runtime_error("<WORD NOT FOUND>");
  }

  out << formatTranslations(wordIt->second) << "\n";
}

void nehvedovich::contains(std::istream& in, const DictionarySet& dicts, std::ostream& out)
{
  std::string dictName, english;
  in >> dictName >> english;

  if (!in || dictName.empty() || english.empty())
  {
    throw std::runtime_error("<INVALID INPUT>");
  }

  auto dictIt = dicts.find(dictName);
  if (dictIt == dicts.end())
  {
    throw std::runtime_error("<DICT NOT FOUND>");
  }

  const Dictionary& dict = dictIt->second;
  out << (dict.find(english) != dict.end() ? "<YES>\n" : "<NO>\n");
}

void nehvedovich::list(std::istream& in, const DictionarySet& dicts, std::ostream& out)
{
  std::string name;
  in >> name;

  if (!in || name.empty())
  {
    throw std::runtime_error("<INVALID INPUT>");
  }

  auto it = dicts.find(name);
  if (it == dicts.end())
  {
    throw std::runtime_error("<DICT NOT FOUND>");
  }

  const Dictionary& dict = it->second;
  if (dict.empty())
  {
    throw std::runtime_error("<EMPTY DICTIONARY>");
  }

  std::vector< std::pair< std::string, std::vector< std::string > > > words(dict.begin(), dict.end());
  std::sort(words.begin(), words.end(), wordComparator);
  std::transform(words.begin(), words.end(), std::ostream_iterator< std::string >(out, "\n"), WordFormatter(WordFormatter::PrintMode));
}

void nehvedovich::merge(std::istream& in, DictionarySet& dicts)
{
  std::string name1, name2, resultName;
  in >> name1 >> name2 >> resultName;

  auto it1 = dicts.find(name1);
  auto it2 = dicts.find(name2);

  if (it1 == dicts.end() || it2 == dicts.end())
  {
    throw std::runtime_error("<DICT NOT FOUND>");
  }

  const Dictionary& dict1 = it1->second;
  const Dictionary& dict2 = it2->second;

  if (dict1.empty() || dict2.empty())
  {
    throw std::runtime_error("<EMPTY DICTIONARY>");
  }

  Dictionary result = dict1;
  std::vector< std::pair< std::string, std::vector< std::string > > > tmp(dict2.begin(), dict2.end());
  std::transform(tmp.begin(), tmp.end(), tmp.begin(), MergeDictionaries{ result });
  dicts[resultName] = std::move(result);
}

void nehvedovich::quantity(std::istream& in, const DictionarySet& dicts, std::ostream& out)
{
  std::string name;
  in >> name;

  auto it = dicts.find(name);
  if (it == dicts.end())
  {
    throw std::runtime_error("<DICT NOT FOUND>");
  }

  const Dictionary& dict = it->second;
  if (dict.empty())
  {
    throw std::runtime_error("<EMPTY DICTIONARY>");
  }

  out << dict.size() << "\n";
}

void nehvedovich::reverseTranslate(std::istream& in, const DictionarySet& dicts, std::ostream& out)
{
  std::string name, russian;
  in >> name >> russian;

  auto it = dicts.find(name);
  if (it == dicts.end())
  {
    throw std::runtime_error("<DICT NOT FOUND>");
  }

  const Dictionary& dict = it->second;
  if (dict.empty())
  {
    throw std::runtime_error("<EMPTY DICTIONARY>");
  }

  std::vector< std::pair< std::string, std::vector< std::string > > > filtered;
  std::copy_if(dict.begin(), dict.end(), std::back_inserter(filtered), TranslationFilter(TranslationFilter::ContainsMode, 0, russian));

  if (filtered.empty())
  {
    throw std::runtime_error("<WORD NOT FOUND>");
  }

  std::vector< std::string > englishWords;
  englishWords.reserve(filtered.size());

  std::transform(filtered.begin(), filtered.end(), std::back_inserter(englishWords), WordFormatter(WordFormatter::ExtractMode));
  std::sort(englishWords.begin(), englishWords.end());

  out << Join(", ")(englishWords) << "\n";
}

void nehvedovich::edit(std::istream& in, DictionarySet& dicts)
{
  std::string dictName, word, oldTranslation, newTranslation;
  in >> dictName >> word >> oldTranslation >> newTranslation;

  if (!in || dictName.empty() || word.empty() || oldTranslation.empty() || newTranslation.empty())
  {
    throw std::runtime_error("<INVALID INPUT>");
  }

  auto dictIt = dicts.find(dictName);
  if (dictIt == dicts.end())
  {
    throw std::runtime_error("<DICT NOT FOUND>");
  }

  Dictionary& dict = dictIt->second;
  auto wordIt = dict.find(word);
  if (wordIt == dict.end())
  {
    throw std::runtime_error("<WORD NOT FOUND>");
  }

  std::vector< std::string >& translations = wordIt->second;
  auto oldIt = std::find(translations.begin(), translations.end(), oldTranslation);
  if (oldIt == translations.end())
  {
    throw std::runtime_error("<WORD NOT FOUND>");
  }

  *oldIt = newTranslation;
  std::sort(translations.begin(), translations.end());
  translations.erase(std::unique(translations.begin(), translations.end()), translations.end());
}

void nehvedovich::moreTranslations(std::istream& in, const DictionarySet& dicts, std::ostream& out)
{
  std::string name;
  size_t count;
  in >> name >> count;

  auto it = dicts.find(name);
  if (it == dicts.end())
  {
    throw std::runtime_error("<DICT NOT FOUND>");
  }

  const Dictionary& dict = it->second;
  if (dict.empty())
  {
    throw std::runtime_error("<EMPTY DICTIONARY>");
  }

  std::vector< std::pair< std::string, std::vector< std::string > > > filtered;
  std::copy_if(dict.begin(), dict.end(), std::back_inserter(filtered), TranslationFilter(TranslationFilter::MoreThanMode, count));

  if (filtered.empty())
  {
    throw std::runtime_error("<NO WORDS FOUND>");
  }

  std::transform(filtered.begin(), filtered.end(), std::ostream_iterator< std::string >(out, "\n"), WordFormatter(WordFormatter::PrintMode));
}
