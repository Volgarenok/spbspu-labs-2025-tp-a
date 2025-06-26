#ifndef SUPPORT_HPP
#define SUPPORT_HPP

namespace filonova
{
  using Dictionary = std::unordered_map< std::string, size_t >;
  using DictionarySet = std::unordered_map< std::string, Dictionary >;

  struct IsValidNameChar
  {
    bool operator()(char ch) const
    {
      return std::isalnum(static_cast< unsigned char >(ch)) || ch == '_' || ch == '-';
    }
  };

  bool isValidName(const std::string &name)
  {
    if (name.empty())
    {
      return false;
    }
    return std::all_of(name.begin(), name.end(), IsValidNameChar());
  }

  struct IsValidWordChar
  {
    bool operator()(char ch) const
    {
      return std::isalpha(static_cast< unsigned char >(ch)) || ch == '-' || ch == '\'';
    }
  };

  bool isValidWord(const std::string &word)
  {
    return !word.empty() && std::all_of(word.begin(), word.end(), IsValidWordChar());
  }

  struct ToLowerChar
  {
    char operator()(unsigned char ch) const
    {
      return std::tolower(ch);
    }
  };

  std::string toLower(const std::string &str)
  {
    std::string result(str);
    std::transform(result.begin(), result.end(), result.begin(), ToLowerChar());
    return result;
  }

  struct MergeDictEntry
  {
    Dictionary &destination;
    explicit MergeDictEntry(Dictionary &d): destination(d) {}

    void operator()(const std::pair< const std::string, size_t > &pair) const
    {
      destination[pair.first] += pair.second;
    }
  };

  struct CompareByFrequencyDescending
  {
    bool operator()(const std::pair< std::string, size_t > &a, const std::pair< std::string, size_t > &b) const
    {
      return a.second > b.second || (a.second == b.second && a.first < b.first);
    }
  };

  struct CompareByFrequencyAscending
  {
    bool operator()(const std::pair< std::string, size_t > &a, const std::pair< std::string, size_t > &b) const
    {
      return a.second < b.second || (a.second == b.second && a.first < b.first);
    }
  };

  struct FilterByPresenceInDict
  {
    const Dictionary &dict2;
    bool presenceState;

    FilterByPresenceInDict(const Dictionary &d, bool state): dict2(d), presenceState(state) {}

    bool operator()(const std::pair< const std::string, size_t > &pair) const
    {
      return presenceState == (dict2.find(pair.first) != dict2.end());
    }
  };
}

#endif
