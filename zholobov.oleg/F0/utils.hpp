#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include <vector>

#include "commands.hpp"

namespace zholobov {

  std::vector< std::string > splitTokens(const std::string& line);

}

std::istream& operator>>(std::istream& in, zholobov::Words& words);
std::ostream& operator<<(std::ostream& out, const zholobov::Words& words);
std::ostream& operator<<(std::ostream& out, const zholobov::Dictionary& dict);

#endif
