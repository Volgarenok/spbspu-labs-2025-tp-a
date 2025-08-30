#include <algorithm>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iterator>
#include <numeric>
#include <vector>

#include "IOFmtguard.h"
#include "ShannonFano.h"

namespace
{
  using namespace voronina;

  template < class T >
  T staticCast(const T& value)
  {
    return static_cast< T >(value);
  }

  struct BitExtractor
  {
    int j = 7;
    unsigned char byte;

    BitExtractor(unsigned char b):
      byte(b)
    {}

    char operator()();
  };

  char BitExtractor::operator()()
  {
    char bit = ((byte >> j) & 1) ? '1' : '0';
    if (--j < 0)
    {
      j = 7;
    }
    return bit;
  }

  std::string byteToString(unsigned char byte)
  {
    std::string acc;
    std::generate_n(std::back_inserter(acc), 8, BitExtractor(byte));
    return acc;
  }

  struct ByteGenerator
  {
    const std::string& bitMask;
    int bitMaskLength;
    int i = 0;

    ByteGenerator(const std::string& bitMask, int bitMaskLength):
      bitMask(bitMask), bitMaskLength(bitMaskLength)
    {}

    std::string operator()()
    {
      std::string byte = bitMask.substr(i, 8);
      i += 8;
      return byte.append(8 - byte.length(), '0');
    }
  };

  struct BitToDecimalConverter
  {
    int degree = 7;

    int operator()(int sum, char bit)
    {
      if (degree < 0)
      {
        throw std::out_of_range("Degree must be in the range 0-7. Developer might have made a mistake.");
      }
      if (bit == '1')
      {
        sum += (1 << degree);
      }
      --degree;
      return sum;
    }
  };

  struct ByteMaskToCharTransformer
  {
    char operator()(const std::string& byte) const
    {
      return static_cast< char >(std::accumulate(byte.begin(), byte.end(), 0, BitToDecimalConverter()));
    }
  };

  struct SplitPointFinder
  {
    double total;
    const std::vector< double >& prefixSums;
    std::size_t idx;

    SplitPointFinder(double total, const std::vector< double >& prefixSums):
      total(total), prefixSums(prefixSums), idx(0)
    {}

    bool operator()(const Symbol& symb)
    {
      double leftSumFrequency = prefixSums[idx++];
      return leftSumFrequency >= total / 2;
    }
  };

  struct SymbolToSymbolMapEntry
  {
    std::pair< char, Symbol > operator()(const Symbol& symbol) const
    {
      return std::make_pair(symbol.symbol, symbol);
    }
  };

  struct SymbolToCodeMapEntry
  {
    std::pair< std::string, Symbol > operator()(const Symbol& symbol) const
    {
      return std::make_pair(symbol.code, symbol);
    }
  };

  struct SymbolToCodeTransformer
  {
    const std::unordered_map< char, Symbol >& symbolMap;

    SymbolToCodeTransformer(const std::unordered_map< char, Symbol >& symbolMap):
      symbolMap(symbolMap)
    {}

    std::string operator()(const std::string& acc, char symbol) const
    {
      auto it = symbolMap.find(symbol);
      if (it == symbolMap.end())
      {
        throw std::invalid_argument("Символ '" + std::string(1, symbol) + "' не может быть закодирован.");
      }
      return acc + it->second.code;
    }
  };

  struct FrequencySetter
  {
    const std::string& text;
    FrequencySetter(const std::string& text):
      text(text)
    {}

    Symbol operator()(Symbol symbol) const
    {
      symbol.frequency = static_cast< double >(std::count(text.begin(), text.end(), symbol.symbol)) / text.length();
      return symbol;
    }
  };

  int encodeBitMaskToDestination(const std::string& bitMask, std::string& destination)
  {
    int decimalSum = 0;
    int bitMaskLength = bitMask.length();
    int degree = 7;
    std::vector< std::string > bytes;

    std::generate_n(std::back_inserter(bytes), (bitMaskLength + 7) / 8, ByteGenerator(bitMask, bitMaskLength));
    std::transform(bytes.begin(), bytes.end(), std::back_inserter(destination), ByteMaskToCharTransformer());
    return bitMaskLength % 8;
  }

  double logFrequencyAccumulator(double sum, const Symbol& symb)
  {
    return sum + symb.frequency * std::log2(symb.frequency);
  }
}

namespace voronina
{
  const std::vector< Symbol >& ShannonFanoTable::symbols() const
  {
    return symbols_;
  }

  int ShannonFanoTable::size() const
  {
    return symbols_.size();
  }

  const std::string& ShannonFanoTable::originFile() const
  {
    return originFile_;
  }

  void ShannonFanoTable::initializeSymbolFrequencies(const std::string& text)
  {
    std::string sortedText = text;
    std::sort(sortedText.begin(), sortedText.end());
    auto uniqueEnd = std::unique(sortedText.begin(), sortedText.end());
    symbols_.clear();

    std::transform(sortedText.begin(), uniqueEnd, std::back_inserter(symbols_), SymbolCreator{});
    std::transform(symbols_.begin(), symbols_.end(), symbols_.begin(), FrequencySetter(text));
    std::sort(symbols_.begin(), symbols_.end(), FrequencyComparator{});
  }

  void ShannonFanoTable::shannonFanoRecursion(const std::vector< Symbol >::iterator& begin,
                                              const std::vector< Symbol >::iterator& end)
  {
    if (begin == end)
    {
      return;
    }

    double total = std::accumulate(begin, end + 1, 0.0, FrequencyAccumulator{});
    double leftSumFrequency = begin->frequency;

    std::vector< double > frequencies(std::distance(begin, end) + 1);
    std::transform(begin, end + 1, frequencies.begin(), std::bind(&Symbol::frequency, std::placeholders::_1));

    std::vector< double > prefixSums(frequencies.size());
    std::partial_sum(frequencies.begin(), frequencies.end(), prefixSums.begin());

    auto splitIterator = std::find_if(begin, end, SplitPointFinder(total, prefixSums));
    std::transform(begin, splitIterator + 1, begin, appendZero);
    std::transform(splitIterator + 1, end + 1, splitIterator + 1, appendOne);

    shannonFanoRecursion(begin, splitIterator);
    shannonFanoRecursion(splitIterator + 1, end);
  }

  void ShannonFanoTable::generateShannonFanoCodes(const std::string& text, const std::string originFile)
  {
    if (text.empty())
    {
      throw std::invalid_argument("Невозможно создать кодировку Шеннона-Фано из пустой строки.");
    }

    originFile_ = originFile;
    initializeSymbolFrequencies(text);
    shannonFanoRecursion(symbols_.begin(), symbols_.end() - 1);

    auto symbolInserter = std::inserter(symbolMap_, symbolMap_.end());
    std::transform(symbols_.begin(), symbols_.end(), symbolInserter, SymbolToSymbolMapEntry());

    auto codeMapInserter = std::inserter(codeMap_, codeMap_.end());
    std::transform(symbols_.begin(), symbols_.end(), codeMapInserter, SymbolToCodeMapEntry());
  }

  int ShannonFanoTable::encode(const std::string& text, std::string& destination) const
  {
    if (symbolMap_.empty())
    {
      throw std::logic_error(
          "Contract violation: symbolMap_ must be initialized before encoding. Call generateShannonFanoCodes() first.");
    }
    std::string bitMask = std::accumulate(text.begin(), text.end(), std::string{}, SymbolToCodeTransformer(symbolMap_));
    return encodeBitMaskToDestination(bitMask, destination);
  }

  std::string ShannonFanoTable::decode(const std::string& text, int significantBitsInLastByte) const
  {
    if (symbolMap_.empty())
    {
      auto errorMessage = "Contract violation: symbolMap_ must be initialized before decoding";
      throw std::logic_error(errorMessage);
    }

    if (significantBitsInLastByte > 7)
    {
      throw std::invalid_argument("Количество значимых битов в последнем байте должно быть в диапазоне от 0 до 7");
    }

    std::vector< unsigned char > bytes;
    bytes.reserve(text.size());

    auto toUnsignedChar = std::bind(staticCast< unsigned char >, std::placeholders::_1);
    std::transform(text.begin(), text.end(), std::back_inserter(bytes), toUnsignedChar);

    std::vector< std::string > bitMasks;
    std::transform(bytes.begin(), bytes.end(), std::back_inserter(bitMasks), byteToString);
    std::string bitMask = std::accumulate(bitMasks.begin(), bitMasks.end(), std::string{});

    std::size_t bitMaskLength = bitMask.length();
    if (significantBitsInLastByte != 0)
    {
      bitMaskLength = bitMaskLength - 8 + significantBitsInLastByte;
    }

    std::string buffer;
    std::string destination;
    for (std::size_t i = 0; i < bitMaskLength; ++i)
    {
      buffer += bitMask[i];

      auto it = codeMap_.find(buffer);
      if (it != codeMap_.end())
      {
        destination += it->second.symbol;
        buffer.clear();
      }
    }
    return destination;
  }

  double ShannonFanoTable::calculateEntropy()
  {
    return -1 * std::accumulate(symbols_.begin(), symbols_.end(), 0.0, logFrequencyAccumulator);
  }

  std::ostream& operator<<(std::ostream& out, const ShannonFanoTable& table)
  {
    iofmtguard ofmtguard(out);
    std::cout << std::left << std::setw(10) << "Symbol" << std::setw(10) << "Frequency" << "Code\n";
    std::copy(std::begin(table.symbols_), std::end(table.symbols_), std::ostream_iterator< Symbol >(out, "\n"));
    return out;
  }
}
