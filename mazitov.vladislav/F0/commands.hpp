#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "dataset.hpp"
#include <functional>

namespace mazitov
{
  void getCmds(std::map< std::string, std::function< void(DataSetManager&, std::istream&, std::ostream&) > > &);
  void createCommand(DataSetManager &, std::istream &, std::ostream &);
  void compressCommand(DataSetManager &, std::istream &, std::ostream &);
  void decompressCommand(DataSetManager &, std::istream &, std::ostream &);
  void degreeCommand(DataSetManager &, std::istream &, std::ostream &);
  void showOriginalCommand(DataSetManager &, std::istream &, std::ostream &);
  void showCompressedCommand(DataSetManager &, std::istream &, std::ostream &);
  void deleteCommand(DataSetManager &, std::istream &, std::ostream &);
  void mergeCommand(DataSetManager &, std::istream &, std::ostream &);
  void subtractCommand(DataSetManager &, std::istream &, std::ostream &);
  void compareDegreesCommand(DataSetManager &, std::istream &, std::ostream &);
  void showCodeCommand(DataSetManager &, std::istream &, std::ostream &);
  void loadTextCommand(DataSetManager &, std::istream &, std::ostream &);
  void compressedToBinCommand(DataSetManager &, std::istream &, std::ostream &);
  void loadCompressedCommand(DataSetManager &, std::istream &, std::ostream &);
}

#endif
