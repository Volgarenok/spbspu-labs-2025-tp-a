#include "commands.hpp"
#include <ostream>
#include <string>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <vector>
#include <fstream>

namespace
{
  using osIt = std::ostream_iterator< std::string >;
  const std::string& extractDict(const kiselev::Dicts::value_type& val)
  {
    return val.first;
  }

  struct WordPrinter
  {
    std::ostream& out;
    void operator()(const std::string& word)
    {
      out << " " << word;
    }
  };

  struct DictPrinter
  {
    std::ostream& out;

    void operator()(const kiselev::Dict::value_type& val) const
    {
      out << val.first;
      if (!val.second.empty())
      {
        std::for_each(val.second.begin(), val.second.end(), WordPrinter{ out });
      }
      out << "\n";
    }
  };

  struct TranslationFinder
  {
    const std::string& word;
    std::vector<std::string>& translations;
    void operator()(const kiselev::Dict::value_type& val) const
    {
      if (std::find(val.second.begin(), val.second.end(), word) != val.second.end())
      {
        translations.push_back(val.first);
      }
    }
  };

  struct TranslationChecker
  {
    const std::string& str;
    bool operator()(const kiselev::Dict::value_type& val)
    {
      return std::find(val.second.begin(), val.second.end(), str) == val.second.end();
    }
  };

  struct VectorChecker
  {
    const std::vector<std::string>& vec;
    bool operator()(const std::string& trans) const
    {
      return std::find(vec.begin(), vec.end(), trans) == vec.end();
    }
  };
  struct DictMerger
  {
    kiselev::Dict operator()(kiselev::Dict res, const kiselev::Dict::value_type& val)
    {
      auto it = res.find(val.first);
      if (it == res.end())
      {
        res.insert(val);
      }
      else
      {
        std::vector<std::string> newTranslations;
        VectorChecker check{ it->second };
        std::copy_if(val.second.begin(), val.second.end(), std::back_inserter(newTranslations), check);
        it->second.insert(it->second.end(), newTranslations.begin(), newTranslations.end());
      }
      return res;
    }
  };

  kiselev::Dict unionTwoDict(const kiselev::Dict& dict1, const kiselev::Dict& dict2)
  {
    return std::accumulate(dict2.begin(), dict2.end(), dict1, DictMerger{});
  }

  struct LetterSearcher {
    const std::string& letters;
    std::ostream& out;
    bool operator()(bool found, const kiselev::Dict::value_type& val)
    {
      if (val.first.rfind(letters, 0) == 0)
      {
        DictPrinter{ out }(val);
        return true;
      }
      return found;
    }
  };

  struct TranslationMerger
  {
    kiselev::Dict& dict;
    bool& allExist;
    void operator()(const kiselev::Dict::value_type& val)
    {
      auto val2 = dict.find(val.first);
      if (val2 == dict.end())
      {
        dict[val.first] = val.second;
        allExist = false;
      }
      else
      {
        std::vector<std::string> newTranslations;
        std::copy_if(val.second.begin(), val.second.end(), std::back_inserter(newTranslations), VectorChecker{ val2->second });
        if (!newTranslations.empty())
        {
          val2->second.insert(val2->second.end(), newTranslations.begin(), newTranslations.end());
          allExist = false;
        }
      }
    }
  };

  struct DictIntersector
  {
    const kiselev::Dict& dict;
    kiselev::Dict res{};
    void operator()(const kiselev::Dict::value_type& val)
    {
      auto it = dict.find(val.first);
      if (it != dict.end())
      {
        res[val.first] = val.second;
        std::vector<std::string> translations;
        std::set_intersection(
          val.second.begin(), val.second.end(), it->second.begin(), it->second.end(), std::back_inserter(translations));
        if (!translations.empty())
        {
          res[val.first] = translations;
        }
      }
    }
  };

  kiselev::Dict intersectTwoDict(const kiselev::Dict& dict1, const kiselev::Dict& dict2)
  {
    DictIntersector intersector{ dict2 };
    std::for_each(dict1.begin(), dict1.end(), intersector);
    return intersector.res;
  }

  struct DictComplementer
  {
    const kiselev::Dict& dict;
    bool operator()(const kiselev::Dict::value_type& val) const
    {
      return dict.find(val.first) == dict.end();
    }
  };
}
void kiselev::doNewDict(std::istream& in, std::ostream& out, Dicts& dicts)
{
  std::string newDict;
  in >> newDict;
  if (dicts.find(newDict) != dicts.end())
  {
    out << "<DICTIONARY ALREADY EXISTS>\n";
    return;
  }
  dicts[newDict];
}

void kiselev::doDeleteDict(std::istream& in, std::ostream& out, Dicts& dicts)
{
  std::string dict;
  in >> dict;
  if (dicts.find(dict) == dicts.end())
  {
    out << "<DICTIONARY NOT FOUND>\n";
    return;
  }
  dicts.erase(dict);
}

void kiselev::doAddElement(std::istream& in, std::ostream& out, Dicts& dicts)
{
  std::string nameDict;
  std::string eng;
  std::string rus;
  in >> nameDict >> eng >> rus;
  auto dictIt = dicts.find(nameDict);
  if (dictIt == dicts.end())
  {
    out << "<DICTIONARY NOT FOUND>\n";
    return;
  }
  Dict dict = dictIt->second;
  auto engIt = dict.find(eng);
  if (engIt == dict.end())
  {
    std::vector< std::string > translations;
    translations.push_back(rus);
    dict[eng] = translations;
  }
  else
  {
    engIt->second.push_back(rus);
  }
}

void kiselev::doDeleteElement(std::istream& in, std::ostream& out, Dicts& dicts)
{
  std::string nameDict;
  std::string eng;
  in >> nameDict >> eng;
  auto dictIt = dicts.find(nameDict);
  if (dictIt == dicts.end())
  {
    out << "<DICTIONARY NOT FOUND>\n";
    return;
  }
  Dict dict = dictIt->second;
  auto engIt = dict.find(eng);
  if (engIt == dict.end())
  {
    out << "<ENGWORD NOT FOUND>\n";
    return;
  }
  dict.erase(eng);
}

void kiselev::doListDict(std::ostream& out, const Dicts& dicts)
{
  if (dicts.empty())
  {
    out << "<EMPTY>\n";
    return;
  }
  std::transform(dicts.begin(), dicts.end(), osIt(out, "\n"), extractDict);
}

void kiselev::doPrintDict(std::istream& in, std::ostream& out, const Dicts& dicts)
{
  std::string nameDict;
  in >> nameDict;
  auto dictIt = dicts.find(nameDict);
  if (dictIt == dicts.cend())
  {
    out << "<DICTIONARY NOT FOUND>\n";
    return;
  }
  out << nameDict << '\n';
  std::for_each(dictIt->second.begin(), dictIt->second.end(), DictPrinter{ out });
}

void kiselev::doTranslateWord(std::istream& in, std::ostream& out, const Dicts& dicts)
{
  std::string nameDict;
  std::string word;
  in >> nameDict >> word;
  auto dictIt = dicts.find(nameDict);
  if (dictIt == dicts.cend())
  {
    out << "<DICTIONARY NOT FOUND>\n";
    return;
  }
  const Dict dict = dictIt->second;
  auto engIt = dict.find(word);
  if (engIt != dict.cend())
  {
    out << *engIt->second.begin();
    std::for_each(std::next(engIt->second.begin()), engIt->second.end(), WordPrinter{ out });
    out << "\n";
    return;
  }
  std::vector< std::string > translations;
  std::for_each(dict.begin(), dict.end(), TranslationFinder{ word, translations });
  if (!translations.empty())
  {
    out << *translations.begin();
    std::for_each(std::next(engIt->second.begin()), engIt->second.end(), WordPrinter{ out });
    out << "\n";
    return;
  }
  out << "<WORD NOT FOUND>\n";
}

void kiselev::doUnionDict(std::istream& in, std::ostream& out, Dicts& dicts)
{
  std::string nameNewDict;
  std::string firstDict;
  std::string secondDict;
  in >> nameNewDict >> firstDict >> secondDict;
  if (dicts.find(nameNewDict) != dicts.end())
  {
    out << "<DICTIONARY ALREADY EXISTS>\n";
    return;
  }
  auto first = dicts.find(firstDict);
  auto second = dicts.find(secondDict);
  if (first == dicts.end() || second == dicts.end())
  {
    out << "<DICTIONARY NOT FOUND>\n";
    return;
  }
  Dict res = unionTwoDict(first->second, second->second);
  std::string nextDict;
  while (in >> nextDict)
  {
    auto it = dicts.find(nextDict);
    if (it == dicts.end())
    {
      out << "<DICTIONARY NOT FOUND>\n";
      return;
    }
    res = unionTwoDict(res, it->second);
    if (in.get() == '\n')
    {
      break;
    }
  }
  dicts[nameNewDict] = res;
}

void kiselev::doSaveDict(std::istream& in, std::ostream& out, const Dicts& dicts)
{
  std::string fileName;
  std::string dictName;
  in >> dictName >> fileName;
  auto dictIt = dicts.find(dictName);
  if (dictIt == dicts.cend())
  {
    out << "<DICTIONARY NOT FOUND>\n";
    return;
  }
  std::ofstream file(fileName);
  if (!file)
  {
    out << "<FILE ERROR>\n";
    return;
  }
  file << dictName << "\n";
  Dict dict = dictIt->second;
  out << dictName << '\n';
  std::for_each(dictIt->second.begin(), dictIt->second.end(), DictPrinter{ out });
  file << "\n";
}

void kiselev::doCountWord(std::istream& in, std::ostream& out, const Dicts& dicts)
{
  std::string dictName;
  in >> dictName;
  auto it = dicts.find(dictName);
  if (it == dicts.cend())
  {
    out << "<DICTIONARY NOT FOUND>\n";
    return;
  }
  out << it->second.size() << "\n";
}

void kiselev::doSearchLetter(std::istream& in, std::ostream& out, const Dicts& dicts)
{
  std::string dictName;
  std::string letters;
  in >> dictName >> letters;
  auto dictIt = dicts.find(dictName);
  if (dictIt == dicts.cend())
  {
    out << "<DICTIONARY NOT FOUND>\n";
    return;
  }
  Dict dict = dictIt->second;
  bool found = std::accumulate(dict.begin(), dict.end(), false, LetterSearcher{ letters, out });
  if (!found)
  {
    out << "<WORD NOT FOUND>\n";
  }
}

void kiselev::doLoadDict(std::istream& in, std::ostream& out, Dicts& dicts)
{
  std::string fileName;
  in >> fileName;
  std::ifstream file(fileName);
  if (!file)
  {
    out << "<FILE ERROR>\n";
    return;
  }

  std::string dictName;
  while (file >> dictName)
  {
    Dict dict;
    std::string eng;
    while (file >> eng)
    {
      std::vector<std::string> ruswords;
      std::copy(std::istream_iterator<std::string>(file), std::istream_iterator<std::string>(), std::back_inserter(ruswords));
      if (!ruswords.empty() && ruswords.back().back() == '\n')
      {
        ruswords.back().pop_back();
      }
      dict[eng] = ruswords;
      if (file.get() == '\n')
      {
        break;
      }
      else
      {
        file.unget();
      }
    }
    auto it = dicts.find(dictName);
    if (it != dicts.end())
    {
      bool allExist = true;
      std::for_each(dict.begin(), dict.end(), TranslationMerger{ it->second, allExist });
      if (allExist)
      {
        out << "<DICTIONARY ALREADY EXISTS>\n";
      }
    }
    else
    {
      dicts[dictName] = dict;
    }
  }
}

void kiselev::doClearDict(std::istream& in, std::ostream& out, Dicts& dicts)
{
  std::string dictName;
  in >> dictName;
  auto it = dicts.find(dictName);
  if (it == dicts.end())
  {
    out << "<DICTIONARY NOT FOUND>\n";
    return;
  }
  it->second.clear();
}

void kiselev::doIntersectDict(std::istream& in, std::ostream& out, Dicts& dicts)
{
  std::string nameNewDict;
  std::string firstDict;
  std::string secondDict;
  in >> nameNewDict >> firstDict >> secondDict;
  if (dicts.find(nameNewDict) != dicts.end())
  {
    out << "<DICTIONARY ALREADY EXISTS>\n";
    return;
  }
  auto first = dicts.find(firstDict);
  auto second = dicts.find(secondDict);
  if (first == dicts.end() || second == dicts.end())
  {
    out << "<DICTIONARY NOT FOUND>\n";
    return;
  }
  Dict res = intersectTwoDict(first->second, second->second);
  std::string nextDict;
  while (in >> nextDict)
  {
    auto it = dicts.find(nextDict);
    if (it == dicts.end())
    {
      out << "<DICTIONARY NOT FOUND>\n";
      return;
    }
    res = intersectTwoDict(res, it->second);
    if (in.get() == '\n')
    {
      break;
    }
  }
  dicts[nameNewDict] = res;
}

void kiselev::doComplementDict(std::istream& in, std::ostream& out, Dicts& dicts)
{
  std::string nameNewDict;
  std::string firstDict;
  std::string secondDict;
  in >> nameNewDict >> firstDict >> secondDict;
  if (dicts.find(nameNewDict) != dicts.end())
  {
    out << "<DICTIONARY ALREADY EXISTS>\n";
    return;
  }
  auto first = dicts.find(firstDict);
  auto second = dicts.find(secondDict);
  if (first == dicts.end() || second == dicts.end())
  {
    out << "<DICTIONARY NOT FOUND>\n";
    return;
  }

  Dict res;
  std::copy_if(first->second.begin(), first->second.end(), std::inserter(res, res.end()), DictComplementer{ second->second });
  dicts[nameNewDict] = res;
}
