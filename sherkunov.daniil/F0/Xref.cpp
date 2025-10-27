#include "Xref.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <functional>
#include <iterator>
#include <numeric>
#include <sstream>
#include <stdexcept>

namespace sherkunov
{

  namespace
  {

    struct ToLowerChar
    {
      char operator()(char c) const
      {
        return static_cast< char >(std::tolower(static_cast< unsigned char >(c)));
      }
    };

    struct IsDigitChar
    {
      bool operator()(unsigned char c) const
      {
        return std::isdigit(c) != 0;
      }
    };

    struct IsAlphaChar
    {
      bool operator()(unsigned char c) const
      {
        return std::isalpha(c) != 0;
      }
    };

    struct IsNotAlphaChar
    {
      bool operator()(unsigned char c) const
      {
        return std::isalpha(c) == 0;
      }
    };

    struct IsAlnumChar
    {
      bool operator()(unsigned char c) const
      {
        return std::isalnum(c) != 0;
      }
    };

    struct IsValidNameChar
    {
      bool operator()(unsigned char c) const
      {
        return std::isalnum(c) || c == '_' || c == '-';
      }
    };

    struct AccumMaxPos
    {
      size_t operator()(size_t acc, const std::pair< const std::string, std::vector< size_t > >& kv) const
      {
        if (kv.second.empty())
        {
          return acc;
        }
        size_t mx = *std::max_element(kv.second.begin(), kv.second.end());
        return std::max(acc, mx);
      }
    };

    struct JoinWithSpace
    {
      std::string operator()(const std::string& acc, const std::string& s) const
      {
        return acc + " " + s;
      }
    };

    struct PushToRefs
    {
      std::map< std::string, std::vector< size_t > >* refs;
      size_t* idx;

      void operator()(std::string word) const
      {
        word.erase(std::remove_if(word.begin(), word.end(), IsNotAlphaChar{}), word.end());
        std::transform(word.begin(), word.end(), word.begin(), ToLowerChar{});
        if (!word.empty())
        {
          (*refs)[word].push_back(*idx);
        }
        ++(*idx);
      }
    };

    struct PrintWithComma
    {
      std::ostream* out;
      bool* first;
      void operator()(size_t v) const
      {
        if (!*first)
        {
          (*out) << ",";
        }
        (*out) << v;
        *first = false;
      }
    };

    struct PrintRefLine
    {
      std::ostream* out;
      void operator()(const std::pair< const std::string, std::vector< size_t > >& kv) const
      {
        (*out) << kv.first << ":";
        bool first = true;
        std::for_each(kv.second.begin(), kv.second.end(), PrintWithComma{ out, &first });
        (*out) << "\n";
      }
    };

    struct ToCountPair
    {
      std::pair< std::string, size_t > operator()(
        const std::pair< const std::string, std::vector< size_t > >& kv) const
      {
        return { kv.first, kv.second.size() };
      }
    };

    struct ByCountDesc
    {
      bool operator()(const std::pair< std::string, size_t >& a, const std::pair< std::string, size_t >& b) const
      {
        if (a.second != b.second)
        {
          return a.second > b.second;
        }
        return a.first < b.first;
      }
    };

    struct PrintCountLine
    {
      std::ostream* out;
      void operator()(const std::pair< std::string, size_t >& kv) const
      {
        (*out) << "  " << kv.first << ": " << kv.second << "\n";
      }
    };

    struct PrintPosSpace
    {
      std::ostream* out;
      bool* first;
      void operator()(size_t v) const
      {
        if (!*first)
        {
          (*out) << " ";
        }
        (*out) << v;
        *first = false;
      }
    };

    struct MergeAccum
    {
      std::map< std::string, std::vector< size_t > >* dst;
      size_t offset;
      void operator()(const std::pair< const std::string, std::vector< size_t > >& kv) const
      {
        auto& vec = (*dst)[kv.first];
        vec.reserve(vec.size() + kv.second.size());
        for (size_t pos : kv.second)
        {
          vec.push_back(pos + offset);
        }
      }
    };

  }

  bool CrossReferenceSystem::isValidName(const std::string& name)
  {
    if (name.empty())
    {
      return false;
    }
    return std::all_of(name.begin(), name.end(), IsValidNameChar{});
  }

  bool CrossReferenceSystem::isValidWord(const std::string& word)
  {
    if (word.empty())
    {
      return false;
    }
    return std::all_of(word.begin(), word.end(), IsAlphaChar{});
  }

  std::vector< std::string > CrossReferenceSystem::splitIntoWords(const std::string& content)
  {
    std::istringstream iss(content);
    return std::vector< std::string >(
      std::istream_iterator< std::string >(iss), std::istream_iterator< std::string >());
  }

  std::string CrossReferenceSystem::joinWords(const std::vector< std::string >& words)
  {
    if (words.empty())
    {
      return std::string();
    }
    return std::accumulate(std::next(words.begin()), words.end(), words.front(), JoinWithSpace{});
  }

  void CrossReferenceSystem::buildReferences(const std::string& text_name, const std::string& content)
  {
    TextData& data = texts[text_name];
    data.content = content;
    data.references.clear();
    auto words = splitIntoWords(content);
    size_t idx = 0;
    std::for_each(words.begin(), words.end(), PushToRefs{ &data.references, &idx });
  }

  void CrossReferenceSystem::build(const std::string& text_name, const std::string& text_content)
  {
    if (!isValidName(text_name))
    {
      throw std::runtime_error("<INVALID NAME>");
    }
    if (text_content.empty())
    {
      throw std::runtime_error("<EMPTY TEXT>");
    }
    if (texts.find(text_name) != texts.end())
    {
      throw std::runtime_error("<ALREADY EXISTS>");
    }
    buildReferences(text_name, text_content);
  }

  void CrossReferenceSystem::reconstruct(const std::string& text_name, const std::string& output_file)
  {
    auto it = texts.find(text_name);
    if (it == texts.end())
    {
      throw std::runtime_error("<NOT FOUND>");
    }
    const TextData& data = it->second;
    auto words = splitIntoWords(data.content);

    size_t maxPos = std::accumulate(
      data.references.begin(), data.references.end(), size_t{ 0 }, AccumMaxPos{});
    if (!words.empty() && maxPos >= words.size())
    {
      throw std::runtime_error("<CORRUPTED INDEX>");
    }

    if (output_file.empty())
    {
      std::cout << data.content << std::endl;
      return;
    }

    std::ifstream probe(output_file);
    if (probe.good())
    {
      throw std::runtime_error("<IO EXISTS>");
    }

    std::ofstream out(output_file);
    if (!out)
    {
      throw std::runtime_error("<IO ERROR>");
    }
    std::for_each(data.references.begin(), data.references.end(), PrintRefLine{ &out });
  }

  void CrossReferenceSystem::concat(const std::string& new_name, const std::string& name1, const std::string& name2)
  {
    if (!isValidName(new_name))
    {
      throw std::runtime_error("<INVALID NAME>");
    }
    if (texts.find(new_name) != texts.end())
    {
      throw std::runtime_error("<ALREADY EXISTS>");
    }
    auto it1 = texts.find(name1);
    auto it2 = texts.find(name2);
    if (it1 == texts.end() || it2 == texts.end())
    {
      throw std::runtime_error("<NOT FOUND>");
    }
    std::string joined = it1->second.content + " " + it2->second.content;
    buildReferences(new_name, joined);
  }

  std::vector< size_t > CrossReferenceSystem::search(const std::string& text_name, const std::string& word)
  {
    auto it = texts.find(text_name);
    if (it == texts.end())
    {
      throw std::runtime_error("<NOT FOUND>");
    }
    if (word.empty())
    {
      throw std::runtime_error("<EMPTY WORD>");
    }
    std::string key = word;
    std::transform(key.begin(), key.end(), key.begin(), ToLowerChar{});
    auto itw = it->second.references.find(key);
    if (itw == it->second.references.end())
    {
      return {};
    }
    return itw->second;
  }

  void CrossReferenceSystem::replace(
    const std::string& text_name, const std::string& old_word, const std::string& new_word)
  {
    auto it = texts.find(text_name);
    if (it == texts.end())
    {
      throw std::runtime_error("<NOT FOUND>");
    }
    if (old_word.empty())
    {
      throw std::runtime_error("<EMPTY WORD>");
    }
    const auto& refs = it->second.references;
    std::string key = old_word;
    std::transform(key.begin(), key.end(), key.begin(), ToLowerChar{});
    auto itw = refs.find(key);
    if (itw == refs.end())
    {
      throw std::runtime_error("<WORD NOT FOUND>");
    }

    auto words = splitIntoWords(it->second.content);
    for (size_t pos : itw->second)
    {
      if (pos < words.size())
      {
        words[pos] = new_word;
      }
    }
    std::string new_content = joinWords(words);
    buildReferences(text_name, new_content);
  }

  void CrossReferenceSystem::insert(const std::string& text_name, size_t position, const std::string& word)
  {
    auto it = texts.find(text_name);
    if (it == texts.end())
    {
      throw std::runtime_error("<NOT FOUND>");
    }
    if (!isValidWord(word))
    {
      throw std::runtime_error("<INVALID WORD>");
    }

    auto words = splitIntoWords(it->second.content);
    if (position > words.size())
    {
      throw std::runtime_error("<INVALID POSITION>");
    }

    words.insert(words.begin() + position, word);
    buildReferences(text_name, joinWords(words));
  }


  void CrossReferenceSystem::remove(const std::string& text_name, size_t start, size_t end)
  {
    auto it = texts.find(text_name);
    if (it == texts.end())
    {
      throw std::runtime_error("<NOT FOUND>");
    }
    auto words = splitIntoWords(it->second.content);
    if (start > end || end >= words.size())
    {
      throw std::runtime_error("<INVALID RANGE>");
    }
    words.erase(words.begin() + start, words.begin() + end + 1);
    buildReferences(text_name, joinWords(words));
  }

  void CrossReferenceSystem::import_text(const std::string& filename)
  {
    std::ifstream in(filename);
    if (!in)
    {
      throw std::runtime_error("<FILE NOT FOUND>");
    }
    std::ostringstream ss;
    ss << in.rdbuf();
    std::string content = ss.str();
    if (content.empty())
    {
      throw std::runtime_error("<INVALID FORMAT>");
    }
    std::string name = filename;
    auto slash = name.find_last_of("/\\");
    if (slash != std::string::npos)
    {
      name = name.substr(slash + 1);
    }
    auto dot = name.find_last_of('.');
    if (dot != std::string::npos)
    {
      name = name.substr(0, dot);
    }
    build(name, content);
  }

  void CrossReferenceSystem::export_text(const std::string& text_name, const std::string& filename)
  {
    auto it = texts.find(text_name);
    if (it == texts.end())
    {
      throw std::runtime_error("<NOT FOUND>");
    }
    std::ofstream out(filename);
    if (!out)
    {
      throw std::runtime_error("<IO ERROR>");
    }
    std::for_each(it->second.references.begin(), it->second.references.end(), PrintRefLine{ &out });
  }

  void CrossReferenceSystem::stats(const std::string& text_name)
  {
    auto it = texts.find(text_name);
    if (it == texts.end())
    {
      throw std::runtime_error("<NOT FOUND>");
    }
    const auto& data = it->second;
    auto words = splitIntoWords(data.content);

    std::cout << "Unique words: " << data.references.size() << std::endl;
    std::cout << "Total words: " << words.size() << std::endl;

    std::vector< std::pair< std::string, size_t > > counts;
    counts.reserve(data.references.size());
    std::transform(
      data.references.begin(), data.references.end(), std::back_inserter(counts), ToCountPair{});
    std::sort(counts.begin(), counts.end(), ByCountDesc{});

    size_t top = std::min< size_t >(counts.size(), 5);
    for (size_t i = 0; i < top; ++i)
    {
      PrintCountLine{ &std::cout }(counts[i]);
    }
  }

  void CrossReferenceSystem::merge(const std::string& new_name, const std::string& name1, const std::string& name2)
  {
    if (!isValidName(new_name))
    {
      throw std::runtime_error("<INVALID NAME>");
    }
    if (texts.find(new_name) != texts.end())
    {
      throw std::runtime_error("<ALREADY EXISTS>");
    }
    auto it1 = texts.find(name1);
    auto it2 = texts.find(name2);
    if (it1 == texts.end() || it2 == texts.end())
    {
      throw std::runtime_error("<NOT FOUND>");
    }

    const auto& t1 = it1->second;
    const auto& t2 = it2->second;

    std::vector< std::string > words1 = splitIntoWords(t1.content);
    size_t offset = words1.size();

    TextData merged;
    merged.content = t1.content + " " + t2.content;
    merged.references = t1.references;

    std::for_each(t2.references.begin(), t2.references.end(), MergeAccum{ &merged.references, offset });

    texts[new_name] = std::move(merged);
  }

  int processCommandLineArguments(int argc, char** argv, CrossReferenceSystem& system)
  {
    if (argc <= 1)
    {
      return -1;
    }
    std::string arg1 = argv[1];
    if (arg1 == "--help")
    {
      std::cout << "Usage:\n"
                << "  --help\n"
                << "  --check <file>\n"
                << "  --export <file>\n"
                << "  <file>\n";
      return 0;
    }
    if (arg1 == "--check")
    {
      if (argc < 3)
      {
        std::cerr << "Error: <FILE NOT FOUND>\n";
        return 1;
      }
      try
      {
        system.import_text(argv[2]);
        std::cout << "File is valid\n";
        return 0;
      }
      catch (const std::exception& e)
      {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
      }
    }
    if (arg1 == "--export")
    {
      return 0;
    }

    try
    {
      system.import_text(arg1);
      return 0;
    }
    catch (const std::exception& e)
    {
      std::cerr << "Error: " << e.what() << std::endl;
      return 1;
    }
  }

  void runInteractiveMode(CrossReferenceSystem& system)
  {
    std::string line;
    while (true)
    {
      std::cout << "> " << std::flush;
      if (!std::getline(std::cin, line))
      {
        break;
      }
      if (line.empty())
      {
        continue;
      }
      try
      {
        auto sp = line.find(' ');
        std::string cmd = (sp == std::string::npos) ? line : line.substr(0, sp);
        std::string args = (sp == std::string::npos) ? "" : line.substr(sp + 1);

        if (cmd == "build")
        {
          auto sp2 = args.find(' ');
          if (sp2 == std::string::npos)
          {
            throw std::runtime_error("<INVALID ARGUMENTS>");
          }
          std::string name = args.substr(0, sp2);
          std::string content = args.substr(sp2 + 1);
          system.build(name, content);
          std::cout << "OK" << std::endl;
        }
        else if (cmd == "reconstruct")
        {
          auto sp2 = args.find(' ');
          if (sp2 == std::string::npos)
          {
            system.reconstruct(args);
          }
          else
          {
            std::string name = args.substr(0, sp2);
            std::string file = args.substr(sp2 + 1);
            system.reconstruct(name, file);
          }
        }
        else if (cmd == "concat")
        {
          size_t p1 = args.find(' ');
          size_t p2 = args.find(' ', p1 + 1);
          if (p1 == std::string::npos || p2 == std::string::npos)
          {
            throw std::runtime_error("<INVALID ARGUMENTS>");
          }
          std::string new_name = args.substr(0, p1);
          std::string a = args.substr(p1 + 1, p2 - p1 - 1);
          std::string b = args.substr(p2 + 1);
          system.concat(new_name, a, b);
          std::cout << "OK" << std::endl;
        }
        else if (cmd == "search")
        {
          size_t p = args.find(' ');
          if (p == std::string::npos)
          {
            throw std::runtime_error("<INVALID ARGUMENTS>");
          }
          std::string name = args.substr(0, p);
          std::string w = args.substr(p + 1);
          auto res = system.search(name, w);
          bool first = true;
          std::for_each(res.begin(), res.end(), PrintPosSpace{ &std::cout, &first });
          std::cout << std::endl;
        }
        else if (cmd == "replace")
        {
          size_t p1 = args.find(' ');
          size_t p2 = args.find(' ', p1 + 1);
          if (p1 == std::string::npos || p2 == std::string::npos)
          {
            throw std::runtime_error("<INVALID ARGUMENTS>");
          }
          std::string name = args.substr(0, p1);
          std::string oldw = args.substr(p1 + 1, p2 - p1 - 1);
          std::string neww = args.substr(p2 + 1);
          system.replace(name, oldw, neww);
          std::cout << "OK" << std::endl;
        }
        else if (cmd == "insert")
        {
          size_t p1 = args.find(' ');
          size_t p2 = args.find(' ', p1 + 1);
          if (p1 == std::string::npos || p2 == std::string::npos)
          {
            throw std::runtime_error("<INVALID ARGUMENTS>");
          }
          std::string name = args.substr(0, p1);
          size_t pos = std::stoull(args.substr(p1 + 1, p2 - p1 - 1));
          std::string word = args.substr(p2 + 1);
          system.insert(name, pos, word);
          std::cout << "OK" << std::endl;
        }
        else if (cmd == "remove")
        {
          size_t p1 = args.find(' ');
          size_t p2 = args.find(' ', p1 + 1);
          if (p1 == std::string::npos || p2 == std::string::npos)
          {
            throw std::runtime_error("<INVALID ARGUMENTS>");
          }
          std::string name = args.substr(0, p1);
          size_t a = std::stoull(args.substr(p1 + 1, p2 - p1 - 1));
          size_t b = std::stoull(args.substr(p2 + 1));
          system.remove(name, a, b);
          std::cout << "OK" << std::endl;
        }
        else if (cmd == "import")
        {
          system.import_text(args);
          std::cout << "OK" << std::endl;
        }
        else if (cmd == "export")
        {
          size_t pos = args.find(' ');
          if (pos == std::string::npos)
          {
            throw std::runtime_error("<INVALID ARGUMENTS>");
          }
          std::string name = args.substr(0, pos);
          std::string filename = args.substr(pos + 1);
          system.export_text(name, filename);
          std::cout << "OK" << std::endl;
        }
        else if (cmd == "stats")
        {
          system.stats(args);
        }
        else if (cmd == "merge")
        {
          size_t pos1 = args.find(' ');
          size_t pos2 = args.find(' ', pos1 + 1);
          if (pos1 == std::string::npos || pos2 == std::string::npos)
          {
            throw std::runtime_error("<INVALID ARGUMENTS>");
          }
          std::string new_name = args.substr(0, pos1);
          std::string name1 = args.substr(pos1 + 1, pos2 - pos1 - 1);
          std::string name2 = args.substr(pos2 + 1);
          system.merge(new_name, name1, name2);
          std::cout << "OK" << std::endl;
        }
        else if (cmd == "exit" || cmd == "quit")
        {
          break;
        }
        else
        {
          std::cout << "Unknown command: " << cmd << std::endl;
        }
      }
      catch (const std::exception& e)
      {
        std::cerr << "Error: " << e.what() << std::endl;
      }
    }
  }

}
