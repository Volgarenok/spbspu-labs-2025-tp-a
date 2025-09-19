#include "XrefDictionary.h"
#include <fstream>
#include <algorithm>
#include <regex>
#include "analyzeFunctors.h"

void khokhryakova::buildXref(std::istream& in, std::ostream& out, XrefDictionary& dict)
{
  std::string fileName;
  std::string id;
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
  std::vector< std::string > lines;
  readFileLines(file, lines);
  if (lines.empty())
  {
    out << "<EmptyText>\n";
    return;
  }

  struct RecursiveLineProcessor
  {
    const std::vector< std::string >& lines;
    size_t index;
    OneXrefDict& dict;
    RecursiveLineProcessor(const std::vector< std::string >& l, size_t i, OneXrefDict& d):
      lines(l), index(i), dict(d)
    {}
    void process()
    {
      if (index < lines.size())
      {
        processLineRecursive(lines[index], index + 1, dict);
        if (index + 1 < lines.size())
        {
          RecursiveLineProcessor next(lines, index + 1, dict);
          next.process();
        }
      }
    }
  };

  RecursiveLineProcessor processor(lines, 0, newDict);
  processor.process();

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

  struct RecursiveDictPrinter
  {
    std::map< std::string, std::vector< Position > >::const_iterator current;
    std::map< std::string, std::vector< Position > >::const_iterator end;
    PrintPair& printer;
    RecursiveDictPrinter(std::map< std::string, std::vector< Position > >::const_iterator c,
        std::map< std::string, std::vector< Position > >::const_iterator e, PrintPair& p):
      current(c), end(e), printer(p)
    {}
    void print()
    {
      if (current != end)
      {
        printer(*current);
        auto next_it = current;
        ++next_it;
        if (next_it != end)
        {
          RecursiveDictPrinter next(next_it, end, printer);
          next.print();
        }
      }
    }
  };

  RecursiveDictPrinter dictPrinter(it->second.dictionary.begin(), it->second.dictionary.end(), printer);
  dictPrinter.print();
}

void khokhryakova::findWord(std::istream& in, std::ostream& out, const XrefDictionary& dict)
{
  std::string word;
  std::string id;
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

  struct RecursiveExtractor
  {
    const std::vector< Position >& positions;
    size_t index;
    ExtractLine& extractor;
    RecursiveExtractor(const std::vector< Position >& pos, size_t i, ExtractLine& ext):
      positions(pos), index(i), extractor(ext)
    {}
    void extract()
    {
      if (index < positions.size())
      {
        extractor(positions[index]);
        if (index + 1 < positions.size())
        {
          RecursiveExtractor next(positions, index + 1, extractor);
          next.extract();
        }
      }
    }
  };

  RecursiveExtractor extractor(wordIt->second, 0, lineExtractor);
  extractor.extract();
  std::sort(lines.begin(), lines.end());
  lines.erase(std::unique(lines.begin(), lines.end()), lines.end());
  out << "\"" << word << "\" occurs in lines: ";
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
  RecursiveNumberPrinter numPrinter(lines, 0, printer);
  numPrinter.print();
  out << "\n";
}

void khokhryakova::countWord(std::istream& in, std::ostream& out, const XrefDictionary& dict)
{
  std::string word;
  std::string id;
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
  std::string word;
  std::string id;
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

  struct RecursiveContextPrinter
  {
    const std::vector< Position >& positions;
    size_t index;
    ContextPrinter& printer;
    RecursiveContextPrinter(const std::vector< Position >& pos, size_t i, ContextPrinter& p):
      positions(pos), index(i), printer(p)
    {}
    void print()
    {
      if (index < positions.size())
      {
        printer(positions[index]);
        if (index + 1 < positions.size())
        {
          RecursiveContextPrinter next(positions, index + 1, printer);
          next.print();
        }
      }
    }
  };

  RecursiveContextPrinter ctxProcessor(wordIt->second, 0, ctxPrinter);
  ctxProcessor.print();
}

void khokhryakova::mergeXref(std::istream& in, std::ostream& out, XrefDictionary& dict)
{
  std::string id1;
  std::string id2;
  std::string newId;
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

  struct RecursiveMerger
  {
    std::map< std::string, std::vector< Position > >::const_iterator current;
    std::map< std::string, std::vector< Position > >::const_iterator end;
    MergeFunctor& merger;
    RecursiveMerger(std::map< std::string, std::vector< Position > >::const_iterator c,
        std::map< std::string, std::vector< Position > >::const_iterator e, MergeFunctor& m):
      current(c), end(e), merger(m)
    {}
    void merge()
    {
      if (current != end)
      {
        merger(*current);
        auto next_it = current;
        ++next_it;
        if (next_it != end)
        {
          RecursiveMerger next(next_it, end, merger);
          next.merge();
        }
      }
    }
  };

  RecursiveMerger mergeProcessor(it2->second.dictionary.begin(), it2->second.dictionary.end(), merger);
  mergeProcessor.merge();
  dict.dicts[newId] = std::move(merged);
  std::cout << "Dictionaries \"" << id1 << "\" and \"" << id2 << "\" merged into \"" << newId << "\"" << '\n';
}

void khokhryakova::intersectXref(std::istream& in, std::ostream& out, XrefDictionary& dict)
{
  std::string id1;
  std::string id2;
  std::string newId;
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

  struct RecursiveIntersector
  {
    std::map< std::string, std::vector< Position > >::const_iterator current;
    std::map< std::string, std::vector< Position > >::const_iterator end;
    IntersectFunctor& intersector;
    RecursiveIntersector(std::map< std::string, std::vector< Position > >::const_iterator c,
        std::map< std::string, std::vector< Position > >::const_iterator e, IntersectFunctor& i):
      current(c), end(e), intersector(i)
    {}
    void intersect()
    {
      if (current != end)
      {
        intersector(*current);
        auto next_it = current;
        ++next_it;
        if (next_it != end)
        {
          RecursiveIntersector next(next_it, end, intersector);
          next.intersect();
        }
      }
    }
  };

  RecursiveIntersector intProcessor(it1->second.dictionary.begin(), it1->second.dictionary.end(), intersector);
  intProcessor.intersect();
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
  std::string id1;
  std::string id2;
  std::string newId;
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

  struct RecursiveDiffer
  {
    std::map< std::string, std::vector< Position > >::const_iterator current;
    std::map< std::string, std::vector< Position > >::const_iterator end;
    DiffFunctor& differ;
    RecursiveDiffer(std::map< std::string, std::vector< Position > >::const_iterator c,
        std::map< std::string, std::vector< Position > >::const_iterator e, DiffFunctor& d):
      current(c), end(e), differ(d)
    {}
    void diff()
    {
      if (current != end)
      {
        differ(*current);
        auto next_it = current;
        ++next_it;
        if (next_it != end)
        {
          RecursiveDiffer next(next_it, end, differ);
          next.diff();
        }
      }
    }
  };

  RecursiveDiffer diffProcessor1(it1->second.dictionary.begin(), it1->second.dictionary.end(), differ1);
  diffProcessor1.diff();
  DiffFunctor differ2{ it1->second.dictionary, result.dictionary };
  RecursiveDiffer diffProcessor2(it2->second.dictionary.begin(), it2->second.dictionary.end(), differ2);
  diffProcessor2.diff();
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

  struct RecursiveSaver
  {
    std::unordered_map< std::string, OneXrefDict >::const_iterator current;
    std::unordered_map< std::string, OneXrefDict >::const_iterator end;
    SaveDict& saver;
    RecursiveSaver(std::unordered_map< std::string, OneXrefDict >::const_iterator c,
        std::unordered_map< std::string, OneXrefDict >::const_iterator e, SaveDict& s):
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
          RecursiveSaver next(next_it, end, saver);
          next.save();
        }
      }
    }
  };

  RecursiveSaver saveProcessor(dict.dicts.begin(), dict.dicts.end(), saver);
  saveProcessor.save();
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
  std::vector< std::string > lines;
  readFileLines(file, lines);
  std::string currentId;
  OneXrefDict* currentDict = nullptr;

  struct RecursiveLoadProcessor
  {
    const std::vector< std::string >& lines;
    size_t index;
    std::string& currentId;
    OneXrefDict*& currentDict;
    XrefDictionary& dict;
    RecursiveLoadProcessor(const std::vector< std::string >& l, size_t i, std::string& id, OneXrefDict*& cd, XrefDictionary& d):
      lines(l), index(i), currentId(id), currentDict(cd), dict(d)
    {}
    void process()
    {
      if (index < lines.size())
      {
        processLoadLine(lines[index], currentId, currentDict, dict);
        if (index + 1 < lines.size())
        {
          RecursiveLoadProcessor next(lines, index + 1, currentId, currentDict, dict);
          next.process();
        }
      }
    }
  };

  RecursiveLoadProcessor loadProcessor(lines, 0, currentId, currentDict, dict);
  loadProcessor.process();
  out << "<LoadedSuccessfully>\n";
}

void khokhryakova::restoreText(std::istream& in, std::ostream& out, const XrefDictionary& dict)
{
  std::string id;
  std::string outputFile;
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

  struct RecursiveRestorer
  {
    const std::vector< std::vector< std::string > >& text;
    size_t index;
    TextRestorer& restorer;
    RecursiveRestorer(const std::vector< std::vector< std::string > >& t, size_t i, TextRestorer& r):
      text(t), index(i), restorer(r)
    {}
    void restore()
    {
      if (index < text.size())
      {
        restorer(text[index]);
        if (index + 1 < text.size())
        {
          RecursiveRestorer next(text, index + 1, restorer);
          next.restore();
        }
      }
    }
  };

  RecursiveRestorer restoreProcessor(it->second.text, 0, restorer);
  restoreProcessor.restore();
  out << "Text restored from \"" << id << "\" and written to \"" << outputFile << "\"\n";
}

void khokhryakova::insertWord(std::istream& in, std::ostream& out, XrefDictionary& dict)
{
  std::string dictName;
  std::string word;
  int lineNum;
  int position;
  in >> dictName >> lineNum >> position >> word;
  auto it = dict.dicts.find(dictName);
  if (it == dict.dicts.end())
  {
    out << "<DictionaryError>\n";
    return;
  }
  if (lineNum <= 0 || lineNum > static_cast< int >(it->second.text.size()))
  {
    out << "<PositionError>\n";
    return;
  }
  auto& line = it->second.text[lineNum - 1];
  if (position < 0 || position > static_cast< int >(line.size()))
  {
    out << "<PositionError>\n";
    return;
  }
  cleanWord(word);
  line.insert(line.begin() + position, word);
  it->second.dictionary[word].push_back({ static_cast< size_t >(lineNum), static_cast< size_t >(position + 1) });
  out << "Word \"" << word << "\" added to line " << lineNum << " at position " << position << "\n";
}

void khokhryakova::deleteWord(std::istream& in, std::ostream& out, XrefDictionary& dict)
{
  std::string dictName;
  int lineNum;
  int position;
  in >> dictName >> lineNum >> position;
  auto it = dict.dicts.find(dictName);
  if (it == dict.dicts.end())
  {
    out << "<DictionaryError>\n";
    return;
  }
  if (lineNum <= 0 || lineNum > static_cast< int >(it->second.text.size()))
  {
    out << "<PositionError>\n";
    return;
  }
  auto& line = it->second.text[lineNum - 1];
  if (position <= 0 || position > static_cast< int >(line.size()))
  {
    out << "<PositionError>\n";
    return;
  }
  std::string deletedWord = line[position - 1];
  line.erase(line.begin() + position - 1);
  out << "Word \"" << deletedWord << "\" deleted from line " << lineNum << " at position " << position << "\n";
}

void khokhryakova::outputWord(std::istream& in, std::ostream& out, const XrefDictionary& dict)
{
  std::string dictName;
  int lineNum;
  int position;
  in >> dictName >> lineNum >> position;
  auto it = dict.dicts.find(dictName);
  if (it == dict.dicts.end())
  {
    out << "<DictionaryError>\n";
    return;
  }
  if (lineNum <= 0 || lineNum > static_cast< int >(it->second.text.size()))
  {
    out << "<PositionError>\n";
    return;
  }
  const auto& line = it->second.text[lineNum - 1];
  if (position <= 0 || position > static_cast< int >(line.size()))
  {
    out << "<PositionError>\n";
    return;
  }
  out << "Word \"" << line[position - 1] << "\" found at line " << lineNum << " position " << position << "\n";
}

void khokhryakova::showHelp(std::istream&, std::ostream& out)
{
  out << "Text analyzer with cross-reference dictionary\n";
  out << "Commands:\n";
  out << "BUILD <file_path> <dict_name> - build cross-reference dictionary\n";
  out << "PRINT <dict_name> - print dictionary\n";
  out << "FIND <word> <dict_name> - find word occurrences\n";
  out << "COUNT <word> <dict_name> - count word occurrences\n";
  out << "CONTEXT <word> <radius> <dict_name> - show word context\n";
  out << "MERGE <dict1> <dict2> <result_dict> - merge dictionaries\n";
  out << "INTERSECT <dict1> <dict2> <result_dict> - intersect dictionaries\n";
  out << "DIFF <dict1> <dict2> <result_dict> - difference of dictionaries\n";
  out << "SAVE <dict_name> - save dictionaries\n";
  out << "LOAD <dict_name> - load dictionaries\n";
  out << "RESTORE <dict_name> <output_file> - restore text from dictionary\n";
  out << "INSERT_WORD <dict_name> <line_num> <position> <word> - insert word\n";
  out << "DELETE_WORD <dict_name> <line_num> <position> - delete word\n";
  out << "OUTPUT_WORD <dict_name> <line_num> <position> - output word\n";
}
