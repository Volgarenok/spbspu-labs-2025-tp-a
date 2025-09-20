#include "commands.hpp"

#include <iostream>

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
