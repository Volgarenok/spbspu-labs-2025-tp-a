#include "commands.hpp"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iterator>
#include <limits>
#include <numeric>

namespace smirnov
{
  struct InsertWordNTimes
  {
    Dictionary& dict;
    InsertWordNTimes(Dictionary& d):
      dict(d)
    {}
    Dictionary operator()(Dictionary acc, const std::pair< const std::string, size_t >& p) const
    {
      size_t count = p.second;
      while (count--)
      {
        acc.insertWord(p.first);
      }
      return acc;
    }
  };

  struct IntersectAccumulate
  {
    const Dictionary& other;
    IntersectAccumulate(const Dictionary& o):
      other(o)
    {}
    Dictionary operator()(Dictionary acc, const std::pair< const std::string, size_t >& p) const
    {
      if (other.contains(p.first))
      {
        int freq = p.second + other.getFrequency(p.first);
        while (freq--)
        {
          acc.insertWord(p.first);
        }
      }
      return acc;
    }
  };

  struct FormatSizeTPair
  {
    std::string operator()(const std::pair< std::string, size_t >& p) const
    {
      return p.first + ": " + std::to_string(p.second);
    }
  };

  struct FormatDoublePair
  {
    std::string operator()(const std::pair< std::string, double >& p) const
    {
      std::stringstream ss;
      ss.setf(std::ios::fixed);
      ss.precision(6);
      ss << p.first << ": " << p.second;
      return ss.str();
    }
  };

  void printHelp(std::ostream& out)
  {
    out << "Help for commands:\n";
    out << "1. createemptydict <dictname>\n";
    out << "2. insertword <dictname> <word>\n";
    out << "3. insertfile <dictname> <filename>\n";
    out << "4. getfrequency <dictname> <word>\n";
    out << "5. mostfrequent <dictname>\n";
    out << "6. deleteword <dictname> <word>\n";
    out << "7. clear <dictname>\n";
    out << "8. merge <dict1> <dict2> <newdict>\n";
    out << "9. intersect <dict1> <dict2> <newdict>\n";
    out << "10. printsorted <dictname>\n";
    out << "11. printbyfrequency <dictname>\n";
    out << "12. size <dictname>\n";
    out << "13. topprelativefrequent <dictname> <N>\n";
    out << "14. getrelativefrequency <dictname> <word>\n";
    out << "15. frequencyrangerelative <dictname> <min> <max>\n";
    out << "16. bottomprelativefrequent <dictname> <N>\n";
    out << "17. medianfrequency <dictname>\n";
    out << "18. delete <dictname>\n";
  }

  void createemptydict(dictionaries& dicts, std::istream& in)
  {
    std::string name;
    in >> name;
    if (dicts.find(name) != dicts.end())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    dicts[name] = Dictionary();
  }

  void insertword(dictionaries& dicts, std::istream& in)
  {
    std::string dictname, word;
    in >> dictname >> word;
    if (dicts.find(dictname) == dicts.end())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    dicts[dictname].insertWord(word);
  }

  void insertfile(dictionaries& dicts, std::istream& in)
  {
    std::string dictname, filename;
    in >> dictname >> filename;
    if (dicts.find(dictname) == dicts.cend())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    std::ifstream file(filename);
    if (!file.is_open())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    dicts[dictname].insertFromFile(filename);
  }

  void deleteword(dictionaries& dicts, std::istream& in)
  {
    std::string dictname, word;
    in >> dictname >> word;
    if (dicts.find(dictname) == dicts.end() || !dicts[dictname].contains(word))
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    dicts[dictname].deleteWord(word);
  }

  void clear(dictionaries& dicts, std::istream& in)
  {
    std::string dictname;
    in >> dictname;
    if (dicts.find(dictname) == dicts.end())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    if (dicts[dictname].empty())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    dicts[dictname].clear();
  }

  void merge(dictionaries& dicts, std::istream& in)
  {
    std::string dict1, dict2, newdict;
    in >> dict1 >> dict2 >> newdict;
    if (!in || dicts.find(dict1) == dicts.end() || dicts.find(dict2) == dicts.end() ||
        dicts.find(newdict) != dicts.end())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    const Dictionary& d1 = dicts[dict1];
    const Dictionary& d2 = dicts[dict2];
    if (d1.empty() && d2.empty())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    Dictionary result;
    result =
      std::accumulate(d1.getSortedByWord().begin(), d1.getSortedByWord().end(), result, InsertWordNTimes(result));
    result =
      std::accumulate(d2.getSortedByWord().begin(), d2.getSortedByWord().end(), result, InsertWordNTimes(result));

    dicts[newdict] = result;
  }

  void intersect(dictionaries& dicts, std::istream& in)
  {
    std::string dict1, dict2, newdict;
    in >> dict1 >> dict2 >> newdict;
    if (!in || dicts.find(dict1) == dicts.end() || dicts.find(dict2) == dicts.end() ||
        dicts.find(newdict) != dicts.end())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    const Dictionary& d1 = dicts[dict1];
    const Dictionary& d2 = dicts[dict2];

    Dictionary result;
    result = std::accumulate(d1.getSortedByWord().begin(), d1.getSortedByWord().end(), result, IntersectAccumulate(d2));

    if (result.empty())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    dicts[newdict] = result;
  }

  void deleteDict(dictionaries& dicts, std::istream& in)
  {
    std::string dictname;
    in >> dictname;
    if (!in || dicts.find(dictname) == dicts.end())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    dicts.erase(dictname);
  }

  void getfrequency(dictionaries& dicts, std::istream& in, std::ostream& out)
  {
    std::string dictname, word;
    in >> dictname >> word;
    if (dicts.find(dictname) == dicts.end())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    int f = dicts[dictname].getFrequency(word);
    if (f < 0)
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    out << f << "\n";
  }

  void mostfrequent(dictionaries& dicts, std::istream& in, std::ostream& out)
  {
    std::string dictname;
    in >> dictname;
    if (dicts.find(dictname) == dicts.end() || dicts[dictname].empty())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    out << dicts[dictname].mostFrequent() << "\n";
  }

  void printsorted(dictionaries& dicts, std::istream& in, std::ostream& out)
  {
    std::string dictname;
    in >> dictname;
    if (!in || dicts.find(dictname) == dicts.end())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    auto vec = dicts[dictname].getSortedByWord();
    std::transform(vec.begin(), vec.end(), std::ostream_iterator< std::string >(out, "\n"), FormatSizeTPair());
  }

  void printbyfrequency(dictionaries& dicts, std::istream& in, std::ostream& out)
  {
    std::string dictname;
    in >> dictname;
    if (!in || dicts.find(dictname) == dicts.end())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    auto vec = dicts[dictname].getSortedByFrequency();
    std::transform(vec.begin(), vec.end(), std::ostream_iterator< std::string >(out, "\n"), FormatSizeTPair());
  }

  void size(dictionaries& dicts, std::istream& in, std::ostream& out)
  {
    std::string dictname;
    in >> dictname;
    if (!in || dicts.find(dictname) == dicts.end())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    out << dicts[dictname].size() << "\n";
  }

  void topprelativefrequent(dictionaries& dicts, std::istream& in, std::ostream& out)
  {
    std::string dictname;
    int N;
    in >> dictname >> N;
    if (!in || dicts.find(dictname) == dicts.end())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    auto result = dicts[dictname].getTopRelative(N);
    if (result.empty())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    std::transform(result.begin(), result.end(), std::ostream_iterator< std::string >(out, "\n"), FormatDoublePair());
  }

  void getrelativefrequency(dictionaries& dicts, std::istream& in, std::ostream& out)
  {
    std::string dictname, word;
    in >> dictname >> word;
    if (!in || dicts.find(dictname) == dicts.end())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    double freq = dicts[dictname].getRelativeFrequency(word);
    if (freq < 0)
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    char buffer[64];
    std::snprintf(buffer, sizeof(buffer), "%.6f", freq);
    out << word << ": " << buffer << "\n";
  }

  void frequencyrangerelative(dictionaries& dicts, std::istream& in, std::ostream& out)
  {
    std::string dictname;
    double min, max;
    in >> dictname >> min >> max;
    if (!in || dicts.find(dictname) == dicts.end() || min > max)
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    auto result = dicts[dictname].getRangeRelative(min, max);
    if (result.empty())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    std::transform(result.begin(), result.end(), std::ostream_iterator< std::string >(out, "\n"), FormatDoublePair());
  }

  void bottomprelativefrequent(dictionaries& dicts, std::istream& in, std::ostream& out)
  {
    std::string dictname;
    int N;
    in >> dictname >> N;
    if (!in || dicts.find(dictname) == dicts.end())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    auto result = dicts[dictname].getBottomRelative(N);
    if (result.empty())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    std::transform(result.begin(), result.end(), std::ostream_iterator< std::string >(out, "\n"), FormatDoublePair());
  }

  void medianfrequency(dictionaries& dicts, std::istream& in, std::ostream& out)
  {
    std::string dictname;
    in >> dictname;
    if (!in || dicts.find(dictname) == dicts.end())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    auto result = dicts[dictname].medianFrequency();
    if (dicts[dictname].empty())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    std::copy(result.begin(), result.end(), std::ostream_iterator< std::string >(out, "\n"));
  }
}

