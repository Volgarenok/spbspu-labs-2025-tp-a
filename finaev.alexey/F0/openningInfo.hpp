#ifndef OPENNINGINFO
#define OPENNINGINFO
#include <vector>
#include <string>

namespace finaev
{
  struct DebutMoves
  {
    std::vector< std::string > moves;
    bool containsSequence(const std::vector< std::string >& sequence) const;
  };

  std::istream& operator>>(std::istream&, DebutMoves&);

  struct OpenningInfo
  {
    DebutMoves moves;
    std::string name;
    std::string description;
  };
}

#endif
