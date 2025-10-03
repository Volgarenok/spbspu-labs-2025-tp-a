#ifndef SYMBOL_H
#define SYMBOL_H

#include <iosfwd>
#include <string>

namespace voronina
{
  struct Symbol
  {
    char symbol;
    std::string code;
    double frequency;
  };

  struct FrequencyComparator
  {
    bool operator()(Symbol const& a, Symbol const& b) const;
  };

  struct SymbolCreator
  {
    Symbol operator()(char symbol) const;
  };

  struct FrequencyAccumulator
  {
    double operator()(double sum, Symbol const& symbol) const;
  };

  struct CodeLengthComparator
  {
    bool operator()(Symbol const& a, Symbol const& b) const;
  };

  bool operator==(Symbol const& symb, char c);
  bool operator==(char c, Symbol const& symb);
  std::ostream& operator<<(std::ostream& out, Symbol const& symb);
  Symbol& appendZero(Symbol& symb);
  Symbol& appendOne(Symbol& symb);
}

#endif
