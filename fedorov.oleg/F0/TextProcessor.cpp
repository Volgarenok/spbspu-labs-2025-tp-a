#include "TextProcessor.hpp"

#include <fstream>
#include <iostream>
#include <cctype>
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <iterator>
#include <functional>
#include <map>

#include "Utility.hpp"
#include "ValidationUtils.hpp"

namespace crossref
{
  namespace
  {
    struct WordProcessor
    {
      std::vector< std::string > &words;
      const TextProcessor &processor;

      void operator()(const std::string &word) const
      {
        std::string clean = processor.sanitizeWord(word);
        if (!clean.empty())
        {
          words.push_back(clean);
        }
      }
    };
  }

  std::string TextProcessor::sanitizeWord(std::string word)
  {
    word.erase(std::remove_if(word.begin(), word.end(), isNotAlpha), word.end());
    std::transform(word.begin(), word.end(), word.begin(), toLower);
    return word;
  }

  bool TextProcessor::isNotAlpha(unsigned char c)
  {
    return !isalpha(c);
  }

  char TextProcessor::toLower(char c)
  {
    return tolower(static_cast< unsigned char >(c));
  }

  void TextProcessor::processLine(HashTable &table, const std::string &line, int lineNumber) const
  {
    std::vector< std::string > words;
    splitRecursive(line, 0, words);

    std::vector< std::string > cleanWords;
    std::for_each(words.begin(), words.end(), WordProcessor{cleanWords, *this});

    struct TableInserter
    {
      HashTable &table;
      int lineNumber;
      void operator()(const std::string &word) const
      {
        table.insert(word, lineNumber);
      }
    };
    std::for_each(cleanWords.begin(), cleanWords.end(), TableInserter{table, lineNumber});
  }

  void TextProcessor::loadFile(const std::string &text_id, const std::string &filename)
  {
    validation::checkIdExists(texts, text_id, "<TEXT ID EXISTS>");
    std::ifstream file(filename);
    if (!file)
    {
      throw std::runtime_error("<FILE NOT FOUND>");
    }

    std::vector< Line > lines;
    std::copy(std::istream_iterator< Line >(file), std::istream_iterator< Line >(), std::back_inserter(lines));

    if (lines.empty())
    {
      throw std::runtime_error("<EMPTY FILE>");
    }

    std::vector< std::string > content;
    auto bind_t = std::bind(&Line::data, std::placeholders::_1);
    std::transform(lines.begin(), lines.end(), std::back_inserter(content), bind_t);
    texts[text_id] = content;
  }
}
