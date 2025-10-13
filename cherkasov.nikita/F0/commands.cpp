#include "commands.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <iterator>

namespace cherkasov
{
  struct PrintKey
  {
    std::ostream& out;
    void operator()(const DictTable::value_type& kv) const
    {
      out << kv.first << '\n';
    }
  };

  struct Merger
  {
    Dict& target;
    void operator()(const Dict::value_type& kv) const
    {
      target[kv.first] += kv.second;
    }
  };

  struct Intersector
  {
    const Dict& other;
    Dict& result;
    void operator()(const Dict::value_type& kv) const
    {
      auto it = other.find(kv.first);
      if (it != other.end())
      {
        result[kv.first] = std::min(kv.second, it->second);
      }
    }
  };

  struct CmpTop
  {
    bool operator()(const WordEntry& a, const WordEntry& b) const
    {
      return (a.second == b.second) ? (a.first < b.first) : (a.second > b.second);
    }
  };

  struct CmpRare
  {
    bool operator()(const WordEntry& a, const WordEntry& b) const
    {
      return (a.second == b.second) ? (a.first < b.first) : (a.second < b.second);
    }
  };

  struct Importer
  {
    Dict& dict;
    void operator()(const std::string& word) const
    {
      ++dict[word];
    }
  };

  struct PrintWord
  {
    std::ostream& out;
    void operator()(const WordEntry& w) const
    {
      out << w.first << " " << w.second << '\n';
    }
  };

  void makeDict(std::istream & in, DictTable & dicts)
  {
    std::string name;
    in >> name;
    if (dicts.find(name) != dicts.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    dicts[name] = Dict{};
  }

  void listDicts(std::ostream & out, const DictTable & dicts)
  {
    if (dicts.empty())
    {
      out << "<EMPTY>\n";
      return;
    }
    std::for_each(dicts.begin(), dicts.end(), PrintKey{out});
  }

  void importText(std::istream & in, DictTable & dicts)
  {
    std::string name, filename;
    in >> name >> filename;
    auto it = dicts.find(name);
    if (it == dicts.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    std::ifstream file(filename);
    if (!file.is_open())
    {
      throw std::logic_error("<INVALID FILE>");
    }
    std::istream_iterator<std::string> begin(file), end;
    std::for_each(begin, end, Importer{it->second});
  }

  void mergeDicts(std::istream & in, DictTable & dicts)
  {
    std::string res, d1, d2;
    in >> res >> d1 >> d2;
    if (dicts.find(res) != dicts.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    if (dicts.find(d1) == dicts.end() || dicts.find(d2) == dicts.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    Dict merged = dicts[d1];
    std::for_each(dicts[d2].begin(), dicts[d2].end(), Merger{merged});
    dicts[res] = std::move(merged);
  }

  void intersectDicts(std::istream & in, DictTable & dicts)
  {
    std::string res, d1, d2;
    in >> res >> d1 >> d2;
    if (dicts.find(res) != dicts.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    if (dicts.find(d1) == dicts.end() || dicts.find(d2) == dicts.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    Dict result;
    std::for_each(dicts[d1].begin(), dicts[d1].end(), Intersector{dicts[d2], result});
    dicts[res] = std::move(result);
  }

  void cloneDict(std::istream & in, DictTable & dicts)
  {
    std::string res, src;
    in >> res >> src;
    if (dicts.find(res) != dicts.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    auto it = dicts.find(src);
    if (it == dicts.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    dicts[res] = it->second;
  }

  void insertWord(std::istream & in, DictTable & dicts)
  {
    std::string name, word;
    int freq;
    in >> name >> word >> freq;
    auto it = dicts.find(name);
    if (it == dicts.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    if (freq <= 0)
    {
      throw std::logic_error("<INVALID NUMBER>");
    }
    if (it->second.find(word) != it->second.end())
    {
      throw std::logic_error("<INVALID WORD>");
    }
    it->second[word] = freq;
  }

  void dictSize(std::istream & in, std::ostream & out, const DictTable & dicts)
  {
    std::string name;
    in >> name;
    auto it = dicts.find(name);
    if (it == dicts.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    out << it->second.size() << '\n';
  }

  void eraseWord(std::istream & in, DictTable & dicts)
  {
    std::string name, word;
    in >> name >> word;
    auto it = dicts.find(name);
    if (it == dicts.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    if (it->second.erase(word) == 0)
    {
      throw std::logic_error("<INVALID WORD>");
    }
  }

  void clearDict(std::istream & in, DictTable & dicts)
  {
    std::string name;
    in >> name;
    auto it = dicts.find(name);
    if (it == dicts.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    it->second.clear();
  }

  void printTop(std::istream & in, std::ostream & out, const DictTable & dicts)
  {
    std::string name;
    int count;
    in >> name >> count;
    auto it = dicts.find(name);
    if (it == dicts.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    if (count <= 0 || count > static_cast<int>(it->second.size()))
    {
      throw std::logic_error("<INVALID NUMBER>");
    }
    std::vector<WordEntry> words(it->second.begin(), it->second.end());
    std::sort(words.begin(), words.end(), CmpTop{});
    std::for_each(words.begin(), words.begin() + count, PrintWord{out});
  }

  void printRare(std::istream & in, std::ostream & out, const DictTable & dicts)
  {
    std::string name;
    int count;
    in >> name >> count;
    auto it = dicts.find(name);
    if (it == dicts.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    if (count <= 0 || count > static_cast<int>(it->second.size()))
    {
      throw std::logic_error("<INVALID NUMBER>");
    }
    std::vector<WordEntry> words(it->second.begin(), it->second.end());
    std::sort(words.begin(), words.end(), CmpRare{});
    std::for_each(words.begin(), words.begin() + count, PrintWord{out});
  }
  void printHelp(std::ostream& out)
  {
    out << "Usage: ./program [file]\n";
    out << "Commands:\n";
    out << " createdict <dict>\n";
    out << " showdicts\n";
    out << " loadtext <dict> <file>\n";
    out << " union <res> <d1> <d2>\n";
    out << " intersect <res> <d1> <d2>\n";
    out << " copy <res> <dict>\n";
    out << " addword <dict> <word> <freq>\n";
    out << " size <dict>\n";
    out << " cleanword <dict> <word>\n";
    out << " cleandict <dict>\n";
    out << " top <dict> <n>\n";
    out << " rare <dict> <n>\n";
  }
  void loadFile(const std::string& filename, DictTable& dicts)
  {
    std::ifstream file(filename);
    if (!file)
    {
      throw std::runtime_error("Failed to open file: " + filename);
    }

    auto& dict = dicts["default"];
    std::string word;
    while (file >> word)
    {
      ++dict[word];
    }
  }
}
