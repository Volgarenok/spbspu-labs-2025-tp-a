#include "commands.hpp"
#include "dictionary.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <limits>
#include <cstdlib>
#include <iterator>
#include <algorithm>

namespace {

struct PrintWordTranslation {
  void operator()(const std::pair<std::string, std::string> &p) const {
    std::cout << p.first << ": " << p.second << '\n';
  }
};

struct PrintWord {
  void operator()(const std::string &word) const {
    std::cout << word << '\n';
  }
};

struct WordLengthSum {
  std::size_t operator()(std::size_t acc, const std::pair<std::string, std::string> &p) const {
    return acc + p.second.size();
  }
};

struct WordFinderPrinter {
  const std::string &word;
  explicit WordFinderPrinter(const std::string &w) : word(w) {}
  void operator()(const std::pair<const std::string, Dictionary> &entry) const {
    std::string result;
    if (entry.second.findWord(word, result)) {
      std::cout << entry.first << ": " << result << '\n';
    }
  }
};

}

void CommandHandler::handleCommand(const std::string &line)
{
  std::istringstream iss(line);
  std::string cmd;
  iss >> cmd;

  if (cmd == "--help") {
    printHelp();
    return;
  }

  if (cmd == "exit") {
    std::exit(0);
  }

  std::string dictName;
  iss >> dictName;

  if (dictName.empty() && cmd != "translate-all") {
    std::cout << "INVALID_COMMAND" << '\n';
    return;
  }

  Dictionary &dict = dictionaries_[dictName];

  if (cmd == "add") {
    std::string word, translation;
    iss >> word >> std::ws;
    std::getline(iss, translation);
    std::string dummy;
    if (dict.findWord(word, dummy)) {
      std::cout << "WORD_EXISTS" << '\n';
    } else {
      dict.addWord(word, translation);
    }
  } else if (cmd == "remove") {
    std::string word;
    iss >> word;
    std::string dummy;
    if (!dict.findWord(word, dummy)) {
      std::cout << "WORD_NOT_FOUND" << '\n';
    } else {
      dict.removeWord(word);
    }
  } else if (cmd == "edit") {
    std::string word, newTranslation;
    iss >> word >> std::ws;
    std::getline(iss, newTranslation);
    std::string dummy;
    if (!dict.findWord(word, dummy)) {
      std::cout << "WORD_NOT_FOUND" << '\n';
    } else {
      dict.editWord(word, newTranslation);
    }
  } else if (cmd == "search") {
    std::string word, translation;
    iss >> word;
    if (dict.findWord(word, translation)) {
      std::cout << translation << '\n';
    } else {
      std::cout << "NO_RESULTS" << '\n';
    }
  } else if (cmd == "list") {
    std::string prefix;
    iss >> prefix;
    std::vector<std::string> results = dict.listWordsWithPrefix(prefix);

    struct RecursivePrintWord {
      const std::vector<std::string> &words;
      std::size_t index;
      RecursivePrintWord(const std::vector<std::string> &w) : words(w), index(0) {}
      void print() {
        if (index >= words.size()) return;
        std::cout << words[index] << '\n';
        ++index;
        print();
      }
    };
    RecursivePrintWord printer(results);
    printer.print();

  } else if (cmd == "export") {
    std::string filename;
    iss >> filename;
    exportDict(dict, filename);
  } else if (cmd == "import") {
    std::string filename;
    iss >> filename;
    importDict(dict, filename);
  } else if (cmd == "stats") {
    printStats(dict);
  } else if (cmd == "quiz") {
    std::size_t count = 5;
    iss >> count;
    quiz(dict, count);
  } else if (cmd == "clear") {
    std::cout << "Are you sure? (y/n)" << '\n';
    std::string confirm;
    std::getline(std::cin >> std::ws, confirm);
    if (confirm == "y") {
      dict.clear();
    }
    return;
  } else if (cmd == "find-by-translation") {
    std::string fragment;
    iss >> fragment;
    std::vector<std::string> results = dict.getWordsByTranslationFragment(fragment);

struct RecursivePrintWord {
      const std::vector<std::string> &words;
      std::size_t index;
      RecursivePrintWord(const std::vector<std::string> &w) : words(w), index(0) {}
      void print() {
        if (index >= words.size()) return;
        std::cout << words[index] << '\n';
        ++index;
        print();
      }
    };
    RecursivePrintWord printer(results);
    printer.print();

  } else if (cmd == "sort") {
    std::vector<std::pair<std::string, std::string>> all = dict.getAllWords();
    std::sort(all.begin(), all.end());

    struct RecursivePrintPair {
      const std::vector<std::pair<std::string, std::string>> &pairs;
      std::size_t index;
      RecursivePrintPair(const std::vector<std::pair<std::string, std::string>> &p) : pairs(p), index(0) {}
      void print() {
        if (index >= pairs.size()) return;
        std::cout << pairs[index].first << ": " << pairs[index].second << '\n';
        ++index;
        print();
      }
    };
    RecursivePrintPair printer(all);
    printer.print();

  } else if (cmd == "random") {
    std::pair<std::string, std::string> pair = dict.getRandomPair();
    std::cout << pair.first << ": " << pair.second << '\n';
  } else if (cmd == "merge") {
    std::string otherName, newName;
    iss >> otherName >> newName;
    Dictionary &other = dictionaries_[otherName];
    Dictionary &newDict = dictionaries_[newName];
    newDict = dict;
    newDict.mergeFrom(other);
  } else if (cmd == "intersect") {
    std::string otherName, newName;
    iss >> otherName >> newName;
    Dictionary &other = dictionaries_[otherName];
    dictionaries_[newName] = dict.intersectWith(other);
  } else if (cmd == "diff") {
    std::string otherName, newName;
    iss >> otherName >> newName;
    Dictionary &other = dictionaries_[otherName];
    dictionaries_[newName] = dict.diffFrom(other);
  } else if (cmd == "compare") {
    std::string otherName;
    iss >> otherName;
    compareDicts(dict, dictionaries_[otherName]);
  } else if (cmd == "extract") {
    std::string start, end, newName;
    iss >> start >> end >> newName;
    dictionaries_[newName] = dict.extractRange(start, end);
  } else if (cmd == "translate-all") {
    std::string word;
    iss >> word;

    struct RecursiveTranslateAll {
      const std::string &word;
      std::map<std::string, Dictionary>::const_iterator current;
      std::map<std::string, Dictionary>::const_iterator end;

      RecursiveTranslateAll(const std::string &w,
                            std::map<std::string, Dictionary>::const_iterator c,
                            std::map<std::string, Dictionary>::const_iterator e)
          : word(w), current(c), end(e) {}

      void operator()() const {
        if (current == end) {
          return;
        }
        std::string translation;
        if (current->second.findWord(word, translation)) {
          std::cout << current->first << ": " << translation << '\n';
        }
        RecursiveTranslateAll next(word, std::next(current), end);
        next();
      }
    };

    RecursiveTranslateAll printer(word, dictionaries_.cbegin(), dictionaries_.cend());
    printer();
  } else if (cmd == "swap-translations") {
    dict.swapTranslations();
  } else {
    std::cout << "UNKNOWN_COMMAND" << '\n';
  }
}

void CommandHandler::printHelp() const
{
  std::cout << "Available commands:\n";
  std::cout << "  add <dict> <word> <translation>\n";
  std::cout << "      Add word with translation\n";
  std::cout << "  remove <dict> <word>\n";
  std::cout << "      Remove word from dictionary\n";
  std::cout << "  edit <dict> <word> <new_translation>\n";
  std::cout << "      Edit translation of a word\n";
  std::cout << "  search <dict> <word>\n";
  std::cout << "      Search for a word's translation\n";
  std::cout << "  list <dict> <prefix>\n";
  std::cout << "      List words starting with prefix\n";
  std::cout << "  export <dict> <filename>\n";
  std::cout << "      Save dictionary to file\n";
  std::cout << "  import <dict> <filename>\n";
  std::cout << "      Load dictionary from file\n";
  std::cout << "  stats <dict>\n";
  std::cout << "      Show word count and average translation length\n";
  std::cout << "  quiz <dict> <count>\n";
  std::cout << "      Quiz with optional question count (default 5)\n";
  std::cout << "  clear <dict>\n";
  std::cout << "      Clear dictionary (with confirmation)\n";
  std::cout << "  find-by-translation <dict> <fragment>\n";
  std::cout << "      Find words by translation fragment\n";
  std::cout << "  sort <dict>\n";
  std::cout << "      Print dictionary in alphabetical order\n";
  std::cout << "  random <dict>\n";
  std::cout << "      Show a random word and its translation\n";
  std::cout << "  merge <dict1> <dict2> <new_dict>\n";
  std::cout << "      Merge two dictionaries\n";
  std::cout << "  intersect <dict1> <dict2> <new_dict>\n";
  std::cout << "      Dictionary of words common to both\n";
  std::cout << "  diff <dict1> <dict2> <new_dict>\n";
  std::cout << "      Words in dict1 but not in dict2\n";
  std::cout << "  compare <dict1> <dict2>\n";
  std::cout << "      Compare dictionaries\n";
  std::cout << "  extract <dict> <start> <end> <new_dict>\n";
  std::cout << "      Extract range by first letter\n";
  std::cout << "  translate-all <word>\n";
  std::cout << "      Show all translations of a word\n";
  std::cout << "  swap-translations <dict>\n";
  std::cout << "      Make dictionary Russian-English\n";
  std::cout << "  exit\n";
  std::cout << "      Exit the program\n";
}

void CommandHandler::printStats(const Dictionary &dict) const
{
  std::size_t total = dict.getWordCount();
  std::vector<std::pair<std::string, std::string> > all = dict.getAllWords();

  struct LengthSum {
    std::vector<std::pair<std::string, std::string> >::const_iterator current;
    std::vector<std::pair<std::string, std::string> >::const_iterator end;
    std::size_t sum;
    LengthSum(std::vector<std::pair<std::string, std::string> >::const_iterator b,
              std::vector<std::pair<std::string, std::string> >::const_iterator e)
      : current(b), end(e), sum(0) {}
    void compute() {
      if (current == end) return;
      sum += current->second.size();
      ++current;
      compute();
    }
  };

  LengthSum lengthSum(all.begin(), all.end());
  lengthSum.compute();

  std::cout << "Words: " << total << '\n';
  if (total > 0) {
    std::cout << "Average translation length: " << lengthSum.sum / total << '\n';
  }
}

void CommandHandler::quiz(const Dictionary &dict, std::size_t count) const
{
  std::srand(std::time(nullptr));

  struct QuizRunner {
    const Dictionary &dict;
    std::size_t count;
    std::size_t current;
    QuizRunner(const Dictionary &d, std::size_t c) : dict(d), count(c), current(0) {}
    void run() {
      if (current >= count) return;
      std::pair<std::string, std::string> q = dict.getRandomPair();
      std::cout << "Translate: " << q.first << '\n';
      std::string userAnswer;
      std::getline(std::cin >> std::ws, userAnswer);
      if (userAnswer == q.second) {
        std::cout << "Correct!" << '\n';
      } else {
        std::cout << "Wrong. Correct: " << q.second << '\n';
      }
      ++current;
      run();
    }
  };

  QuizRunner qr(dict, count);
  qr.run();
}

void CommandHandler::exportDict(const Dictionary &dict, const std::string &filename) const
{
  std::ofstream out(filename);
  if (!out) {
    std::cout << "CANNOT_WRITE_FILE" << '\n';
    return;
  }
  std::vector<std::pair<std::string, std::string> > all = dict.getAllWords();

  struct ExportPrinter {
    std::ofstream &outStream;
    std::vector<std::pair<std::string, std::string> >::const_iterator current;
    std::vector<std::pair<std::string, std::string> >::const_iterator end;
    ExportPrinter(std::ofstream &out,
                  std::vector<std::pair<std::string, std::string> >::const_iterator b,
                  std::vector<std::pair<std::string, std::string> >::const_iterator e)
      : outStream(out), current(b), end(e) {}
    void printAll() {
      if (current == end) return;
      outStream << current->first << ": " << current->second << '\n';
      ++current;
      printAll();
    }
  };

  ExportPrinter printer(out, all.begin(), all.end());
  printer.printAll();
}

void CommandHandler::importDict(Dictionary &dict, const std::string &filename)
{
  std::ifstream in(filename);
  if (!in) {
    std::cout << "FILE_NOT_FOUND" << '\n';
    return;
  }
  std::string line;

  struct ImportReader {
    std::ifstream &inStream;
    Dictionary &dictRef;
    ImportReader(std::ifstream &in, Dictionary &d) : inStream(in), dictRef(d) {}
    void readAll() {
      std::string line;
      if (!std::getline(inStream, line)) return;
      std::size_t pos = line.find(": ");
      if (pos != std::string::npos) {
        std::string word = line.substr(0, pos);
        std::string translation = line.substr(pos + 2);
        dictRef.addWord(word, translation);
      }
      readAll();
    }
  };

  ImportReader reader(in, dict);
  reader.readAll();
}

void CommandHandler::compareDicts(const Dictionary &dict1, const Dictionary &dict2) const
{
  std::size_t same = 0;
  std::size_t only1 = 0;
  std::size_t only2 = 0;

  std::vector<std::pair<std::string, std::string> > all1 = dict1.getAllWords();

  struct Compare1 {
    const std::vector<std::pair<std::string, std::string> > &all;
    std::size_t &sameRef;
    std::size_t &only1Ref;
    std::size_t index;
    const Dictionary &dict2Ref;
    Compare1(const std::vector<std::pair<std::string, std::string> > &a,
             std::size_t &s, std::size_t &o1,
             const Dictionary &d2)
      : all(a), sameRef(s), only1Ref(o1), index(0), dict2Ref(d2) {}
    void process() {
      if (index >= all.size()) return;
      std::string other;
      if (dict2Ref.findWord(all[index].first, other)) {
        if (other == all[index].second) {
          ++sameRef;
        }
      } else {
        ++only1Ref;
      }
      ++index;
      process();
    }
  };

  Compare1 cmp1(all1, same, only1, dict2);
  cmp1.process();

  std::vector<std::pair<std::string, std::string> > all2 = dict2.getAllWords();

  struct Compare2 {
    const std::vector<std::pair<std::string, std::string> > &all;
    std::size_t &only2Ref;
    std::size_t index;
    const Dictionary &dict1Ref;
    Compare2(const std::vector<std::pair<std::string, std::string> > &a,
             std::size_t &o2,
             const Dictionary &d1)
      : all(a), only2Ref(o2), index(0), dict1Ref(d1) {}
    void process() {
      if (index >= all.size()) return;
      std::string other;
      if (!dict1Ref.findWord(all[index].first, other)) {
        ++only2Ref;
      }
      ++index;
      process();
    }
  };

  Compare2 cmp2(all2, only2, dict1);
  cmp2.process();

  std::cout << "Same: " << same << '\n';
  std::cout << "Only in first: " << only1 << '\n';
  std::cout << "Only in second: " << only2 << '\n';
}
