#ifndef ALPHABET_HPP
#define ALPHABET_HPP

#include <set>
#include <iostream>
#include "Dictionary.hpp"

namespace holodilov
{
  class Alphabet
  {
  public:
    void load(const Dictionary& dictionary);

    bool isLoaded() const;

    bool operator==(const Alphabet& rhs) const;

    friend std::ostream& operator<<(std::ostream& out, const Alphabet& alphabet);
    friend std::istream& operator>>(std::istream& in, Alphabet& alphabet);

  private:
    std::set< char > alphabet_;
    bool isLoaded_ = false;
  };

  std::ostream& operator<<(std::ostream& out, const Alphabet& alphabet);

  std::istream& operator>>(std::istream& in, Alphabet& alphabet);
}

#endif
