#include <fstream>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <functional>
#include <stdexcept>
#include <vector>
#include <map>
#include <set>
#include <numeric>

#include "TextProcessor.hpp"
#include "Utility.hpp"
#include "ValidationUtils.hpp"
#include "ImportDictParsers.hpp"

namespace crossref
{
  namespace
  {
    struct PrintComma
    {
      std::ostream &os;
      bool first;
      PrintComma(std::ostream &o):
        os(o),
        first(true)
      {}
      void operator()(int value)
      {
        if (!first)
        {
          os << ", ";
        }
        else
        {
          first = false;
        }
        os << value;
      }
    };

    struct DictExporter
    {
      std::ofstream &out;
      DictExporter(std::ofstream &o):
        out(o)
      {}

      void operator()(const std::pair< std::string, std::vector< int > > &entry) const
      {
        out << entry.first << ": ";
        if (!entry.second.empty())
        {
          PrintComma printer(out);
          std::for_each(entry.second.begin(), entry.second.end(), printer);
        }
        out << '\n';
      }
    };

    struct DictNamePrinter
    {
      bool first;
      DictNamePrinter():
        first(true)
      {}
      void operator()(const std::pair< std::string, HashTable > &item)
      {
        if (!first)
        {
          std::cout << ", ";
        }
        std::cout << item.first;
        first = false;
      }
    };

    struct LineProcessor
    {
      HashTable &table;
      int &lineNumber;
      const TextProcessor &processor;

      void operator()(const std::string &line) const
      {
        processor.processLine(table, line, lineNumber);
        lineNumber++;
      }
    };

    struct XrefFormatter
    {
      struct RecursiveAppender
      {
        std::string &line;
        std::vector< int >::const_iterator current;
        std::vector< int >::const_iterator end;

        RecursiveAppender(std::string &l, std::vector< int >::const_iterator c, std::vector< int >::const_iterator e):
          line(l),
          current(c),
          end(e)
        {}

        void operator()()
        {
          if (current == end)
          {
            return;
          }
          line += ", " + std::to_string(*current);
          ++current;
          (*this)();
        }
      };

      using entryFunctorType = std::pair< std::string, std::vector< int > >;
      void operator()(const entryFunctorType &entry, std::vector< std::string > &output) const
      {
        std::string line = entry.first + ": ";
        if (!entry.second.empty())
        {
          line += std::to_string(entry.second[0]);
          if (entry.second.size() > 1)
          {
            RecursiveAppender appender(line, std::next(entry.second.begin()), entry.second.end());
            appender();
          }
        }
        output.push_back(line);
      }
    };

    struct XrefProcessor
    {
      std::vector< std::string > &output;
      explicit XrefProcessor(std::vector< std::string > &out):
        output(out)
      {}

      void operator()(const std::pair< std::string, std::vector< int > > &entry) const
      {
        XrefFormatter formatter;
        formatter(entry, output);
      }
    };

    struct FileLineLoader
    {
      std::vector< std::string > &lines;
      FileLineLoader(std::vector< std::string > &l):
        lines(l)
      {}

      void operator()(const Line &line) const
      {
        lines.push_back(line.data);
      }
    };

    struct DictLineHandler
    {
      HashTable &table;
      DictLineHandler(HashTable &t):
        table(t)
      {}

      void operator()(const std::string &line) const
      {
        processDictLine(table, line);
      }
    };
  }

  void TextProcessor::buildDict(const std::string &dict_id, const std::string &text_id)
  {
    validation::checkIdExists(dicts, dict_id, "<DICT ID EXISTS>");
    validation::checkIdNotFound(texts, text_id, "<TEXT NOT FOUND>");

    HashTable table;
    dictWordOrder.erase(dict_id);
    auto &wordOrder = dictWordOrder[dict_id];

    int line_num = 1;
    const auto &text_lines = texts.find(text_id)->second;

    struct LineProcessor
    {
      HashTable &table;
      std::map< int, std::vector< std::string > > &wordOrder;
      int &line_num;
      const TextProcessor &processor;

      void operator()(const std::string &line) const
      {
        std::vector< std::string > words;
        crossref::splitRecursive(line, 0, words);

        std::vector< std::string > cleanWords;
        struct WordCleaner
        {
          const TextProcessor &processor;
          std::vector< std::string > &cleanWords;
          void operator()(const std::string &word) const
          {
            std::string clean = processor.sanitizeWord(word);
            if (!clean.empty())
            {
              cleanWords.push_back(clean);
            }
          }
        };
        std::for_each(words.begin(), words.end(), WordCleaner{processor, cleanWords});

        struct WordInserter
        {
          HashTable &table;
          int line_num;
          void operator()(const std::string &word) const
          {
            table.insert(word, line_num);
          }
        };
        std::for_each(cleanWords.begin(), cleanWords.end(), WordInserter{table, line_num});

        if (!cleanWords.empty())
        {
          wordOrder[line_num] = cleanWords;
        }
        line_num++;
      }
    };

    LineProcessor processor{table, wordOrder, line_num, *this};
    std::for_each(text_lines.begin(), text_lines.end(), processor);

    dicts[dict_id] = table;
  }

  void TextProcessor::showDict(const std::string &dict_id) const
  {
    auto it = dicts.find(dict_id);
    validation::checkIdNotFound(dicts, dict_id, "<DICT NOT FOUND>");

    if (it->second.isEmpty())
    {
      throw std::runtime_error("<EMPTY DICT>");
    }

    auto entries = it->second.getSortedEntries();
    struct EntryPrinter
    {
      void operator()(const std::pair< std::string, std::vector< int > > &entry) const
      {
        std::cout << entry.first << ": ";
        if (!entry.second.empty())
        {
          PrintComma printer(std::cout);
          std::for_each(entry.second.begin(), entry.second.end(), printer);
        }
        std::cout << '\n';
      }
    };

    std::for_each(entries.begin(), entries.end(), EntryPrinter());
  }

  void TextProcessor::importDict(const std::string &dict_id, const std::string &filename)
  {
    validation::checkIdExists(dicts, dict_id, "<DICT ID EXISTS>");

    std::ifstream file(filename);
    if (!file)
    {
      throw std::runtime_error("<FILE NOT FOUND>");
    }

    std::vector< Line > raw_lines;
    std::copy(std::istream_iterator< Line >(file), std::istream_iterator< Line >(), std::back_inserter(raw_lines));

    if (raw_lines.empty())
    {
      throw std::runtime_error("<IMPORT ERROR>");
    }

    std::vector< std::string > file_lines;
    FileLineLoader loader(file_lines);
    std::for_each(raw_lines.begin(), raw_lines.end(), loader);

    HashTable table;
    DictLineHandler handler(table);
    std::for_each(file_lines.begin(), file_lines.end(), handler);

    dicts[dict_id] = table;
  }

  void TextProcessor::exportDict(const std::string &dict_id, const std::string &filename) const
  {
    auto it = dicts.find(dict_id);
    validation::checkIdNotFound(dicts, dict_id, "<DICT NOT FOUND>");

    std::ofstream out(filename);
    if (!out)
    {
      throw std::runtime_error("<WRITE ERROR>");
    }

    auto entries = it->second.getSortedEntries();
    std::for_each(entries.begin(), entries.end(), DictExporter(out));
  }

  void TextProcessor::xrefToText(const std::string &new_text_id, const std::string &dict_id)
  {
    validation::checkIdExists(texts, new_text_id, "<TEXT ID EXISTS>");
    validation::checkIdNotFound(dicts, dict_id, "<DICT NOT FOUND>");

    auto orderIt = dictWordOrder.find(dict_id);
    if (orderIt == dictWordOrder.end())
    {
      throw std::runtime_error("<ORDER INFO NOT FOUND>");
    }
    const auto &wordOrder = orderIt->second;

    std::vector< std::string > xref_lines;

    struct LineProcessor
    {
      std::vector< std::string > &output;

      void operator()(const std::pair< const int, std::vector< std::string > > &line) const
      {
        std::vector< std::string > unique_words;
        struct UniqueInserter
        {
          std::vector< std::string > &unique_words;
          void operator()(const std::string &word) const
          {
            if (std::find(unique_words.begin(), unique_words.end(), word) == unique_words.end())
            {
              unique_words.push_back(word);
            }
          }
        };
        std::for_each(line.second.begin(), line.second.end(), UniqueInserter{unique_words});

        struct Concatenator
        {
          std::string operator()(const std::string &a, const std::string &b) const
          {
            return a + " " + b;
          }
        };

        if (!unique_words.empty())
        {
          std::string line_text =
              std::accumulate(std::next(unique_words.begin()), unique_words.end(), unique_words[0], Concatenator{});
          output.push_back(line_text);
        }
      }
    };

    LineProcessor processor{xref_lines};
    std::for_each(wordOrder.begin(), wordOrder.end(), processor);

    texts[new_text_id] = xref_lines;
  }

  void TextProcessor::deleteDict(const std::string &dict_id)
  {
    if (dicts.erase(dict_id) == 0)
    {
      throw std::runtime_error("<DICT NOT FOUND>");
    }
  }

  void TextProcessor::listDicts() const
  {
    std::cout << "Dicts: ";
    if (dicts.empty())
    {
      std::cout << "none\n";
      return;
    }

    DictNamePrinter printer;
    std::for_each(dicts.begin(), dicts.end(), printer);
    std::cout << '\n';
  }

}
