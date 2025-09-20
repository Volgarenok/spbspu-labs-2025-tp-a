#include "ImportDictParsers.hpp"

#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <string>
#include <vector>
#include <functional>

#include "ValidationUtils.hpp"

namespace crossref
{

  struct IsSpace
  {
    bool operator()(char c) const
    {
      return std::isspace(static_cast< unsigned char >(c));
    }
  };

  struct FindComma
  {
    bool operator()(char c) const
    {
      return c == ',';
    }
  };

  struct NumberProcessor
  {
    std::vector< int > &numbers;
    const std::string &num_str;

    NumberProcessor(std::vector< int > &nums, const std::string &ns):
      numbers(nums),
      num_str(ns)
    {}

    void operator()() const
    {
      try
      {
        numbers.push_back(std::stoi(num_str));
      }
      catch (...)
      {
        throw std::runtime_error("<IMPORT ERROR>");
      }
    }
  };

  struct NumberPartHandler
  {
    const std::string &numbers;
    std::vector< int > &line_numbers;
    size_t &start;

    NumberPartHandler(const std::string &n, std::vector< int > &ln, size_t &s):
      numbers(n),
      line_numbers(ln),
      start(s)
    {}

    void operator()() const
    {
      auto end = std::find_if(numbers.begin() + start, numbers.end(), FindComma());

      std::string num_str(numbers.begin() + start, end);

      std::string::iterator new_end = std::remove_if(num_str.begin(), num_str.end(), IsSpace());
      num_str.erase(new_end, num_str.end());

      if (!num_str.empty())
      {
        NumberProcessor processor(line_numbers, num_str);
        processor();
      }

      start = (end == numbers.end()) ? numbers.size() : std::distance(numbers.begin(), end) + 1;
    }
  };

  struct DictLineProcessor
  {
    HashTable &table;
    explicit DictLineProcessor(HashTable &t):
      table(t)
    {}

    void operator()(const std::string &line) const
    {
      size_t colon_pos = line.find(':');
      if (colon_pos == std::string::npos || colon_pos == 0)
      {
        throw std::runtime_error("<IMPORT ERROR>");
      }

      std::string word = line.substr(0, colon_pos);
      std::string numbers = line.substr(colon_pos + 1);

      std::string::iterator new_end = std::remove_if(word.begin(), word.end(), IsSpace());
      word.erase(new_end, word.end());

      if (word.empty())
      {
        throw std::runtime_error("<IMPORT ERROR>");
      }

      std::vector< int > line_numbers;
      size_t start = 0;

      struct NumberParser
      {
        const std::string &numbers;
        std::vector< int > &line_numbers;
        size_t &start;

        NumberParser(const std::string &n, std::vector< int > &ln, size_t &s):
          numbers(n),
          line_numbers(ln),
          start(s)
        {}

        void operator()() const
        {
          if (start >= numbers.size())
          {
            return;
          }

          size_t end = numbers.find(',', start);
          if (end == std::string::npos)
          {
            end = numbers.size();
          }

          std::string num_str = numbers.substr(start, end - start);
          num_str.erase(std::remove_if(num_str.begin(), num_str.end(), IsSpace()), num_str.end());

          if (!num_str.empty())
          {
            try
            {
              line_numbers.push_back(std::stoi(num_str));
            }
            catch (...)
            {
              throw std::runtime_error("<IMPORT ERROR>");
            }
          }

          start = (end == numbers.size()) ? numbers.size() : end + 1;
          (*this)();
        }
      };

      NumberParser parser(numbers, line_numbers, start);
      parser();

      struct Inserter
      {
        HashTable &table;
        const std::string &word;
        Inserter(HashTable &t, const std::string &w):
          table(t),
          word(w)
        {}
        void operator()(int num) const
        {
          table.insert(word, num);
        }
      };

      Inserter inserter(table, word);
      std::for_each(line_numbers.begin(), line_numbers.end(), inserter);
    }
  };

  void processDictLine(HashTable &table, const std::string &line)
  {
    DictLineProcessor processor(table);
    processor(line);
  }

}
