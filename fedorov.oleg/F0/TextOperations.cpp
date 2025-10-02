#include <fstream>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <functional>
#include <stdexcept>
#include <vector>
#include <map>
#include <set>

#include "TextProcessor.hpp"
#include "Utility.hpp"
#include "ValidationUtils.hpp"

namespace crossref
{
  namespace
  {
    struct LinePrinter
    {
      mutable int count;
      LinePrinter():
        count(1)
      {}
      void operator()(const std::string &line) const
      {
        std::cout << count++ << ": " << line << '\n';
      }
    };

    struct WordReplacer
    {
      std::string old_word;
      std::string new_word;

      struct Replacer
      {
        const WordReplacer &parent;
        std::string result;
        size_t pos;

        Replacer(const WordReplacer &p, const std::string &line):
          parent(p),
          result(line),
          pos(0)
        {}

        void operator()()
        {
          if (pos >= result.size())
          {
            return;
          }

          size_t found_pos = result.find(parent.old_word, pos);
          if (found_pos == std::string::npos)
          {
            return;
          }

          result.replace(found_pos, parent.old_word.length(), parent.new_word);
          pos = found_pos + parent.new_word.length();
          (*this)();
        }
      };

      std::string operator()(const std::string &line) const
      {
        if (old_word.empty())
        {
          return line;
        }

        Replacer replacer(*this, line);
        replacer();
        return replacer.result;
      }
    };

    struct HeaderChecker
    {
      std::string pattern;
      std::vector< std::string > &headers;
      mutable int current_line;

      HeaderChecker(const std::string &p, std::vector< std::string > &h, int start):
        pattern(p),
        headers(h),
        current_line(start)
      {}

      void operator()(const std::string &line) const
      {
        if (line.find(pattern) == 0)
        {
          headers.push_back(line + " (строка " + std::to_string(current_line) + ")");
        }
        current_line++;
      }
    };

    struct HeaderFormatter
    {
      std::vector< std::string > &headers;
      mutable int current_line;

      HeaderFormatter(std::vector< std::string > &h, int start):
        headers(h),
        current_line(start)
      {}

      void operator()(const std::string &line) const
      {
        headers.push_back(line + " (строка " + std::to_string(current_line) + ")");
        current_line++;
      }
    };

    struct NamePrinter
    {
      bool first;
      NamePrinter():
        first(true)
      {}
      void operator()(const std::pair< std::string, std::vector< std::string > > &item)
      {
        if (!first)
        {
          std::cout << ", ";
        }
        std::cout << item.first;
        first = false;
      }
    };
  }

  void TextProcessor::showText(const std::string &text_id) const
  {
    auto it = texts.find(text_id);
    validation::checkIdNotFound(texts, text_id, "<TEXT NOT FOUND>");

    LinePrinter printer;
    std::for_each(it->second.begin(), it->second.end(), printer);
  }

  void TextProcessor::deleteText(const std::string &text_id)
  {
    if (texts.erase(text_id) == 0)
    {
      throw std::runtime_error("<TEXT NOT FOUND>");
    }
  }

  void
  TextProcessor::concatTexts(const std::string &new_text_id, const std::string &text_id1, const std::string &text_id2)
  {
    validation::checkIdExists(texts, new_text_id, "<TEXT ID EXISTS>");
    validation::checkIdNotFound(texts, text_id1, "<TEXT NOT FOUND>");
    validation::checkIdNotFound(texts, text_id2, "<TEXT NOT FOUND>");

    std::vector< std::string > new_text;
    std::copy(texts[text_id1].begin(), texts[text_id1].end(), std::back_inserter(new_text));
    std::copy(texts[text_id2].begin(), texts[text_id2].end(), std::back_inserter(new_text));

    texts[new_text_id] = new_text;
  }

  void TextProcessor::extractLines(const std::string &new_text_id,
                                   const std::string &source_text_id,
                                   int start_line,
                                   int end_line)
  {
    validation::checkIdExists(texts, new_text_id, "<TEXT ID EXISTS>");
    auto it = texts.find(source_text_id);
    validation::checkIdNotFound(texts, source_text_id, "<TEXT NOT FOUND>");

    validation::checkLineRange(start_line, end_line, it->second.size());

    std::vector< std::string > extracted;
    std::copy(it->second.begin() + start_line - 1, it->second.begin() + end_line, std::back_inserter(extracted));

    texts[new_text_id] = extracted;
  }

  void TextProcessor::replaceWords(const std::string &text_id, const std::string &old_word, const std::string &new_word)
  {
    auto it = texts.find(text_id);
    validation::checkIdNotFound(texts, text_id, "<TEXT NOT FOUND>");

    WordReplacer replacer{old_word, new_word};
    std::vector< std::string > updated;
    std::transform(it->second.begin(), it->second.end(), std::back_inserter(updated), replacer);

    it->second = updated;
  }

  void TextProcessor::extractHeaders(const std::string &new_text_id,
                                     const std::string &source_text_id,
                                     const std::string &pattern)
  {
    auto it_src = texts.find(source_text_id);
    validation::checkIdNotFound(texts, source_text_id, "<TEXT NOT FOUND>");
    validation::checkIdExists(texts, new_text_id, "<TEXT ID EXISTS>");

    std::vector< std::string > headers;
    HeaderChecker checker(pattern, headers, 1);

    std::for_each(it_src->second.begin(), it_src->second.end(), checker);

    if (headers.empty())
    {
      throw std::runtime_error("<PATTERN NOT FOUND>");
    }

    texts[new_text_id] = headers;
  }

  void TextProcessor::duplicateTextSection(
      const std::string &new_text_id, const std::string &source_text_id, int start_line, int end_line, int times)
  {
    auto it_src = texts.find(source_text_id);
    validation::checkIdNotFound(texts, source_text_id, "<TEXT NOT FOUND>");
    validation::checkIdExists(texts, new_text_id, "<TEXT ID EXISTS>");

    validation::checkLineRange(start_line, end_line, it_src->second.size());
    validation::checkPositive(times, "TIMES");

    std::vector< std::string > section;
    auto start = it_src->second.begin() + start_line - 1;
    auto end = it_src->second.begin() + end_line;

    struct SectionCopier
    {
      std::vector< std::string > &section;
      decltype(start) src_start;
      decltype(start) src_end;
      int remaining;

      SectionCopier(std::vector< std::string > &s, decltype(start) start_it, decltype(start) end_it, int rem):
        section(s),
        src_start(start_it),
        src_end(end_it),
        remaining(rem)
      {}

      void operator()()
      {
        if (remaining <= 0)
        {
          return;
        }

        std::copy(src_start, src_end, std::back_inserter(section));
        remaining--;
        (*this)();
      }
    };

    SectionCopier copier(section, start, end, times);
    copier();

    texts[new_text_id] = section;
  }
  void TextProcessor::listTexts() const
  {
    std::cout << "Texts: ";
    if (texts.empty())
    {
      std::cout << "none\n";
      return;
    }

    NamePrinter printer;
    std::for_each(texts.begin(), texts.end(), printer);
    std::cout << '\n';
  }

}
