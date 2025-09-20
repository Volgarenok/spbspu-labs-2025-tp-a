#include "commands.hpp"
#include <vector>
#include <limits>
#include <iterator>
#include <algorithm>
#include "support.hpp"

void filonova::createDictionary(DictionarySet &dicts, std::istream &in)
{
  std::string name;
  in >> name;

  if (!isValidName(name))
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  dicts[name].clear();
}

void filonova::deleteDictionary(DictionarySet &dicts, std::istream &in)
{
  std::string name;
  in >> name;

  if (dicts.erase(name) == 0)
  {
    throw std::logic_error("<WRONG DICT>");
  }
}

void filonova::insert(DictionarySet &dicts, std::istream &in)
{
  std::string name, filename;
  in >> name >> filename;

  auto it = dicts.find(name);
  if (it == dicts.end())
  {
    throw std::logic_error("<WRONG DICT>");
  }

  std::ifstream file(filename);
  if (!file.is_open())
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  Word w;
  while (file >> w)
  {
    it->second[w.text]++;
  }
}

void filonova::remove(DictionarySet &dicts, std::istream &in)
{
  std::string name;
  Word w;

  in >> name >> w;

  auto it = dicts.find(name);
  if (it == dicts.end())
  {
    throw std::logic_error("<WRONG DICT>");
  }

  auto wIt = it->second.find(w.text);
  if (wIt == it->second.end())
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  it->second.erase(wIt);
}

void filonova::clear(DictionarySet &dicts, std::istream &in)
{
  std::string name;
  in >> name;

  auto it = dicts.find(name);
  if (it == dicts.end())
  {
    throw std::logic_error("<WRONG DICT>");
  }

  if (it->second.empty())
  {
    throw std::logic_error("<EMPTY>");
  }

  it->second.clear();
}

void filonova::contains(DictionarySet &dicts, std::istream &in, std::ostream &out)
{
  std::string name;
  Word w;
  in >> name >> w;

  auto it = dicts.find(name);
  if (it == dicts.end())
  {
    throw std::logic_error("<WRONG DICT>");
  }

  bool found = it->second.count(w.text);
  out << (found ? "<YES>\n" : "<NO>\n");
}

void filonova::merge(DictionarySet &dicts, std::istream &in)
{
  std::string newDict, dict1, dict2;
  if (!(in >> newDict >> dict1 >> dict2))
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  auto it1 = dicts.find(dict1);
  auto it2 = dicts.find(dict2);

  if (!isValidName(newDict) || it1 == dicts.end() || it2 == dicts.end())
  {
    throw std::logic_error("<WRONG DICT>");
  }

  Dictionary &dictNew = dicts[newDict];
  dictNew.clear();

  std::copy(it1->second.begin(), it1->second.end(), std::inserter(dictNew, dictNew.end()));

  std::transform(it2->second.begin(), it2->second.end(), std::inserter(dictNew, dictNew.end()), MergeDictEntry(dictNew));
}

void filonova::print(DictionarySet &dicts, std::istream &in, std::ostream &out)
{
  std::string name;
  in >> name;

  printWords(dicts, name, out, 0, true);
}

void filonova::count(DictionarySet &dicts, std::istream &in, std::ostream &out)
{
  std::string name;
  Word w;
  in >> name >> w;

  auto it = dicts.find(name);
  if (it == dicts.end())
  {
    throw std::logic_error("<WRONG DICT>");
  }

  auto wIt = it->second.find(w.text);
  if (wIt == it->second.end())
  {
    throw std::logic_error("<NOT FOUND>");
  }
  else
  {
    out << wIt->second << "\n";
  }
}

void filonova::top(DictionarySet &dicts, std::istream &in, std::ostream &out)
{
  std::string name;
  int count = 0;
  in >> name >> count;

  if (in.fail() || count <= 0)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  printWords(dicts, name, out, static_cast< size_t >(count), true);
}

void filonova::unique(DictionarySet &dicts, std::istream &in, std::ostream &out)
{
  std::string name;
  in >> name;

  auto it = dicts.find(name);
  if (it == dicts.end())
  {
    throw std::logic_error("<WRONG DICT>");
  }

  out << it->second.size() << "\n";
}

void filonova::mostrare(DictionarySet &dicts, std::istream &in, std::ostream &out)
{
  std::string name;
  int count = 0;
  in >> name >> count;

  if (in.fail() || count <= 0)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  printWords(dicts, name, out, static_cast< size_t >(count), true);
}
void filonova::intersectDictionary(DictionarySet &dicts, std::istream &in)
{
  std::string newDict, dict1, dict2;
  if (!(in >> newDict >> dict1 >> dict2))
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  auto it1 = dicts.find(dict1);
  auto it2 = dicts.find(dict2);

  if (!isValidName(newDict) || it1 == dicts.end() || it2 == dicts.end())
  {
    throw std::logic_error("<WRONG DICT>");
  }

  Dictionary &dictNew = dicts[newDict];
  dictNew.clear();

  std::copy_if(it1->second.begin(), it1->second.end(), std::inserter(dictNew, dictNew.begin()), WordIntersectFilter(it2->second));
}

void filonova::excludeDictionary(DictionarySet &dicts, std::istream &in)
{
  std::string newDict, dict1, dict2;
  if (!(in >> newDict >> dict1 >> dict2))
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  auto it1 = dicts.find(dict1);
  auto it2 = dicts.find(dict2);

  if (!isValidName(newDict) || it1 == dicts.end() || it2 == dicts.end())
  {
    throw std::logic_error("<WRONG DICT>");
  }

  Dictionary &dictNew = dicts[newDict];
  dictNew.clear();

  std::copy_if(it1->second.begin(), it1->second.end(), std::inserter(dictNew, dictNew.begin()), WordExcludeFilter(it2->second));
}
