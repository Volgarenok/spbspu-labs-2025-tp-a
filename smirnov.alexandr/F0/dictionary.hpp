#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP
#include <unordered_map>
#include <vector>
#include <string>

namespace smirnov
{
  using Dict = std::unordered_map< std::string, std::vector< std::string > >;
  using Dicts = std::unordered_map< std::string, Dict >;
}
#endif
