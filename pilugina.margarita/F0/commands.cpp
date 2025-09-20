#include "commands.hpp"
#include <fstream>
#include <iterator>
#include <string>
#include "utilities.hpp"

namespace pilugina
{
  void createDict(std::istream &in, std::ostream &out, dictionaries &dicts)
  {
    std::string dictName;
    if (!read1(in, dictName))
    {
      out << "<INVALID COMMAND>";
      return;
    }
    const bool exists = dicts.find(dictName) != dicts.end();
    if (exists)
    {
      out << "<DICT ALREADY EXISTS>";
      return;
    }
    dicts.insert(std::make_pair(dictName, dictionary()));
    out << "<DICT SUCCESSFULLY CREATED>";
  }

  void deleteDict(std::istream &in, std::ostream &out, dictionaries &dicts)
  {
    std::string dictName;
    if (!read1(in, dictName))
    {
      out << "<INVALID COMMAND>";
      return;
    }
    const std::size_t erased = dicts.erase(dictName);
    out << (erased ? "<DICT SUCCESSFULLY DELETED>" : "<DICT NOT FOUND>");
  }

  void insertWord(std::istream &in, std::ostream &out, dictionaries &dicts)
  {
    std::string dictName, word;
    if (!read2(in, dictName, word))
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

  void insertTranslation(std::istream &in, std::ostream &out, dictionaries &dicts)
  {
    std::string dictName, word, translation;
    if (!read3(in, dictName, word, translation))
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

  void removeWord(std::istream &in, std::ostream &out, dictionaries &dicts)
  {
    std::string dictName, word;
    if (!read2(in, dictName, word))
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

  void removeTranslation(std::istream &in, std::ostream &out, dictionaries &dicts)
  {
    std::string dictName, word, translation;
    if (!read3(in, dictName, word, translation))
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

  void searchTranslation(std::istream &in, std::ostream &out, const dictionaries &dicts)
  {
    std::string dictName, word;
    if (!read2(in, dictName, word))
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

    out << "<TRANSLATIONS: " << joinedTranslations(wIt->second) << ">";
  }

  void printSizeDict(std::istream &in, std::ostream &out, const dictionaries &dicts)
  {
    std::string dictName;
    if (!read1(in, dictName))
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

  void printDict(std::istream &in, std::ostream &out, const dictionaries &dicts)
  {
    std::string dictName;
    if (!read1(in, dictName))
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
      out << firstWord.first << ": " << joinedTranslations(firstWord.second);

      std::vector< std::string > formatted;
      formatted.reserve(dict.size() - 1);
      std::transform(std::next(dict.begin()), dict.end(), std::back_inserter(formatted), WordLineFormatter());
      std::vector< char > sink(formatted.size(), 0);
      std::transform(formatted.begin(), formatted.end(), sink.begin(), PrefixAndAppendToOstream(out));
    }

    out << ">";
  }

  void saveDict(std::istream &in, std::ostream &out, const dictionaries &dicts)
  {
    std::string dictName, filename;
    if (!read2(in, dictName, filename))
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
    if (fileExists(filename))
    {
      out << "<FILE ALREADY EXISTS>";
      return;
    }
    std::ofstream file(filename.c_str());
    if (!file)
    {
      out << "<INVALID COMMAND>";
      return;
    }

    const dictionary &d = dIt->second;

    std::vector< std::string > lines;
    lines.reserve(d.size());
    std::transform(d.begin(), d.end(), std::back_inserter(lines), PrintLine(file));

    std::vector< char > sink(lines.size(), 0);
    std::transform(lines.begin(), lines.end(), sink.begin(), WriteLine(file));

    out << "<DICT SUCCESSFULLY SAVED>";
  }

  void mergeDicts(std::istream &in, std::ostream &out, dictionaries &dicts)
  {
    std::string a, b, dest;
    if (!read3(in, a, b, dest))
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
    if (ia->second.empty() || ib->second.empty())
    {
      out << "<EMPTY DICTIONARY>";
      return;
    }
    dictionary result = ia->second;
    std::copy_if(ib->second.begin(), ib->second.end(), std::inserter(result, result.end()), MissingIn(result));

    dicts[dest] = result;
    out << "<MERGE SUCCESSFUL>";
  }

  void joinDicts(std::istream &in, std::ostream &out, dictionaries &dicts)
  {
    std::string a, b;
    if (!read2(in, a, b))
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
    dictionary &dst = ia->second;
    const dictionary &src = ib->second;
    std::copy_if(src.begin(), src.end(), std::inserter(dst, dst.end()), MissingIn(dst));
    std::vector< int > dummy(src.size(), 0);
    std::transform(src.begin(), src.end(), dummy.begin(), ApplyMergeTranslations(dst));

    out << "<JOIN SUCCESSFUL>";
  }

  void innerMerge(std::istream &in, std::ostream &out, dictionaries &dicts)
  {
    std::string a, b, dest;
    if (!read3(in, a, b, dest))
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
    const dictionary &da = ia->second;
    const dictionary &db = ib->second;

    dictionary unique;
    std::copy_if(da.begin(), da.end(), std::inserter(unique, unique.end()), MissingIn(db));
    std::copy_if(db.begin(), db.end(), std::inserter(unique, unique.end()), MissingIn(da));

    dicts[dest] = unique;
    out << "<INNER MERGE SUCCESSFUL>";
  }

  void mergeTranslations(std::istream &in, std::ostream &out, dictionaries &dicts)
  {
    std::string a, b;
    if (!read2(in, a, b))
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
    dictionary &dst = ia->second;
    const dictionary &src = ib->second;

    std::vector< int > dummy(src.size(), 0);
    std::transform(src.begin(), src.end(), dummy.begin(), ApplyMergeTranslations(dst));

    out << "<TRANSLATIONS MERGED>";
  }
}
