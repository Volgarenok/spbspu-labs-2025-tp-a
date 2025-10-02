#ifndef DICT_IMPORT_PARSERS_HPP
#define DICT_IMPORT_PARSERS_HPP

#include <vector>
#include <string>

#include "HashTable.hpp"

namespace crossref
{

  void processDictLine(HashTable &table, const std::string &line);

}

#endif
