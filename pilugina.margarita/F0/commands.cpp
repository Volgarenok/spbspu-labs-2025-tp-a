#include "commands.hpp"
#include <fstream>
#include <functional>
#include <iterator>
#include <string>

namespace pilugina
{
  struct Prefixer
  {
    explicit Prefixer(const std::string &prefix):
      prefix_(prefix)
    {}

    std::string operator()(const std::string &s) const
    {
      return prefix_ + s;
    }

  private:
    std::string prefix_;
  };

  struct MissingIn
  {
    explicit MissingIn(const dictionary &d):
      d_(&d)
    {}

    bool operator()(const std::pair< const std::string, std::vector< std::string > > &p) const
    {
      return d_->find(p.first) == d_->end();
    }

  private:
    const dictionary *d_;
  };

  static std::string joinedTranslations(const std::vector< std::string > &ts)
  {
    if (ts.empty())
    {
      return std::string();
    }

    std::string result = ts.front();

    if (ts.size() > 1)
    {
      std::vector< std::string > restPrefixed;
      restPrefixed.reserve(ts.size() - 1);
      std::transform(ts.begin() + 1, ts.end(), std::back_inserter(restPrefixed), Prefixer(", "));

      struct AppendTo
      {
        explicit AppendTo(std::string &tgt):
          target_(&tgt)
        {}
        char operator()(const std::string &s) const
        {
          (*target_) += s;
          return 0;
        }

      private:
        std::string *target_;
      };

      std::vector< char > sink(restPrefixed.size(), 0);
      std::transform(restPrefixed.begin(), restPrefixed.end(), sink.begin(), AppendTo(result));
    }

    return result;
  }

  struct WordLineFormatter
  {
    std::string operator()(const std::pair< const std::string, std::vector< std::string > > &p) const
    {
      const std::string joined = joinedTranslations(p.second);
      return p.first + ": " + joined;
    }
  };

  struct ApplyMergeTranslations
  {
    explicit ApplyMergeTranslations(dictionary &dst):
      dst_(&dst)
    {}

    int operator()(const std::pair< const std::string, std::vector< std::string > > &p) const
    {
      auto it = dst_->find(p.first);
      if (it == dst_->end())
      {
        return 0;
      }
      const std::vector< std::string > &from = p.second;
      std::vector< std::string > &to = it->second;

      struct AppendIfMissing
      {
        explicit AppendIfMissing(std::vector< std::string > &v):
          to_(&v)
        {}
        int operator()(const std::string &s) const
        {
          if (std::find(to_->begin(), to_->end(), s) == to_->end())
          {
            to_->push_back(s);
          }
          return 0;
        }

      private:
        std::vector< std::string > *to_;
      };

      std::vector< int > dummy(from.size(), 0);
      std::transform(from.begin(), from.end(), dummy.begin(), AppendIfMissing(to));
      return 0;
    }

  private:
    dictionary *dst_;
  };

  static bool read3(std::istream &in, std::string &a, std::string &b, std::string &c)
  {
    return static_cast< bool >(in >> a >> b >> c);
  }
  static bool read2(std::istream &in, std::string &a, std::string &b)
  {
    return static_cast< bool >(in >> a >> b);
  }
  static bool read1(std::istream &in, std::string &a)
  {
    return static_cast< bool >(in >> a);
  }

  static bool fileExists(const std::string &name)
  {
    std::ifstream f(name.c_str(), std::ios::in | std::ios::binary);
    return static_cast< bool >(f);
  }

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
    const auto dIt = dicts.find(dictName);
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
    auto equals = std::bind(std::equal_to<std::string>{}, translation, std::placeholders::_1);
    const bool has = std::find_if(ts.begin(), ts.end(), equals) != ts.end();
    if (has)
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
    auto equals = std::bind(std::equal_to<std::string>{}, translation, std::placeholders::_1);
    auto it = std::find_if(ts.begin(), ts.end(), equals);
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

    const dictionary &d = dIt->second;
    out << "<DICT CONTENT: ";

    if (!d.empty())
    {
      const auto &firstDict = *d.begin();
      out << firstDict.first << ": " << joinedTranslations(firstDict.second);

      std::vector< std::string > formatted;
      formatted.reserve(d.size() - 1);
      std::transform(std::next(d.begin()), d.end(), std::back_inserter(formatted), WordLineFormatter());

      struct PrefixAndAppendToOstream
      {
        explicit PrefixAndAppendToOstream(std::ostream &o):
          out_(&o)
        {}
        char operator()(const std::string &s) const
        {
          (*out_) << "; " << s;
          return 0;
        }

      private:
        std::ostream *out_;
      };

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
    struct PrintLine
    {
      explicit PrintLine(std::ostream &o):
        out_(&o)
      {}
      std::string operator()(const std::pair< const std::string, std::vector< std::string > > &p) const
      {
        return p.first + ": " + joinedTranslations(p.second);
      }
      void flush(const std::string &s) const
      {
        (*out_) << s << '\n';
      }

    private:
      std::ostream *out_;
    };

    std::vector< std::string > lines;
    lines.reserve(d.size());
    std::transform(d.begin(), d.end(), std::back_inserter(lines), PrintLine(file));

    struct WriteLine
    {
      explicit WriteLine(std::ostream &o):
        out_(&o)
      {}
      char operator()(const std::string &s) const
      {
        (*out_) << s << '\n';
        return 0;
      }

    private:
      std::ostream *out_;
    };

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
