#include "analyzeFunctors.h"
#include <algorithm>
#include <cctype>
#include <regex>

char khokhryakova::ToLower::operator()(char c) const
{
  if (c >= 'A' && c <= 'Z')
  {
    return c - 'A' + 'a';
  }
  else
  {
    return c;
  }
}

void khokhryakova::cleanWord(std::string& word)
{
  ToLower toLower;
  std::transform(word.begin(), word.end(), word.begin(), toLower);
}

void khokhryakova::WordExtractor::operator()(const std::smatch& match) const
{
  std::string word = match.str();
  ToLower toLower;
  std::transform(word.begin(), word.end(), word.begin(), toLower);
  words.push_back(word);
  dictionary[word].push_back({ lineNum, colNum });
  ++colNum;
}

void khokhryakova::TextWordExtractor::operator()(const std::smatch& match) const
{
  std::string word = match.str();
  ToLower toLower;
  std::transform(word.begin(), word.end(), word.begin(), toLower);
  words.push_back(word);
}

void khokhryakova::PositionExtractor::operator()(const std::smatch& match) const
{
  size_t lineNum = std::stoul(match[1]);
  size_t colNum = std::stoul(match[2]);
  positions.push_back({ lineNum, colNum });
}

void khokhryakova::ExtractLine::operator()(const Position& pos)
{
  lines.push_back(pos.line);
}

void khokhryakova::PrintNumbers::operator()(size_t num) const
{
  if (!first)
  {
    out << ' ';
  }
  out << num;
  first = false;
}

void khokhryakova::PrintPair::operator()(const std::pair< std::string, std::vector< Position > >& p) const
{
  std::vector< size_t > lines;
  ExtractLine lineExtractor{ lines };

  struct RecursiveLineExtractor
  {
    const std::vector< Position >& positions;
    size_t index;
    ExtractLine& extractor;
    RecursiveLineExtractor(const std::vector< Position >& pos, size_t i, ExtractLine& ext):
      positions(pos), index(i), extractor(ext)
    {}
    void extract()
    {
      if (index < positions.size())
      {
        extractor(positions[index]);
        if (index + 1 < positions.size())
        {
          RecursiveLineExtractor next(positions, index + 1, extractor);
          next.extract();
        }
      }
    }
  };

  RecursiveLineExtractor extractor(p.second, 0, lineExtractor);
  extractor.extract();
  std::sort(lines.begin(), lines.end());
  lines.erase(std::unique(lines.begin(), lines.end()), lines.end());
  out << p.first << ": ";
  PrintNumbers printer{ out };

  struct RecursiveNumberPrinter
  {
    const std::vector< size_t >& numbers;
    size_t index;
    PrintNumbers& printer;
    RecursiveNumberPrinter(const std::vector< size_t >& nums, size_t i, PrintNumbers& p):
      numbers(nums), index(i), printer(p)
    {}
    void print()
    {
      if (index < numbers.size())
      {
        printer(numbers[index]);
        if (index + 1 < numbers.size())
        {
          RecursiveNumberPrinter next(numbers, index + 1, printer);
          next.print();
        }
      }
    }
  };

  RecursiveNumberPrinter numberPrinter(lines, 0, printer);
  numberPrinter.print();
  out << '\n';
}

void khokhryakova::JoinWords::operator()(const std::string& w) const
{
  if (!first)
  {
    out << ' ';
  }
  out << w;
  first = false;
}

void khokhryakova::ContextPrinter::operator()(const Position& pos) const
{
  if (pos.line == 0 || pos.line > text.size())
  {
    return;
  }
  const auto& line = text[pos.line - 1];
  if (pos.col == 0 || pos.col > line.size())
  {
    return;
  }
  const size_t start = (pos.col > radius) ? (pos.col - radius - 1) : 0;
  const size_t end = std::min(pos.col + radius, line.size());
  out << '"';
  JoinWords join{ out };

  struct RecursivePrinter
  {
    const std::vector< std::string >& line;
    size_t current;
    size_t end;
    JoinWords& joiner;
    RecursivePrinter(const std::vector< std::string >& l, size_t s, size_t e, JoinWords& j):
      line(l), current(s), end(e), joiner(j)
    {}
    void print() const
    {
      if (current < end && current < line.size())
      {
        joiner(line[current]);
        if (current + 1 < end)
        {
          RecursivePrinter next(line, current + 1, end, joiner);
          next.print();
        }
      }
    }
  };

  RecursivePrinter printer(line, start, end, join);
  printer.print();
  out << "\" (line " << pos.line << ")\n";
}

void khokhryakova::MergeFunctor::operator()(const std::pair< std::string, std::vector< Position > >& p) const
{
  std::copy(p.second.begin(), p.second.end(), std::back_inserter(dest[p.first]));
}

void khokhryakova::IntersectFunctor::operator()(const std::pair< std::string, std::vector< Position > >& p) const
{
  auto it = reference.find(p.first);
  if (it != reference.end())
  {
    auto& bucket = result[p.first];
    bucket = p.second;
    std::copy(it->second.begin(), it->second.end(), std::back_inserter(bucket));
  }
}

void khokhryakova::DiffFunctor::operator()(const std::pair< std::string, std::vector< Position > >& p) const
{
  if (reference.find(p.first) == reference.end() && result.find(p.first) == result.end())
  {
    result[p.first] = p.second;
  }
}

void khokhryakova::SaveDict::operator()(const std::pair< std::string, OneXrefDict >& p) const
{
  file << '[' << p.first << "]\n";
  SaveTextLine textSaver{ file };

  struct RecursiveTextSaver
  {
    const std::vector< std::vector< std::string > >& text;
    size_t index;
    SaveTextLine& saver;
    RecursiveTextSaver(const std::vector< std::vector< std::string > >& t, size_t i, SaveTextLine& s):
      text(t), index(i), saver(s)
    {}
    void save()
    {
      if (index < text.size())
      {
        saver(text[index]);
        if (index + 1 < text.size())
        {
          RecursiveTextSaver next(text, index + 1, saver);
          next.save();
        }
      }
    }
  };

  RecursiveTextSaver textProcessor(p.second.text, 0, textSaver);
  textProcessor.save();
  SaveWord wordSaver{ file };

  struct RecursiveWordSaver
  {
    std::map< std::string, std::vector< Position > >::const_iterator current;
    std::map< std::string, std::vector< Position > >::const_iterator end;
    SaveWord& saver;
    RecursiveWordSaver(std::map< std::string, std::vector< Position > >::const_iterator c,
        std::map< std::string, std::vector< Position > >::const_iterator e, SaveWord& s):
      current(c), end(e), saver(s)
    {}
    void save()
    {
      if (current != end)
      {
        saver(*current);
        auto next_it = current;
        ++next_it;
        if (next_it != end)
        {
          RecursiveWordSaver next(next_it, end, saver);
          next.save();
        }
      }
    }
  };

  RecursiveWordSaver wordProcessor(p.second.dictionary.begin(), p.second.dictionary.end(), wordSaver);
  wordProcessor.save();
  file << '\n';
}

void khokhryakova::SaveTextLine::operator()(const std::vector< std::string >& line) const
{
  file << "text:";
  WordPrinter wp{ file };

  struct RecursiveWordPrinter
  {
    const std::vector< std::string >& words;
    size_t index;
    WordPrinter& printer;
    RecursiveWordPrinter(const std::vector< std::string >& w, size_t i, WordPrinter& p):
      words(w), index(i), printer(p)
    {}
    void print()
    {
      if (index < words.size())
      {
        printer(words[index]);
        if (index + 1 < words.size())
        {
          RecursiveWordPrinter next(words, index + 1, printer);
          next.print();
        }
      }
    }
  };

  RecursiveWordPrinter wordProcessor(line, 0, wp);
  wordProcessor.print();
  file << '\n';
}

void khokhryakova::SaveWord::operator()(const std::pair< std::string, std::vector< Position > >& p) const
{
  file << p.first << ':';
  SavePosition posSaver{ file };

  struct RecursivePosSaver
  {
    const std::vector< Position >& positions;
    size_t index;
    SavePosition& saver;
    RecursivePosSaver(const std::vector< Position >& pos, size_t i, SavePosition& s):
      positions(pos), index(i), saver(s)
    {}
    void save()
    {
      if (index < positions.size())
      {
        saver(positions[index]);
        if (index + 1 < positions.size())
        {
          RecursivePosSaver next(positions, index + 1, saver);
          next.save();
        }
      }
    }
  };

  RecursivePosSaver posProcessor(p.second, 0, posSaver);
  posProcessor.save();
  file << '\n';
}

void khokhryakova::SavePosition::operator()(const Position& pos) const
{
  if (!first)
  {
    file << ' ';
  }
  file << '(' << pos.line << ',' << pos.col << ')';
  first = false;
}

void khokhryakova::TextRestorer::operator()(const std::vector< std::string >& line) const
{
  WordPrinter wp{ file };

  struct RecursiveWordPrinter
  {
    const std::vector< std::string >& words;
    size_t index;
    WordPrinter& printer;

    RecursiveWordPrinter(const std::vector< std::string >& w, size_t i, WordPrinter& p):
      words(w), index(i), printer(p)
    {}
    void print()
    {
      if (index < words.size())
      {
        printer(words[index]);
        if (index + 1 < words.size())
        {
          RecursiveWordPrinter next(words, index + 1, printer);
          next.print();
        }
      }
    }
  };

  RecursiveWordPrinter wordProcessor(line, 0, wp);
  wordProcessor.print();
  file << '\n';
}

void khokhryakova::WordPrinter::operator()(const std::string& word) const
{
  if (!first)
  {
    file << ' ';
  }
  file << word;
  first = false;
}

void khokhryakova::readFileLines(std::ifstream& file, std::vector< std::string >& lines)
{
  std::string line;
  if (std::getline(file, line))
  {
    lines.push_back(line);
    readFileLines(file, lines);
  }
}

void khokhryakova::processLineRecursive(const std::string& line, size_t lineNum, OneXrefDict& dict)
{
  std::vector< std::string > words;
  const std::regex kWordRegex("[A-Za-z]+");
  auto wordsBegin = std::sregex_iterator(line.begin(), line.end(), kWordRegex);
  auto wordsEnd = std::sregex_iterator();
  WordExtractor extractor{ words, dict.dictionary, lineNum };

  struct RecursiveProcessor
  {
    std::sregex_iterator current;
    std::sregex_iterator end;
    WordExtractor& extractor;
    RecursiveProcessor(std::sregex_iterator c, std::sregex_iterator e, WordExtractor& ex):
      current(c), end(e), extractor(ex)
    {}
    void process()
    {
      if (current != end)
      {
        extractor(*current);
        ++current;
        if (current != end)
        {
          RecursiveProcessor next(current, end, extractor);
          next.process();
        }
      }
    }
  };

  RecursiveProcessor processor(wordsBegin, wordsEnd, extractor);
  processor.process();
  if (!words.empty())
  {
    dict.text.push_back(words);
  }
}

void khokhryakova::processLoadLine(const std::string& line, std::string& currentId, OneXrefDict*& currentDict, XrefDictionary& dict)
{
  const std::regex kWordRegex("[A-Za-z]+");
  const std::regex posRegex("\\((\\d+),(\\d+)\\)");

  if (line.empty())
  {
    return;
  }
  else if (line[0] == '[')
  {
    currentId = line.substr(1, line.size() - 2);
    currentDict = &dict.dicts[currentId];
  }
  else if (line.find("text:") == 0)
  {
    std::vector< std::string > words;
    std::string textLine = line.substr(5);
    auto wordsBegin = std::sregex_iterator(textLine.begin(), textLine.end(), kWordRegex);
    auto wordsEnd = std::sregex_iterator();
    TextWordExtractor extractor{ words };

  struct RecursiveTextProcessor
  {
    std::sregex_iterator current;
    std::sregex_iterator end;
    TextWordExtractor& extractor;
    RecursiveTextProcessor(std::sregex_iterator c, std::sregex_iterator e, TextWordExtractor& ex):
      current(c), end(e), extractor(ex)
    {}
    void process()
    {
      if (current != end)
      {
        extractor(*current);
        ++current;
        if (current != end)
        {
          RecursiveTextProcessor next(current, end, extractor);
          next.process();
        }
      }
    }
  };

  RecursiveTextProcessor textProcessor(wordsBegin, wordsEnd, extractor);
  textProcessor.process();

  if (!words.empty() && currentDict)
  {
    currentDict->text.push_back(words);
  }
  }
  else
  {
    size_t colonPos = line.find(':');
    if (colonPos != std::string::npos && currentDict != nullptr)
    {
      std::string word = line.substr(0, colonPos);
      std::string positions = line.substr(colonPos + 1);
      auto posBegin = std::sregex_iterator(positions.begin(), positions.end(), posRegex);
      auto posEnd = std::sregex_iterator();
      PositionExtractor posExtractor{ currentDict->dictionary[word] };

    struct RecursivePosProcessor
    {
      std::sregex_iterator current;
      std::sregex_iterator end;
      PositionExtractor& extractor;

      RecursivePosProcessor(std::sregex_iterator c, std::sregex_iterator e, PositionExtractor& ex):
        current(c), end(e), extractor(ex)
      {}
      void process()
      {
        if (current != end)
        {
          extractor(*current);
          ++current;
          if (current != end)
          {
            RecursivePosProcessor next(current, end, extractor);
            next.process();
          }
        }
      }
    };

    RecursivePosProcessor posProcessor(posBegin, posEnd, posExtractor);
    posProcessor.process();
    }
  }
}
