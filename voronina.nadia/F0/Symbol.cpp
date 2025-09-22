#include "Symbol.h"
#include <iomanip>
#include <iostream>
#include "IOFmtguard.h"

namespace voronina
{
  bool FrequencyComparator::operator()(Symbol const& a, Symbol const& b) const
  {
    return a.frequency > b.frequency;
  }

  Symbol SymbolCreator::operator()(char symbol) const
  {
    return Symbol{ symbol, "", 0.0 };
  }

  double FrequencyAccumulator::operator()(double sum, Symbol const& symbol) const
  {
    return sum + symbol.frequency;
  }

  bool CodeLengthComparator::operator()(Symbol const& a, Symbol const& b) const
  {
    return a.code.length() < b.code.length();
  }

  bool operator==(Symbol const& symb, char c)
  {
    return symb.symbol == c;
  }

  bool operator==(char c, Symbol const& symb)
  {
    return symb.symbol == c;
  }

  std::ostream& operator<<(std::ostream& out, Symbol const& symbol)
  {
    iofmtguard guard{ out };

    std::string symbolRepresentation;
    switch (symbol.symbol)
    {
      case ' ':
        symbolRepresentation = "' '";
        break;
      case '\n':
        symbolRepresentation = "\\n";
        break;
      default:
        symbolRepresentation = std::string(1, symbol.symbol);
        break;
    }

    out << std::fixed << std::setprecision(2);
    out << std::left << std::setw(10) << symbolRepresentation << std::setw(10) << symbol.frequency
        << symbol.code;

    return out;
  }

  Symbol& appendZero(Symbol& symb)
  {
    symb.code += "0";
    return symb;
  }

  Symbol& appendOne(Symbol& symb)
  {
    symb.code += "1";
    return symb;
  }
}
