#include "XrefDictionary.h"
#include <fstream>
#include <algorithm>
#include <regex>
#include <sstream>
#include "analyzeFunctors.h"

namespace
{
  const std::regex kWordRegex("[A-Za-z]+");
  struct IsEmptyVec
  {
    bool operator()(const std::vector< std::string >& v) const
    {
      return v.empty();
    }
  };
}

void khokhryakova::buildXref(std::istream& in, std::ostream& out, XrefDictionary& dict)
{
  std::string fileName, id;
  in >> fileName >> id;
  if (dict.dicts.find(id) != dict.dicts.end())
  {
    out << "<BusyID>\n";
    return;
  }
  std::ifstream file(fileName, std::ios::binary);
  if (!file.is_open())
  {
    out << "<FileError>\n";
    return;
  }
  OneXrefDict newDict;
  std::string line;
  size_t lineNum = 1;
  while (std::getline(file, line))
  {
    std::vector< std::string > words;
    auto wordsBegin = std::sregex_iterator(line.begin(), line.end(), kWordRegex);
    auto wordsEnd = std::sregex_iterator();
    WordExtractor extractor{ words, newDict.dictionary, lineNum };
    std::for_each(wordsBegin, wordsEnd, extractor);
    if (!words.empty())
    {
      newDict.text.push_back(words);
    }
    ++lineNum;
  }
  const bool empty = std::all_of(newDict.text.begin(), newDict.text.end(), IsEmptyVec{});
  if (empty)
  {
    out << "<EmptyText>\n";
    return;
  }
  dict.dicts[id] = std::move(newDict);
  std::cout << "Cross references for file \"" << fileName << "\" saved to dictionary \"" << id << "\"" << '\n';
}

void khokhryakova::printDict(std::istream& in, std::ostream& out, const XrefDictionary& dict)
{
  std::string id;
  in >> id;
  auto it = dict.dicts.find(id);
  if (it == dict.dicts.end())
  {
    out << "<DictionaryError>\n";
    return;
  }
  PrintPair printer{ out };
  std::for_each(it->second.dictionary.begin(), it->second.dictionary.end(), printer);
}

void khokhryakova::findWord(std::istream& in, std::ostream& out, const XrefDictionary& dict)
{
  std::string word, id;
  in >> word >> id;
  auto it = dict.dicts.find(id);
  if (it == dict.dicts.end())
  {
    out << "<DictionaryError>\n";
    return;
  }
  cleanWord(word);
  auto wordIt = it->second.dictionary.find(word);
  if (wordIt == it->second.dictionary.end())
  {
    out << "<WordError>\n";
    return;
  }
  std::vector< size_t > lines;
  ExtractLine lineExtractor{ lines };
  std::for_each(wordIt->second.begin(), wordIt->second.end(), lineExtractor);
  std::sort(lines.begin(), lines.end());
  lines.erase(std::unique(lines.begin(), lines.end()), lines.end());
  out << "\"" << word << "\" occurs in lines: ";
  PrintNumbers printer{ out };
  std::for_each(lines.begin(), lines.end(), printer);
  out << "\n";
}

void khokhryakova::countWord(std::istream& in, std::ostream& out, const XrefDictionary& dict)
{
  std::string word, id;
  in >> word >> id;
  auto it = dict.dicts.find(id);
  if (it == dict.dicts.end())
  {
    out << "<DictionaryError>\n";
    return;
  }
  cleanWord(word);
  auto wordIt = it->second.dictionary.find(word);
  if (wordIt == it->second.dictionary.end())
  {
    out << "<WordError>\n";
    return;
  }
  out << "Word \"" << word << "\" occurs " << wordIt->second.size() << " times\n";
}

void khokhryakova::getContext(std::istream& in, std::ostream& out, const XrefDictionary& dict)
{
  std::string word, id;
  int radius;
  in >> word >> radius >> id;
  if (radius <= 0)
  {
    out << "<RadiusError>\n";
    return;
  }
  auto it = dict.dicts.find(id);
  if (it == dict.dicts.end())
  {
    out << "<DictionaryError>\n";
    return;
  }
  cleanWord(word);
  auto wordIt = it->second.dictionary.find(word);
  if (wordIt == it->second.dictionary.end())
  {
    out << "<WordError>\n";
    return;
  }
  ContextPrinter ctxPrinter{ out, it->second.text, static_cast< size_t >(radius) };
  std::for_each(wordIt->second.begin(), wordIt->second.end(), ctxPrinter);
}

void khokhryakova::mergeXref(std::istream& in, std::ostream& out, XrefDictionary& dict)
{
  std::string id1, id2, newId;
  in >> id1 >> id2 >> newId;
  if (dict.dicts.find(newId) != dict.dicts.end())
  {
    out << "<BusyID>\n";
    return;
  }
  auto it1 = dict.dicts.find(id1);
  auto it2 = dict.dicts.find(id2);
  if (it1 == dict.dicts.end() || it2 == dict.dicts.end())
  {
    out << "<DictionaryError>\n";
    return;
  }
  OneXrefDict merged = it1->second;
  MergeFunctor merger{ merged.dictionary };
  std::for_each(it2->second.dictionary.begin(), it2->second.dictionary.end(), merger);
  dict.dicts[newId] = std::move(merged);
  std::cout << "Dictionaries \"" << id1 << "\" and \"" << id2 << "\" merged into \"" << newId << "\"" << '\n';
}

void khokhryakova::intersectXref(std::istream& in, std::ostream& out, XrefDictionary& dict)
{
  std::string id1, id2, newId;
  in >> id1 >> id2 >> newId;
  if (dict.dicts.find(newId) != dict.dicts.end())
  {
    out << "<BusyID>\n";
    return;
  }
  auto it1 = dict.dicts.find(id1);
  auto it2 = dict.dicts.find(id2);
  if (it1 == dict.dicts.end() || it2 == dict.dicts.end())
  {
    out << "<DictionaryError>\n";
    return;
  }
  OneXrefDict result;
  result.text = it1->second.text;
  IntersectFunctor intersector{ it2->second.dictionary, result.dictionary };
  std::for_each(it1->second.dictionary.begin(), it1->second.dictionary.end(), intersector);
  if (result.dictionary.empty())
  {
    out << "<NO INTERSECTIONS>\n";
    return;
  }
  dict.dicts[newId] = std::move(result);
  std::cout << "Dictionaries \"" << id1 << "\" and \"" << id2 << "\" intersect into \"" << newId << "\"" << '\n';
}

void khokhryakova::diffXref(std::istream& in, std::ostream& out, XrefDictionary& dict)
{
  std::string id1, id2, newId;
  in >> id1 >> id2 >> newId;
  if (dict.dicts.find(newId) != dict.dicts.end())
  {
    out << "<BusyID>\n";
    return;
  }
  auto it1 = dict.dicts.find(id1);
  auto it2 = dict.dicts.find(id2);
  if (it1 == dict.dicts.end() || it2 == dict.dicts.end())
  {
    out << "<DictionaryError>\n";
    return;
  }
  OneXrefDict result;
  result.text = it1->second.text;
  DiffFunctor differ1{ it2->second.dictionary, result.dictionary };
  std::for_each(it1->second.dictionary.begin(), it1->second.dictionary.end(), differ1);
  DiffFunctor differ2{ it1->second.dictionary, result.dictionary };
  std::for_each(it2->second.dictionary.begin(), it2->second.dictionary.end(), differ2);
  if (result.dictionary.empty())
  {
    out << "<NO DIFFERENCES>\n";
    return;
  }
  dict.dicts[newId] = std::move(result);
  std::cout << "Dictionaries \"" << id1 << "\" and \"" << id2 << "\" difference in \"" << newId << "\"" << '\n';
}

void khokhryakova::saveXref(std::istream& in, std::ostream& out, const XrefDictionary& dict)
{
  std::string fileName;
  in >> fileName;
  std::ofstream file(fileName, std::ios::binary);
  if (!file.is_open())
  {
    out << "<FileError>\n";
    return;
  }
  SaveDict saver{ file };
  std::for_each(dict.dicts.begin(), dict.dicts.end(), saver);
  out << "<SavedSuccessfully>\n";
}

void khokhryakova::loadXref(std::istream& in, std::ostream& out, XrefDictionary& dict)
{
  std::string fileName;
  in >> fileName;
  loadXrefFromPath(out, fileName, dict);
}

void khokhryakova::loadXrefFromPath(std::ostream& out, const std::string& fileName, XrefDictionary& dict)
{
  std::ifstream file(fileName, std::ios::binary);
  if (!file.is_open())
  {
    out << "<FileError>\n";
    return;
  }
  dict.dicts.clear();
  std::string line;
  std::string currentId;
  OneXrefDict* currentDict = nullptr;
  const std::regex posRegex("\\((\\d+),(\\d+)\\)");
  while (std::getline(file, line))
  {
    if (line.empty())
    {
      continue;
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
      std::for_each(wordsBegin, wordsEnd, extractor);
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
        std::for_each(posBegin, posEnd, posExtractor);
      }
    }
  }
  out << "<LoadedSuccessfully>\n";
}

void khokhryakova::restoreText(std::istream& in, std::ostream& out, const XrefDictionary& dict)
{
  std::string id, outputFile;
  in >> id >> outputFile;
  auto it = dict.dicts.find(id);
  if (it == dict.dicts.end())
  {
    out << "<DictionaryError>\n";
    return;
  }
  std::ofstream file(outputFile, std::ios::binary);
  if (!file.is_open())
  {
    out << "<FileError>\n";
    return;
  }
  TextRestorer restorer{ file };
  std::for_each(it->second.text.begin(), it->second.text.end(), restorer);
  out << "Text restored from \"" << id << "\" and written to \"" << outputFile << "\"\n";
}

void khokhryakova::showHelp(std::istream&, std::ostream& out)
{
  out << "Text analyzer with cross-reference dictionary\n";
  out << "Commands:\n";
  out << "BUILD <file> <id> - build cross-reference dictionary\n";
  out << "PRINT <id> - print dictionary\n";
  out << "FIND <word> <id> - find word occurrences\n";
  out << "COUNT <word> <id> - count word occurrences\n";
  out << "CONTEXT <word> <radius> <id> - show word context\n";
  out << "MERGE <id1> <id2> <new_id> - merge dictionaries\n";
  out << "INTERSECT <id1> <id2> <new_id> - intersect dictionaries\n";
  out << "DIFF <id1> <id2> <new_id> - difference of dictionaries\n";
  out << "SAVE <file> - save dictionaries\n";
  out << "LOAD <file> - load dictionaries\n";
  out << "RESTORE <id> <file> - restore text from dictionary\n";
}
