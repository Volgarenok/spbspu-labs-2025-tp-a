#ifndef ALPHABET_HPP
#define ALPHABET_HPP

#include <iosfwd>
#include "Dictionary.hpp"

namespace holodilov
{
  class Alphabet
  {
  public:
    explicit Alphabet(const Dictionary& dictionary);

    Alphabet(std::istream& in);

    bool operator==(const Alphabet& rhs) const;

    friend std::ostream& operator<<(std::ostream& out, const Alphabet& alphabet);

  private:
    std::string alphabet_;
  };

  std::ostream& operator<<(std::ostream& out, const Alphabet& alphabet);
}

#endif
