#ifndef SHANNON_FANO_H
#define SHANNON_FANO_H

#include <cmath>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Symbol.h"

namespace voronina
{
  class ShannonFanoTable
  {
  public:
    void generateShannonFanoCodes(const std::string& text, const std::string originFile = "");
    int encode(const std::string& text, std::string& destination) const;
    std::string decode(const std::string& text, int amountOfSignificantBitsInLastByte) const;
    double calculateEntropy();
    const std::vector< Symbol >& symbols() const;
    int size() const;
    const std::string& originFile() const;

    friend std::ostream& operator<<(std::ostream& out, const ShannonFanoTable& table);

  private:
    using SymbIter = std::vector< Symbol >::iterator;

    std::string originFile_;
    std::vector< Symbol > symbols_;
    std::unordered_map< char, Symbol > symbolMap_;
    std::unordered_map< std::string, Symbol > codeMap_;

    void initializeSymbolFrequencies(const std::string& text);
    void shannonFanoRecursion(const SymbIter& begin, const SymbIter& end);
  };
}

#endif
