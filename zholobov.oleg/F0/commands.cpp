#include "commands.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>

#include "utils.hpp"

void zholobov::printHelp(std::ostream& out)
{
  out << "Commands:\n";
  out << "dict-create <dict>\n";
  out << "dict-remove <dict>\n";
  out << "dict-import <dict> <file>\n";
  out << "dict-export <dict> <file> [overwrite]\n";
  out << "dict-count\n";
  out << "dict-list\n";
  out << "dict-print-words <dict>\n";
  out << "dict-print-translations [<dict>]\n";
  out << "dict-clear [<dict>]\n";
  out << "add-word <dict> <word> <translation>\n";
  out << "remove-word <dict> <word>\n";
  out << "count-words <dict>\n";
  out << "add-translation <dict> <word> <translation>\n";
  out << "remove-translation <dict> <word> <translation>\n";
  out << "change-word <dict> <word> <new_word>\n";
  out << "translate-word <word>\n";
  out << "union <new_dict> <dict-1> [<dict-N> ...]\n";
  out << "intersect <new_dict> <dict-1> [<dict-N> ...]\n";
  out << "rare <N> <new_dict> <dict-1> [<dict-K> ...]\n";
}

void zholobov::cmdDictCreate(Dictionaries& dictionaries, const std::vector< std::string >& args)
{
  if (args.size() != 2) {
    throw InvalidParams();
  }
  const std::string& name = args[1];
  if (dictionaries.count(name)) {
    std::cout << "<DICTIONARY ALREADY EXISTS>\n";
  } else {
    dictionaries[name] = Dictionary();
  }
}

void zholobov::cmdDictRemove(Dictionaries& dictionaries, const std::vector< std::string >& args)
{
  if (args.size() != 2) {
    throw InvalidParams();
  }
  const std::string& name = args[1];
  if (!dictionaries.count(name)) {
    std::cout << "<INVALID DICTIONARY>\n";
  } else {
    dictionaries.erase(name);
  }
}

void zholobov::cmdDictImport(Dictionaries& dictionaries, const std::vector< std::string >& args)
{
  if (args.size() != 3) {
    throw InvalidParams();
  }
  const std::string& dictName = args[1];
  const std::string& fileName = args[2];

  Dictionary& dict = dictionaries[dictName];

  std::ifstream fin(fileName);
  if (!fin) {
    std::cout << "<0 TRANSLATIONS IMPORTED>.\n";
    return;
  }

  int imported = 0;
  int dropped = 0;
  Words tokens;
  while (fin >> tokens) {
    if (tokens.empty()) {
      continue;
    }

    Word eng = tokens.front();
    tokens.pop_front();
    if (tokens.empty()) {
      ++dropped;
      continue;
    }

    Words& entry = dict[eng];
    for (auto it = tokens.cbegin(); it != tokens.cend(); ++it) {
      if (std::find(entry.cbegin(), entry.cend(), *it) == entry.cend()) {
        entry.push_back(*it);
      }
    }
    ++imported;
  }

  std::cout << "<" << imported << " TRANSLATIONS IMPORTED>.";
  if (dropped != 0) {
    std::cout << " [" << dropped << " - DROPPED]";
  }
  std::cout << '\n';
}

void zholobov::cmdDictExport(Dictionaries& dictionaries, const std::vector< std::string >& args)
{
  if (!(args.size() == 3 || args.size() == 4)) {
    throw InvalidParams();
  }
  const std::string& dictName = args[1];
  const std::string& fileName = args[2];
  bool overwrite = ((args.size() == 4) && (args[3] == "overwrite"));

  auto it = dictionaries.find(dictName);
  if (it == dictionaries.end()) {
    std::cout << "<INVALID DICTIONARY>\n";
    return;
  }

  std::ifstream check(fileName);
  if (check && !overwrite) {
    std::cout << "FILE EXISTS\n";
    return;
  }

  std::ofstream fout(fileName, std::ios::trunc);
  if (!fout) {
    return;
  }
  fout << it->second;
}

void zholobov::cmdDictCount(Dictionaries& dictionaries, const std::vector< std::string >& args)
{
  if (args.size() != 1) {
    throw InvalidParams();
  }
  std::cout << dictionaries.size() << '\n';
}

void zholobov::cmdDictList(Dictionaries& dictionaries, const std::vector< std::string >& args)
{
  if (args.size() != 1) {
    throw InvalidParams();
  }
  for (const auto& dict: dictionaries) {
    std::cout << dict.first << '\n';
  }
}

void zholobov::cmdDictPrintWords(Dictionaries& dictionaries, const std::vector< std::string >& args)
{
  if (args.size() != 2) {
    throw InvalidParams();
  }
  const std::string& name = args[1];
  auto it = dictionaries.find(name);
  if (it == dictionaries.end()) {
    std::cout << "<INVALID DICTIONARY>\n";
    return;
  }
  for (const auto& record: it->second) {
    std::cout << record.first << '\n';
  }
}

void zholobov::cmdDictPrintTranslations(Dictionaries& dictionaries, const std::vector< std::string >& args)
{
  if (!(args.size() == 1 || args.size() == 2)) {
    throw InvalidParams();
  }
  if (args.size() == 2) {
    auto it = dictionaries.find(args[1]);
    if (it == dictionaries.end()) {
      std::cout << "<INVALID DICTIONARY>\n";
      return;
    }
    std::cout << it->second;
  } else {
    if (dictionaries.empty()) {
      std::cout << "<NO DICTIONARIES>\n";
      return;
    }
    for (const auto& dict: dictionaries) {
      std::cout << dict.second;
    }
  }
}
