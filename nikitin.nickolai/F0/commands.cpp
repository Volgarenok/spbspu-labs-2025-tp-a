#include "commands.hpp"
#include <fstream>
#include <string>
#include <iterator>
#include <algorithm>
#include <utility>
#include <vector>
#include "utilities.hpp"

namespace
{
  bool dictExists(const nikitin::dictionaries &dicts, const std::string &d)
  {
    return dicts.find(d) != dicts.end();
  }
}

void nikitin::createDict(std::istream &in, std::ostream &out, dictionaries &dicts)
{
  std::string dictName;
  if (!(in >> dictName))
  {
    out << "<INVALID COMMAND>";
    return;
  }

  if (dictExists(dicts, dictName))
  {
    out << "<DICT ALREADY EXISTS>";
    return;
  }
  dicts.insert(std::make_pair(dictName, dictionary()));
  out << "<DICT SUCCESSFULLY CREATED>";
}

void nikitin::deleteDict(std::istream &in, std::ostream &out, dictionaries &dicts)
{
  std::string dictName;
  if (!(in >> dictName))
  {
    out << "<INVALID COMMAND>";
    return;
  }
  const std::size_t erased = dicts.erase(dictName);
  out << (erased ? "<DICT SUCCESSFULLY DELETED>" : "<DICT NOT FOUND>");
}

void nikitin::insertWord(std::istream &in, std::ostream &out, dictionaries &dicts)
{
  std::string dictName, word;
  if (!(in >> dictName >> word))
  {
    out << "<INVALID COMMAND>";
    return;
  }
  auto dIt = dicts.find(dictName);
  if (dIt == dicts.end())
  {
    out << "<DICT NOT FOUND>";
    return;
  }
  dictionary &d = dIt->second;
  const bool exists = d.find(word) != d.end();
  if (exists)
  {
    out << "<WORD ALREADY EXISTS>";
    return;
  }
  d.insert(std::make_pair(word, std::vector< std::string >()));
  out << "<WORD SUCCESSFULLY ADDED>";
}

void nikitin::removeWord(std::istream &in, std::ostream &out, dictionaries &dicts)
{
  std::string dictName, word;
  if (!(in >> dictName >> word))
  {
    out << "<INVALID COMMAND>";
    return;
  }
  auto dIt = dicts.find(dictName);
  if (dIt == dicts.end())
  {
    out << "<DICT NOT FOUND>";
    return;
  }
  dictionary &d = dIt->second;
  const std::size_t erased = d.erase(word);
  out << (erased ? "<WORD SUCCESSFULLY REMOVED>" : "<WORD NOT FOUND>");
}

void nikitin::insertTranslation(std::istream &in, std::ostream &out, dictionaries &dicts)
{
  std::string dictName, word, translation;
  if (!(in >> dictName >> word >> translation))
  {
    out << "<INVALID COMMAND>";
    return;
  }
  auto dIt = dicts.find(dictName);
  if (dIt == dicts.end())
  {
    out << "<DICT NOT FOUND>";
    return;
  }
  dictionary &d = dIt->second;
  auto wIt = d.find(word);
  if (wIt == d.end())
  {
    out << "<WORD NOT FOUND>";
    return;
  }
  std::vector< std::string > &ts = wIt->second;
  auto it = std::find(ts.begin(), ts.end(), translation);
  if (it != ts.end())
  {
    out << "<TRANSLATION ALREADY EXISTS>";
    return;
  }
  ts.push_back(translation);
  out << "<TRANSLATION SUCCESSFULLY ADDED>";
}

void nikitin::removeTranslation(std::istream &in, std::ostream &out, dictionaries &dicts)
{
  std::string dictName, word, translation;
  if (!(in >> dictName >> word >> translation))
  {
    out << "<INVALID COMMAND>";
    return;
  }
  auto dIt = dicts.find(dictName);
  if (dIt == dicts.end())
  {
    out << "<DICT NOT FOUND>";
    return;
  }
  dictionary &d = dIt->second;
  auto wIt = d.find(word);
  if (wIt == d.end())
  {
    out << "<WORD NOT FOUND>";
    return;
  }
  std::vector< std::string > &ts = wIt->second;
  auto it = std::find(ts.begin(), ts.end(), translation);
  if (it == ts.end())
  {
    out << "<TRANSLATION NOT FOUND>";
    return;
  }
  ts.erase(it);
  out << "<TRANSLATION SUCCESSFULLY REMOVED>";
}

void nikitin::search(std::istream &in, std::ostream &out, const dictionaries &dicts)
{
  std::string dictName, word;
  if (!(in >> dictName >> word))
  {
    out << "<INVALID COMMAND>";
    return;
  }
  auto dIt = dicts.find(dictName);
  if (dIt == dicts.end())
  {
    out << "<DICT NOT FOUND>";
    return;
  }
  const dictionary &d = dIt->second;
  auto wIt = d.find(word);
  if (wIt == d.end())
  {
    out << "<WORD NOT FOUND>";
    return;
  }

  out << "<TRANSLATIONS: ";
  printTranslations(wIt->second, out);
  out << ">";
}

void nikitin::printSize(std::istream &in, std::ostream &out, const dictionaries &dicts)
{
  std::string dictName;
  if (!(in >> dictName))
  {
    out << "<INVALID COMMAND>";
    return;
  }
  auto dIt = dicts.find(dictName);
  if (dIt == dicts.end())
  {
    out << "<DICT NOT FOUND>";
    return;
  }
  out << "<DICT SIZE: " << dIt->second.size() << ">";
}

void nikitin::printDict(std::istream &in, std::ostream &out, const dictionaries &dicts)
{
  std::string dictName;
  if (!(in >> dictName))
  {
    out << "<INVALID COMMAND>";
    return;
  }
  auto dIt = dicts.find(dictName);
  if (dIt == dicts.end())
  {
    out << "<DICT NOT FOUND>";
    return;
  }

  const auto dict = dIt->second;
  out << "<DICT CONTENT: ";

  if (!dict.empty())
  {
    const auto firstWord = *dict.begin();
    printDictionary(dict, out);
  }

  out << ">";
}

void nikitin::saveDict(std::istream &in, std::ostream &out, const dictionaries &dicts)
{
  std::string dictName, filename;
  if (!(in >> dictName >> filename))
  {
    out << "<INVALID COMMAND>";
    return;
  }
  auto dIt = dicts.find(dictName);
  if (dIt == dicts.end())
  {
    out << "<DICT NOT FOUND>";
    return;
  }
  std::ofstream file(filename);
  if (!file)
  {
    out << "<INVALID COMMAND>";
    return;
  }

  printDictionary(dIt->second, file);
  out << "<DICT SUCCESSFULLY SAVED>";
}

void nikitin::subtract(std::istream &in, std::ostream &out, dictionaries &dicts)
{
  std::string a, b, res;
  if (!(in >> res >> a >> b))
  {
    out << "<INVALID COMMAND>";
    return;
  }
  auto ia = dicts.find(a);
  auto ib = dicts.find(b);
  if (ia == dicts.end() || ib == dicts.end())
  {
    out << "<DICT NOT FOUND>";
    return;
  }
  dictionary result {};
  auto daBegin = ia->second.begin(), daEnd = ia->second.end();
  auto dbBegin = ib->second.begin(), dbEnd = ib->second.end();
  std::set_difference(daBegin, daEnd, dbBegin, dbEnd, std::inserter(result, result.begin()));
  dicts[res] = std::move(result);
  out << "<MERGE SUCCESSFUL>";
}

void nikitin::intersect(std::istream &in, std::ostream &out, dictionaries &dicts)
{
  std::string res, a, b;
  if (!(in >> res >> a >> b))
  {
    out << "<INVALID COMMAND>";
    return;
  }
  auto ia = dicts.find(a);
  auto ib = dicts.find(b);
  if (ia == dicts.end() || ib == dicts.end())
  {
    out << "<DICT NOT FOUND>";
    return;
  }
  dictionary result {};
  auto daBegin = ia->second.begin(), daEnd = ia->second.end();
  auto dbBegin = ib->second.begin(), dbEnd = ib->second.end();
  std::set_intersection(daBegin, daEnd, dbBegin, dbEnd, std::inserter(result, result.begin()));
  dicts[res] = std::move(result);
  out << "<JOIN SUCCESSFUL>";
}

void nikitin::unite(std::istream &in, std::ostream &out, dictionaries &dicts)
{
  std::string res, a, b;
  if (!(in >> res >> a >> b))
  {
    out << "<INVALID COMMAND>";
    return;
  }
  auto ia = dicts.find(a);
  auto ib = dicts.find(b);
  if (ia == dicts.end() || ib == dicts.end())
  {
    out << "<DICT NOT FOUND>";
    return;
  }
  dictionary result {};
  auto daBegin = ia->second.begin(), daEnd = ia->second.end();
  auto dbBegin = ib->second.begin(), dbEnd = ib->second.end();
  std::set_union(daBegin, daEnd, dbBegin, dbEnd, std::inserter(result, result.begin()));
  dicts[res] = std::move(result);
  out << "<JOIN SUCCESSFUL>";
}

void nikitin::loadCommand(std::istream &in, std::ostream &out, dictionaries &dicts)
{
  std::string dictname, filename;
  if (!(in >> dictname >> filename) || dicts.count(dictname))
  {
    out << "<INVALID COMMAND>";
    return;
  }

  std::ifstream file(filename);
  if (!file)
  {
    out << "<INVALID COMMAND>";
    return;
  }

  const std::string content { std::istream_iterator< char > { file }, std::istream_iterator< char > {} };

  const std::regex nl(R"(\r?\n)");
  using line_it = std::sregex_token_iterator;
  std::vector< std::string > lines {};
  std::copy(line_it { content.begin(), content.end(), nl, -1 }, line_it {}, std::back_inserter(lines));

  dictionary d;
  bool bad = false;
  std::for_each(lines.begin(), lines.end(), LineToDict { d, bad });

  if (bad)
  {
    out << "<INVALID COMMAND>";
    return;
  }

  dicts.emplace(dictname, std::move(d));
}
