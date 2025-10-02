#ifndef SEQUENCE_COMMANDS_HPP
#define SEQUENCE_COMMANDS_HPP

#include <ios>
#include <unordered_map>
#include "music.hpp"

namespace aleksandrov
{
  using Sequences = std::unordered_map< std::string, Sequence >;

  void printHelp(std::ostream&);
  void processCommands(std::istream&, std::ostream&, Sequences&);

  void listSeqs(std::ostream&, const Sequences&);
  void newSeq(std::istream&, Sequences&);
  void loadSeq(std::istream&, Sequences&);
  void saveSeq(std::istream&, Sequences&);
  void clearSeq(std::istream&, Sequences&);
  void deleteSeq(std::istream&, Sequences&);
  void cloneSeq(std::istream&, Sequences&);
  void addToSeq(std::istream&, Sequences&);
  void removeFromSeq(std::istream&, Sequences&);
  void mergeSeqs(std::istream&, Sequences&);
  void printSeq(std::istream&, std::ostream&, const Sequences&);
  void elementsSeq(std::istream&, std::ostream&, const Sequences&);
  void notesSeq(std::istream&, std::ostream&, const Sequences&);
  void typeSeq(std::istream&, std::ostream&, const Sequences&);
  void inverseSeq(std::istream&, Sequences&);
}

#endif

