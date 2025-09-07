#include "analyzeFunctors.h"
#include <algorithm>
#include <cctype>

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
  std::for_each(p.second.begin(), p.second.end(), lineExtractor);
  std::sort(lines.begin(), lines.end());
  lines.erase(std::unique(lines.begin(), lines.end()), lines.end());
  out << p.first << ": ";
  PrintNumbers printer{ out };
  std::for_each(lines.begin(), lines.end(), printer);
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
  std::for_each(line.begin() + start, line.begin() + end, join);
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
  std::for_each(p.second.text.begin(), p.second.text.end(), textSaver);
  SaveWord wordSaver{ file };
  std::for_each(p.second.dictionary.begin(), p.second.dictionary.end(), wordSaver);
  file << '\n';
}

void khokhryakova::SaveTextLine::operator()(const std::vector< std::string >& line) const
{
  file << "text:";
  WordPrinter wp{ file };
  std::for_each(line.begin(), line.end(), wp);
  file << '\n';
}

void khokhryakova::SaveWord::operator()(const std::pair< std::string, std::vector<Position > >& p) const
{
  file << p.first << ':';
  SavePosition posSaver{ file };
  std::for_each(p.second.begin(), p.second.end(), posSaver);
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
  std::for_each(line.begin(), line.end(), wp);
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
