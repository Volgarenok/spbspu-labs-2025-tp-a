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
  T staticCast(const T& value);

  struct BitExtractor
  {
    int j = 7;
    unsigned char byte;

    BitExtractor(unsigned char b);
    char operator()();
  };

  std::string byteToString(unsigned char byte);

  struct ByteGenerator
  {
    const std::string& bitMask;
    int bitMaskLength;
    int i = 0;

    ByteGenerator(const std::string& bitMask, int bitMaskLength);
    std::string operator()();
  };

  struct BitToDecimalAccumulator
  {
    int degree = 7;
    int operator()(int sum, char bit);
  };

  struct ByteMaskToCharTransformer
  {
    char operator()(const std::string& byte) const;
  };

  struct SymbolToSymbolMapEntry
  {
    std::pair< char, Symbol > operator()(const Symbol& symbol) const;
  };

  struct SymbolToCodeMapEntry
  {
    std::pair< std::string, Symbol > operator()(const Symbol& symbol) const;
  };

  struct SymbolToCodeTransformer
  {
    using SymbolMap = std::unordered_map< char, Symbol >;
    const SymbolMap& symbolMap;

    SymbolToCodeTransformer(const SymbolMap& symbolMap);
    std::string operator()(const std::string& acc, char symbol) const;
  };

  struct FrequencySetter
  {
    const std::string& text;
    FrequencySetter(const std::string& text);
    Symbol operator()(Symbol symbol) const;
  };

  struct ByteToCharFunctor
  {
    ByteGenerator& generator;
    ByteMaskToCharTransformer& transformer;

    ByteToCharFunctor(ByteGenerator& gen, ByteMaskToCharTransformer& trans);

    char operator()();
  };

  struct BitMaskExtractor
  {
    const std::string& bitMask;
    int& i;
    BitMaskExtractor(const std::string& bitMask, int& i);
    char operator()();
  };

  int encodeBitMaskToDestination(const std::string& bitMask, std::string& destination);

  double logFrequencyAccumulator(double sum, const Symbol& symb);

  template < class T >
  T staticCast(const T& value)
  {
    return value;
  }

  BitExtractor::BitExtractor(unsigned char b):
    byte(b)
  {}

  char BitExtractor::operator()()
  {
    return ((byte >> j--) & 1) + '0';
  }

  std::string byteToString(unsigned char byte)
  {
    std::string result;
    BitExtractor extractor(byte);
    std::generate_n(std::back_inserter(result), 8, extractor);
    return result;
  }

  ByteGenerator::ByteGenerator(const std::string& bitMask, int bitMaskLength):
    bitMask(bitMask), bitMaskLength(bitMaskLength)
  {}

  std::string ByteGenerator::operator()()
  {
    std::string byte;
    int bitsToExtract = std::min(8, bitMaskLength - i);
    auto extractor = BitMaskExtractor(bitMask, i);
    std::generate_n(std::back_inserter(byte), bitsToExtract, extractor);
    return byte;
  }

  int BitToDecimalAccumulator::operator()(int sum, char bit)
  {
    sum += (bit - '0') << degree--;
    return sum;
  }

  char ByteMaskToCharTransformer::operator()(const std::string& byte) const
  {
    auto accumulator = BitToDecimalAccumulator{};
    return std::accumulate(byte.begin(), byte.end(), 0, accumulator);
  }

  std::pair< char, Symbol > SymbolToSymbolMapEntry::operator()(const Symbol& symbol) const
  {
    return { symbol.symbol, symbol };
  }

  std::pair< std::string, Symbol > SymbolToCodeMapEntry::operator()(const Symbol& symbol) const
  {
    return { symbol.code, symbol };
  }

  SymbolToCodeTransformer::SymbolToCodeTransformer(const SymbolMap& symbolMap):
    symbolMap(symbolMap)
  {}

  std::string SymbolToCodeTransformer::operator()(const std::string& acc, char symbol) const
  {
    auto it = symbolMap.find(symbol);
    if (it != symbolMap.end())
    {
      return acc + it->second.code;
    }
    return acc;
  }

  FrequencySetter::FrequencySetter(const std::string& text):
    text(text)
  {}

  Symbol FrequencySetter::operator()(Symbol symbol) const
  {
    auto size = static_cast< double >(text.size());
    auto count = std::count(text.begin(), text.end(), symbol.symbol);
    symbol.frequency = count / size;
    return symbol;
  }

  ByteToCharFunctor::ByteToCharFunctor(ByteGenerator& gen, ByteMaskToCharTransformer& trans):
    generator(gen), transformer(trans)
  {}

  char ByteToCharFunctor::operator()()
  {
    return transformer(generator());
  }

  BitMaskExtractor::BitMaskExtractor(const std::string& bitMask, int& i):
    bitMask(bitMask), i(i)
  {}

  char BitMaskExtractor::operator()()
  {
    return bitMask[i++];
  }

  int encodeBitMaskToDestination(const std::string& bitMask, std::string& destination)
  {
    int bitMaskLength = bitMask.length();
    int fullBytes = bitMaskLength / 8;
    int remainingBits = bitMaskLength % 8;

    ByteGenerator generator(bitMask, bitMaskLength);
    ByteMaskToCharTransformer transformer;
    ByteToCharFunctor byteToCharFunctor(generator, transformer);
    std::generate_n(std::back_inserter(destination), fullBytes, byteToCharFunctor);
    if (remainingBits)
    {
      std::string lastByte = bitMask.substr(fullBytes * 8, remainingBits);
      lastByte.append(8 - remainingBits, '0');
      destination += transformer(lastByte);
    }
    return remainingBits;
  }

  double logFrequencyAccumulator(double sum, const Symbol& symb)
  {
    if (symb.frequency > 0)
    {
      return sum + symb.frequency * std::log2(symb.frequency);
    }
    return sum;
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

    auto inserter = std::back_inserter(symbols_);
    std::transform(sortedText.begin(), uniqueEnd, inserter, SymbolCreator{});
    auto begin = symbols_.begin();
    auto end = symbols_.end();
    std::transform(begin, end, begin, FrequencySetter(text));
    std::sort(begin, end, FrequencyComparator{});
  }

  void ShannonFanoTable::shannonFanoRecursion(const SymbIter& begin, const SymbIter& end)
  {
    if (begin == end)
    {
      return;
    }

    double total = std::accumulate(begin, end + 1, 0.0, FrequencyAccumulator{});

    std::vector< double > frequencies(std::distance(begin, end) + 1);
    auto transformer = std::bind(&Symbol::frequency, std::placeholders::_1);
    std::transform(begin, end + 1, frequencies.begin(), transformer);

    std::vector< double > prefixSums(frequencies.size());
    std::partial_sum(frequencies.begin(), frequencies.end(), prefixSums.begin());

    using namespace std::placeholders;
    auto greaterThanTotal = std::bind(std::greater_equal< double >{}, _1, total / 2);
    auto sumsBegin = prefixSums.begin();
    auto sumsEnd = prefixSums.end();
    auto prefixSumsSplitIterator = std::find_if(sumsBegin, sumsEnd, greaterThanTotal);
    auto splitIterator = begin + std::distance(prefixSums.begin(), prefixSumsSplitIterator);
    std::transform(begin, splitIterator + 1, begin, appendZero);
    std::transform(splitIterator + 1, end + 1, splitIterator + 1, appendOne);

    shannonFanoRecursion(begin, splitIterator);
    shannonFanoRecursion(splitIterator + 1, end);
  }

  void ShannonFanoTable::generateShannonFanoCodes(const std::string& text,
                                                  const std::string originFile)
  {
    if (text.empty())
    {
      static auto errMessage = "Невозможно создать кодировку Шеннона-Фано из пустой строки";
      throw std::invalid_argument(errMessage);
    }

    originFile_ = originFile;
    initializeSymbolFrequencies(text);
    shannonFanoRecursion(symbols_.begin(), symbols_.end() - 1);

    auto symbolInserter = std::inserter(symbolMap_, symbolMap_.end());
    auto transformer = SymbolToSymbolMapEntry();
    std::transform(symbols_.begin(), symbols_.end(), symbolInserter, transformer);

    auto codeMapInserter = std::inserter(codeMap_, codeMap_.end());
    std::transform(symbols_.begin(), symbols_.end(), codeMapInserter, SymbolToCodeMapEntry());
  }

  int ShannonFanoTable::encode(const std::string& text, std::string& destination) const
  {
    if (symbolMap_.empty())
    {
      throw std::logic_error(
          "Contract violation: symbolMap_ must be initialized before encoding. "
          "Call generateShannonFanoCodes() first.");
    }
    auto transformer = SymbolToCodeTransformer(symbolMap_);
    auto begin = text.begin();
    auto end = text.end();
    std::string bitMask = std::accumulate(begin, end, std::string{}, transformer);
    return encodeBitMaskToDestination(bitMask, destination);
  }

  std::string ShannonFanoTable::decode(const std::string& text,
                                       int significantBitsInLastByte) const
  {
    if (symbolMap_.empty())
    {
      auto errorMessage = "Contract violation: symbolMap_ must be initialized before decoding";
      throw std::logic_error(errorMessage);
    }

    if (significantBitsInLastByte > 7)
    {
      throw std::invalid_argument("Количество значимых битов в последнем байте "
                                  "должно быть в диапазоне от 0 до 7");
    }

    std::vector< unsigned char > bytes;
    bytes.reserve(text.size());

    using namespace std::placeholders;
    auto toUnsignedChar = std::bind(staticCast< unsigned char >, _1);
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
    auto accumulator = logFrequencyAccumulator;
    return -1 * std::accumulate(symbols_.begin(), symbols_.end(), 0.0, accumulator);
  }

  std::ostream& operator<<(std::ostream& out, const ShannonFanoTable& table)
  {
    iofmtguard ofmtguard(out);
    std::cout << std::left << std::setw(10) << "Symbol" << std::setw(10);
    std::cout << "Frequency" << "Code\n";
    auto begin = table.symbols_.begin();
    using OIter = std::ostream_iterator<Symbol>;
    std::copy(begin, table.symbols_.end(), OIter(out, "\n"));
    return out;
  }
}
