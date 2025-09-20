#ifndef COMMANDS_H
#define COMMANDS_H

#include <iostream>

#include "ShannonFano.h"

namespace voronina
{
  using FanoTablesVec = std::vector< ShannonFanoTable >;
  void getCodes(FanoTablesVec& vectorOfTables, std::istream& in, std::ostream& out);
  void encode(FanoTablesVec& vectorOfTables, std::istream& in, std::ostream& out);
  void decode(const FanoTablesVec& vectorOfTables, std::istream& in, std::ostream& out);
  void printInfo(const FanoTablesVec& vectorOfTables, std::istream& in, std::ostream& out);
  void compare(const FanoTablesVec& vectorOfTables, std::istream& in, std::ostream& out);
  void entropy(std::istream& in, std::ostream& out);
  void origin(const FanoTablesVec& vectorOfTables, std::istream& in, std::ostream& out);
  void listCodes(const FanoTablesVec& vectorOfTables, std::istream& in, std::ostream& out);
  void codesInfo(const FanoTablesVec& vectorOfTables, std::istream& in, std::ostream& out);
  void ableCoding(const FanoTablesVec& vectorOfTables, std::istream& in, std::ostream& out);
  void visualize(const FanoTablesVec& vectorOfTables, std::istream& in, std::ostream& out);
  void definiteEncode(const FanoTablesVec& vectorOfTables, std::istream& in, std::ostream& out);
}

#endif
