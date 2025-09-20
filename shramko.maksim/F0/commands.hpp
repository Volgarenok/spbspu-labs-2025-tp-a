#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <functional>
#include <map>
#include <string>
#include <vector>
#include <ostream>
#include "dictionary_manager.hpp"

using CommandFunction = std::function< void(const std::vector< std::string >&, DictionaryManager&, std::ostream&) >;

std::map< std::string, CommandFunction > createCommandMap();

void create(const std::vector< std::string >& args, DictionaryManager& dm, std::ostream& os);
void add(const std::vector< std::string >& args, DictionaryManager& dm, std::ostream& os);
void increment(const std::vector< std::string >& args, DictionaryManager& dm, std::ostream& os);
void search(const std::vector< std::string >& args, DictionaryManager& dm, std::ostream& os);
void delete_(const std::vector< std::string >& args, DictionaryManager& dm, std::ostream& os);
void dump(const std::vector< std::string >& args, DictionaryManager& dm, std::ostream& os);
void top(const std::vector< std::string >& args, DictionaryManager& dm, std::ostream& os);
void bot(const std::vector< std::string >& args, DictionaryManager& dm, std::ostream& os);
void minfreq(const std::vector< std::string >& args, DictionaryManager& dm, std::ostream& os);
void maxfreq(const std::vector< std::string >& args, DictionaryManager& dm, std::ostream& os);
void median(const std::vector< std::string >& args, DictionaryManager& dm, std::ostream& os);

#endif
