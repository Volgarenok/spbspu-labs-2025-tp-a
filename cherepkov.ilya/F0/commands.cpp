#include "commands.hpp"
#include "dictionary_types.hpp"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <set>
#include <vector>
#include <stdexcept>

namespace cherepkov
{

void createDictionary(DictionarySet &dicts, std::istream &in)
{
  std::string name;
  in >> name;

  if (!isValidName(name))
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  if (dicts.count(name) > 0)
  {
    throw std::logic_error("<DICT EXISTS>");
  }

  dicts[name].clear();
}

void deleteDictionary(DictionarySet &dicts, std::istream &in)
{
  std::string name;
  in >> name;

  if (dicts.erase(name) == 0)
  {
    throw std::logic_error("<WRONG DICT>");
  }
}

void insert(DictionarySet &dicts, std::istream &in)
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

void removeWord(DictionarySet &dicts, std::istream &in)
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

void clearDictionary(DictionarySet &dicts, std::istream &in)
{
  std::string name;
  in >> name;

  if (!isValidName(name))
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  auto it = dicts.find(name);
  if (it == dicts.end())
  {
    throw std::logic_error("<WRONG DICT>");
  }

  it->second.clear();
}

void contains(DictionarySet &dicts, std::istream &in, std::ostream &out)
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

void mergeDicts(DictionarySet &dicts, std::istream &in)
{
  std::string line;
  std::getline(in, line);
  std::istringstream ss(line);

  std::string newName;
  if (!(ss >> newName))
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  if (!isValidName(newName))
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  std::vector<std::string> sources;
  std::string s;
  while (ss >> s)
  {
    sources.push_back(s);
  }

  if (sources.empty())
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  for (size_t i = 0; i < sources.size(); ++i)
  {
    if (dicts.find(sources[i]) == dicts.end())
    {
      throw std::logic_error("<WRONG DICT>");
    }
  }

  Dictionary &dest = dicts[newName];
  dest.clear();

  for (size_t i = 0; i < sources.size(); ++i)
  {
    Dictionary &src = dicts[sources[i]];
    for (auto it = src.begin(); it != src.end(); ++it)
    {
      dest[it->first] += it->second;
    }
  }
}

void print(DictionarySet &dicts, std::istream &in, std::ostream &out)
{
  std::string name;
  in >> name;
  cherepkov::printWords(dicts, name, out, 0, /*descending=*/false);
}

void count(DictionarySet &dicts, std::istream &in, std::ostream &out)
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
    throw std::logic_error("<WORD NOT FOUND>");
  }
  else
  {
    out << wIt->second << "\n";
  }
}

void search(DictionarySet &dicts, std::istream &in, std::ostream &out)
{
  Word w;
  std::string name;
  in >> w >> name;

  auto it = dicts.find(name);
  if (it == dicts.end())
  {
    throw std::logic_error("<WRONG DICT>");
  }

  auto wIt = it->second.find(w.text);
  if (wIt == it->second.end())
  {
    throw std::logic_error("<WORD NOT FOUND>");
  }
  else
  {
    out << wIt->second << "\n";
  }
}

void top(DictionarySet &dicts, std::istream &in, std::ostream &out)
{
  std::string name;
  int count = 0;
  in >> name >> count;

  if (in.fail() || count <= 0)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  cherepkov::printWords(dicts, name, out, static_cast< size_t >(count), true);
}

void listTop(DictionarySet &dicts, std::istream &in, std::ostream &out)
{
  std::string name;
  in >> name;

  cherepkov::printWords(dicts, name, out, 0, true);
}

void uniqueMultiple(DictionarySet &dicts, std::istream &in, std::ostream &out)
{
  int N;
  in >> N;
  if (in.fail() || N <= 0)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  std::vector<std::string> names;
  for (int i = 0; i < N; ++i)
  {
    std::string nm;
    if (!(in >> nm))
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    names.push_back(nm);
  }

  for (size_t i = 0; i < names.size(); ++i)
  {
    if (dicts.find(names[i]) == dicts.end())
    {
      throw std::logic_error("<WRONG DICT>");
    }
  }

  std::set<std::string> unionSet;
  for (size_t i = 0; i < names.size(); ++i)
  {
    const Dictionary &d = dicts.at(names[i]);
    for (auto it = d.begin(); it != d.end(); ++it)
    {
      unionSet.insert(it->first);
    }
  }

  out << unionSet.size() << "\n";
}

void intersectDicts(DictionarySet &dicts, std::istream &in)
{
  std::string newName;
  int n;
  if (!(in >> newName >> n))
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  if (!isValidName(newName))
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  if (n <= 0)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  std::vector<std::string> names;
  for (int i = 0; i < n; ++i)
  {
    std::string nm;
    if (!(in >> nm))
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    names.push_back(nm);
  }

  for (size_t i = 0; i < names.size(); ++i)
  {
    if (dicts.find(names[i]) == dicts.end())
    {
      throw std::logic_error("<WRONG DICT>");
    }
  }

  const Dictionary &first = dicts.at(names[0]);
  Dictionary result;
  for (auto it = first.begin(); it != first.end(); ++it)
  {
    const std::string &w = it->first;
    bool presentEverywhere = true;
    for (size_t j = 1; j < names.size(); ++j)
    {
      if (dicts.at(names[j]).count(w) == 0)
      {
        presentEverywhere = false;
        break;
      }
    }
    if (presentEverywhere)
    {
      result[w] = it->second;
    }
  }

  if (result.empty())
  {
    throw std::logic_error("<NO COMMON WORDS>");
  }

  dicts[newName] = result;
}

void diffDicts(DictionarySet &dicts, std::istream &in)
{
  std::string newName, d1, d2;
  if (!(in >> newName >> d1 >> d2))
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  if (!isValidName(newName))
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  auto it1 = dicts.find(d1);
  auto it2 = dicts.find(d2);
  if (it1 == dicts.end() || it2 == dicts.end())
  {
    throw std::logic_error("<WRONG DICT>");
  }

  Dictionary result;
  for (auto it = it1->second.begin(); it != it1->second.end(); ++it)
  {
    if (it2->second.count(it->first) == 0)
    {
      result[it->first] = it->second;
    }
  }

  if (result.empty())
  {
    throw std::logic_error("<NO DIFFERENCE>");
  }

  dicts[newName] = result;
}

void filterByFrequency(DictionarySet &dicts, std::istream &in)
{
  std::string newName, name;
  long long minv, maxv;
  if (!(in >> newName >> name >> minv >> maxv))
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  if (!isValidName(newName))
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  if (minv < 0 || maxv < 0)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  auto it = dicts.find(name);
  if (it == dicts.end())
  {
    throw std::logic_error("<WRONG DICT>");
  }

  Dictionary result;
  for (auto itw = it->second.begin(); itw != it->second.end(); ++itw)
  {
    if (static_cast<long long>(itw->second) >= minv && static_cast<long long>(itw->second) <= maxv)
    {
      result[itw->first] = itw->second;
    }
  }

  if (result.empty())
  {
    throw std::logic_error("<NO MATCHES>");
  }

  dicts[newName] = result;
}

void median(DictionarySet &dicts, std::istream &in, std::ostream &out)
{
  std::string name;
  long long k;
  if (!(in >> name >> k))
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  if (k < 0)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  auto it = dicts.find(name);
  if (it == dicts.end())
  {
    throw std::logic_error("<WRONG DICT>");
  }

  if (it->second.empty())
  {
    throw std::logic_error("<EMPTY DICT>");
  }

  std::vector< std::pair< std::string, size_t > > words(it->second.begin(), it->second.end());

  std::sort(words.begin(), words.end(), CompareByFrequency(false));

  size_t total = words.size();

  size_t mid = (total - 1) / 2;

  long long needed = 2 * k + 1;
  if (static_cast<long long>(total) < needed)
  {
    throw std::logic_error("<NOT ENOUGH WORDS>");
  }

  long long L = static_cast<long long>(mid) - k;
  long long R = static_cast<long long>(mid) + k;

  if (L < 0) { L = 0; R = L + needed - 1; }
  if (R >= static_cast<long long>(total)) { R = static_cast<long long>(total) - 1; L = R - (needed - 1); }

  for (long long i = L; i <= R; ++i)
  {
    out << printPair(words[static_cast<size_t>(i)]) << "\n";
  }
}

}
