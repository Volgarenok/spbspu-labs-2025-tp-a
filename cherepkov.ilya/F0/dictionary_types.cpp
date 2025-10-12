#include "dictionary_types.hpp"
#include <algorithm>
#include <iterator>
#include <vector>
#include <cctype>

namespace cherepkov
{
  bool isLetter(char c)
  {
    return std::isalpha(static_cast< unsigned char >(c));
  }

  char toLower(char c)
  {
    return static_cast< char >(std::tolower(static_cast< unsigned char >(c)));
  }

  bool isPunct(char c)
  {
    return std::ispunct(static_cast< unsigned char >(c));
  }

  std::istream &operator>>(std::istream &in, Word &w)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }

    std::string temp;
    in >> temp;
    if (std::none_of(temp.begin(), temp.end(), isLetter))
    {
      in.setstate(std::ios::failbit);
    }
    else
    {
      const char *specialChars = "'\"()";
      size_t start = temp.find_first_not_of(specialChars);
      if (start == std::string::npos) start = 0;
      temp = temp.substr(start);

      size_t pos = temp.find_first_of(specialChars);
      if (pos != std::string::npos)
      {
        temp = temp.substr(0, pos);
      }

      while (!temp.empty() && isPunct(static_cast<char>(temp.back())))
      {
        temp.pop_back();
      }

      std::transform(temp.begin(), temp.end(), temp.begin(), toLower);

      w.text = std::move(temp);
    }
    return in;
  }

  std::ostream &operator<<(std::ostream &out, const Word &w)
  {
    return out << w.text;
  }

  bool operator<(const Word &lhs, const Word &rhs)
  {
    return lhs.text < rhs.text;
  }

  bool IsValidNameChar::operator()(char ch) const
  {
    return std::isalnum(static_cast< unsigned char >(ch)) || ch == '_' || ch == '-';
  }

  bool isValidName(const std::string &name)
  {
    return !name.empty() && std::all_of(name.begin(), name.end(), IsValidNameChar());
  }

  std::string printPair(const std::pair< std::string, size_t > &p)
  {
    return p.first + ": " + std::to_string(p.second);
  }

  MergeDictEntry::MergeDictEntry(Dictionary &d): destination(d) {}

  std::pair< const std::string, size_t > MergeDictEntry::operator()(const std::pair< const std::string, size_t > &pair) const
  {
    destination[pair.first] += pair.second;
    return std::make_pair(pair.first, destination[pair.first]);
  }

  CompareByFrequency::CompareByFrequency(bool desc) : descending(desc) {}

  bool CompareByFrequency::operator()(const std::pair< std::string, size_t > &a, const std::pair< std::string, size_t > &b) const
  {
    if (a.second == b.second)
    {
      return a.first < b.first;
    }
    return descending ? a.second > b.second : a.second < b.second;
  }

  WordIntersectFilter::WordIntersectFilter(const Dictionary &d) : dict2(d) {}

  bool WordIntersectFilter::operator()(const std::pair< const std::string, size_t > &entry) const
  {
    return dict2.count(entry.first) > 0;
  }

  WordExcludeFilter::WordExcludeFilter(const Dictionary &d) : dict2(d) {}

  bool WordExcludeFilter::operator()(const std::pair< const std::string, size_t > &entry) const
  {
    return dict2.count(entry.first) == 0;
  }

  void printWords(const DictionarySet &dicts, const std::string &name, std::ostream &out, size_t limit, bool descending)
  {
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
    std::sort(words.begin(), words.end(), CompareByFrequency(descending));

    if (limit == 0 || limit > words.size())
    {
      limit = words.size();
    }

    for (size_t i = 0; i < limit; ++i)
    {
      out << printPair(words[i]) << "\n";
    }
  }
}
