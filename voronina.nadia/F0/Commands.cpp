#include <algorithm>
#include <exception>
#include <functional>
#include <iomanip>
#include <iterator>
#include <ostream>
#include <stdexcept>

#include "Commands.h"
#include "Delimiter.h"
#include "FileUtilities.h"
#include "IOFmtguard.h"
#include "ShannonFano.h"

namespace
{
  using namespace voronina;

  struct CodeInfoHeader
  {};

  std::ostream& operator<<(std::ostream& os, const CodeInfoHeader&)
  {
    iofmtguard ifmtguard(os);
    os << std::left << std::setw(4) << "N";
    os << std::setw(16) << "Symbol Count" << std::setw(20) << "Original File" << '\n';
    return os;
  }

  struct CodeInfo
  {
    std::size_t index;
    std::size_t symbolCount;
    std::string origin;
  };

  std::ostream& operator<<(std::ostream& os, const CodeInfo& info)
  {
    iofmtguard ifmtguard(os);
    os << std::left << std::setw(4) << info.index << std::setw(16) << info.symbolCount << std::setw(20) << info.origin;
    return os;
  }

  struct CodeInfoFunctor
  {
    std::size_t index = 0;
    CodeInfo operator()(const ShannonFanoTable& table)
    {
      return CodeInfo{ index++, static_cast< std::size_t >(table.size()), table.originFile() };
    }
  };

  struct CanEncodeTransformer
  {
    const std::string& text;
    const std::vector< ShannonFanoTable >& vectorOfTables;

    std::string operator()(std::size_t idx);
  };

  std::string CanEncodeTransformer::operator()(std::size_t idx)
  {
    try
    {
      std::string encodedText;
      vectorOfTables[idx].encode(text, encodedText);
      return "да";
    }
    catch (const std::exception& e)
    {
      using namespace std::string_literals;
      return "нет"s + " ("s + e.what() + ")"s;
    }
  }

  struct CanEncodeHeader
  {};

  std::ostream& operator<<(std::ostream& out, const CanEncodeHeader&)
  {
    out << std::left << std::setw(4) << "N" << "Possible?\n";
    return out;
  }

  struct CanEncodePrinter
  {
    std::ostream& out;
    std::size_t idx;
    const std::string& canEncodeText;
  };

  std::ostream& operator<<(std::ostream& out, const CanEncodePrinter& printer)
  {
    out << std::setw(4) << (printer.idx + 1) << printer.canEncodeText;
    return out;
  }

  CanEncodePrinter canEncodePrinterBuilder(std::ostream& out, std::size_t idx, const std::string& canEncodeText)
  {
    return CanEncodePrinter{ out, idx, canEncodeText };
  }

  struct CompressionResult
  {
    std::size_t index;
    bool canEncode;
    double compressionRatio;
    std::string errorMsg;
  };

  struct CompressionFunctor
  {
    const std::vector< ShannonFanoTable >& tables;
    const std::string& text;
    std::streamsize before;
    const std::string& encodedFilename;

    CompressionResult operator()(std::size_t i) const;
  };

  CompressionResult CompressionFunctor::operator()(std::size_t i) const
  {
    try
    {
      std::string encodedText;
      tables[i].encode(text, encodedText);
      writeInFile(encodedFilename, encodedText);
      std::streamsize after = getFileSize(encodedFilename);
      if (before == 0)
      {
        return CompressionResult{ i, false, 0.0, "Ошибка: исходный файл пуст" };
      }
      double ratio = after * 100.0 / before;
      return CompressionResult{ i, true, ratio, "" };
    }
    catch (const std::exception& e)
    {
      return CompressionResult{ i, false, 0.0, e.what() };
    }
  }

  std::ostream& operator<<(std::ostream& os, const CompressionResult& result)
  {
    iofmtguard guard{ os };
    os << std::setw(10) << (result.index + 1);
    if (!result.canEncode)
    {
      os << result.errorMsg;
    }
    else
    {
      os << std::setw(20) << std::to_string(result.compressionRatio) + " %";
    }
    return os;
  }

  struct EncodingAmount
  {
    std::size_t& amount;
    std::size_t maxSize;
  };

  std::istream& operator>>(std::istream& in, EncodingAmount&& encodingAmount)
  {
    iofmtguard ifmtguard(in);
    in >> std::noskipws;

    std::size_t amountOfEncodings = 0;
    in >> DelimiterIO{ ' ' } >> amountOfEncodings;

    if (!in)
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    if (amountOfEncodings == 0 || amountOfEncodings > encodingAmount.maxSize)
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    encodingAmount.amount = amountOfEncodings;
    return in;
  }

  struct EncodingIndexes
  {
    std::vector< std::size_t >& indexes;
    std::size_t amount;
  };

  std::istream& operator>>(std::istream& in, EncodingIndexes&& encodingIndexes)
  {
    iofmtguard ifmtguard(in);
    in >> std::noskipws;

    using PrefixedIO = PrefixedIO< ' ', std::size_t >;

    std::vector< std::size_t > indexes;
    std::copy_n(std::istream_iterator< PrefixedIO >(in), encodingIndexes.amount, std::back_inserter(indexes));

    if (!in)
    {
      return in;
    }

    auto begin = indexes.begin();
    auto end = indexes.end();
    auto decrement = std::bind(std::minus{}, std::placeholders::_1, 1);
    std::transform(begin, end, begin, decrement);
    encodingIndexes.indexes = std::move(indexes);
    return in;
  }

  struct EncodingNumber
  {
    std::size_t& number;
    std::size_t maxSize;
  };

  std::istream& operator>>(std::istream& in, EncodingNumber&& encodingNumber)
  {
    std::size_t temp;
    in >> temp;
    if (temp == 0 || temp > encodingNumber.maxSize)
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    encodingNumber.number = temp;
    return in;
  }

  struct Entry
  {
    char symbol;
    std::string code;
  };

  Entry entryCreator(Symbol s)
  {
    return { s.symbol, s.code };
  }

  bool compareEntryCodes(const Entry& a, const Entry& b)
  {
    return a.code < b.code;
  };
}

namespace voronina
{
  void getCodes(std::vector< ShannonFanoTable >& vectorOfTables, std::istream& in, std::ostream& out)
  {
    iofmtguard inguard{ in };
    iofmtguard outguard{ out };
    std::string filename;
    in >> std::noskipws >> DelimiterIO{ ' ' } >> filename;
    if (!in || filename.empty())
    {
      in.setstate(std::ios::failbit);
      throw std::invalid_argument("Имя файла не может быть пустым");
    }
    std::string text = readFileContents(filename);
    ShannonFanoTable fanoTable;
    fanoTable.generateShannonFanoCodes(text, filename);
    vectorOfTables.emplace_back(std::move(fanoTable));
    out << "Кодировка успешно построена. Номер кодировки в таблице - " << vectorOfTables.size() << '\n';
  }

  void encode(std::vector< ShannonFanoTable >& vectorOfTables, std::istream& in, std::ostream& out)
  {
    iofmtguard inguard{ in };
    iofmtguard outguard{ out };
    std::string fileToRead;
    std::string fileToWrite;

    in >> std::noskipws >> DelimiterIO{ ' ' } >> fileToRead;
    if (fileToRead.empty())
    {
      throw std::invalid_argument("Имя файла для чтения не может быть пустым");
    }

    in >> std::noskipws >> DelimiterIO{ ' ' } >> fileToWrite;
    if (fileToWrite.empty())
    {
      throw std::invalid_argument("Имя файла для записи не может быть пустым");
    }

    std::string text = readFileContents(fileToRead);
    ShannonFanoTable fanoTable;
    fanoTable.generateShannonFanoCodes(text, fileToRead);
    vectorOfTables.emplace_back(std::move(fanoTable));
    out << "Кодировка успешно построена. Номер кодировки в таблице - " << vectorOfTables.size() << '\n';

    std::string encodedText = "";
    int amountOfSignificantBits = vectorOfTables.back().encode(text, encodedText);
    if (amountOfSignificantBits == 0)
    {
      amountOfSignificantBits = 8;
    }
    writeInFile(fileToWrite, encodedText);
    out << "Файл успешно закодирован\nРезультат кодирования записан в файл: " << fileToWrite << '\n';
    out << "Количество значащих бит в последнем байте: " << amountOfSignificantBits << '\n';

    auto originalFileSize = getFileSize(fileToRead);
    auto encodedFileSize = getFileSize(fileToWrite);

    out << "Размер оригинального файла: " << originalFileSize << " байт\n";
    out << "Размер закодированного файла: " << encodedFileSize << " байт\n";

    if (encodedFileSize < originalFileSize)
    {
      double percent = (100.0 - (static_cast< double >(encodedFileSize) / originalFileSize) * 100.0);
      out << "Сжатие достигнуто: " << percent << "%\n";
    }
    else
    {
      out << "Сжатие не достигнуто или размер файла увеличился.\n";
    }
  }

  void decode(const std::vector< ShannonFanoTable >& vectorOfTables, std::istream& in, std::ostream& out)
  {
    iofmtguard inguard{ in };
    iofmtguard outguard{ out };

    std::string fileToRead;
    std::string fileToWrite;

    in >> std::noskipws >> DelimiterIO{ ' ' } >> fileToRead;
    if (fileToRead.empty())
    {
      in.setstate(std::ios::failbit);
      throw std::invalid_argument("Имя файла для чтения не может быть пустым");
    }

    in >> std::noskipws >> DelimiterIO{ ' ' } >> fileToWrite;
    if (fileToWrite.empty())
    {
      in.setstate(std::ios::failbit);
      throw std::invalid_argument("Имя файла для записи не может быть пустым");
    }

    std::size_t encodingNumber;
    in >> DelimiterIO{ ' ' } >> EncodingNumber{ encodingNumber, vectorOfTables.size() };
    if (!in)
    {
      throw std::invalid_argument("Неверный номер кодировки");
    }

    std::size_t bits = 0;
    in >> DelimiterIO{ ' ' } >> bits;
    if (!in)
    {
      throw std::invalid_argument("Вы ввели некорректное число значащих битов");
    }

    if (bits == 0 || bits > 8)
    {
      throw std::invalid_argument("Количество значащих бит должно быть в диапазоне [1, 8]");
    }

    std::size_t encodingIndex = encodingNumber - 1;

    std::string encodedText = readFileContents(fileToRead);
    std::string decodedText = vectorOfTables[encodingIndex].decode(encodedText, bits % 8);
    writeInFile(fileToWrite, decodedText);
    out << "Файл " + fileToRead + " успешно декодирован\nРезультат декодирования записан в файл: " << fileToWrite
        << '\n';
  }

  void printInfo(const std::vector< ShannonFanoTable >& vectorOfTables, std::istream& in, std::ostream& out)
  {
    iofmtguard inguard{ in };
    iofmtguard outguard{ out };
    std::size_t encodingNumber;
    in >> EncodingNumber{ encodingNumber, vectorOfTables.size() };
    if (!in)
    {
      throw std::invalid_argument("Неверный номер кодировки");
    }

    std::size_t encodingIndex = encodingNumber - 1;
    if (encodingIndex >= vectorOfTables.size())
    {
      throw std::invalid_argument("Код с номером " + std::to_string(encodingNumber) + " не найден");
    }
    out << vectorOfTables[encodingIndex] << '\n';
  }

  void compare(const std::vector< ShannonFanoTable >& vectorOfTables, std::istream& in, std::ostream& out)
  {
    iofmtguard inguard{ in };
    iofmtguard outguard{ out };
    std::string filename;
    in >> std::noskipws >> DelimiterIO{ ' ' } >> filename;
    if (!in || filename.empty())
    {
      in.setstate(std::ios::failbit);
      throw std::runtime_error("Файл пустой и не может быть закодирован");
    }
    std::size_t encodingsAmount;
    in >> EncodingAmount{ encodingsAmount, vectorOfTables.size() };
    if (!in)
    {
      throw std::invalid_argument("Неверное количество кодировок");
    }
    std::vector< std::size_t > encodingIndexes;
    in >> EncodingIndexes{ encodingIndexes, encodingsAmount };
    if (!in)
    {
      throw std::invalid_argument("Ошибка чтения номеров кодировок");
    }
    std::string encodedFilename = "encodedCompare.txt";
    out << std::left << std::setw(10) << "Encoding" << std::setw(20) << "Compression ratio" << '\n';
    out << std::fixed << std::setprecision(2);

    std::vector< std::size_t > validIndexes;
    auto less_than_size = std::bind(std::less< std::size_t >(), std::placeholders::_1, vectorOfTables.size());
    std::copy_if(encodingIndexes.begin(), encodingIndexes.end(), std::back_inserter(validIndexes), less_than_size);

    std::vector< CompressionResult > results;
    std::string text = readFileContents(filename);
    std::streamsize before = getFileSize(filename);
    CompressionFunctor functor{ vectorOfTables, text, before, encodedFilename };
    std::transform(encodingIndexes.begin(), encodingIndexes.end(), std::back_inserter(results), functor);

    std::copy(results.begin(), results.end(), std::ostream_iterator< CompressionResult >(out, "\n"));
  }

  void entropy(const std::vector< ShannonFanoTable >& vectorOfTables, std::istream& in, std::ostream& out)
  {
    iofmtguard inguard{ in };
    iofmtguard outguard{ out };
    std::string filename;
    in >> std::noskipws >> DelimiterIO{ ' ' } >> filename;
    if (!in || filename.empty())
    {
      in.setstate(std::ios::failbit);
      throw std::invalid_argument("Имя файла не может быть пустым");
    }

    std::string text = readFileContents(filename);
    ShannonFanoTable fanoTable;
    fanoTable.generateShannonFanoCodes(text);

    out << std::fixed << std::setprecision(2);
    out << "Энтропия текста в файле " << filename << " составляет: ";
    out << fanoTable.calculateEntropy() << " бит/символ\n";
  }

  void origin(const std::vector< ShannonFanoTable >& vectorOfTables, std::istream& in, std::ostream& out)
  {
    iofmtguard inguard{ in };
    iofmtguard outguard{ out };
    std::size_t encodingNumber;
    in >> EncodingNumber{ encodingNumber, vectorOfTables.size() };
    if (!in)
    {
      throw std::invalid_argument("Неверный номер кодировки");
    }
    std::size_t encodingIndex = encodingNumber - 1;
    out << vectorOfTables[encodingIndex].originFile() << '\n';
  }

  void listCodes(const std::vector< ShannonFanoTable >& vectorOfTables, std::istream& in, std::ostream& out)
  {
    iofmtguard inguard{ in };
    iofmtguard outguard{ out };
    std::vector< CodeInfo > codes;
    CodeInfoFunctor functor;
    std::transform(vectorOfTables.begin(), vectorOfTables.end(), std::back_inserter(codes), functor);
    out << CodeInfoHeader{};
    std::copy(codes.begin(), codes.end(), std::ostream_iterator< CodeInfo >(out, "\n"));
  }

  void codesInfo(const std::vector< ShannonFanoTable >& vectorOfTables, std::istream& in, std::ostream& out)
  {
    iofmtguard inguard{ in };
    iofmtguard outguard{ out };
    std::size_t encodingsAmount;
    in >> EncodingAmount{ encodingsAmount, vectorOfTables.size() };
    if (!in)
    {
      throw std::invalid_argument("Неверное количество кодировок");
    }
    std::vector< std::size_t > encodingIndexes;
    in >> EncodingIndexes{ encodingIndexes, encodingsAmount };
    if (!in)
    {
      throw std::invalid_argument("Ошибка чтения номеров кодировок");
    }

    std::vector< std::size_t > validIndexes;
    auto less_than_size = std::bind(std::less< std::size_t >(), std::placeholders::_1, vectorOfTables.size());
    std::copy_if(encodingIndexes.begin(), encodingIndexes.end(), std::back_inserter(validIndexes), less_than_size);

    std::vector< CodeInfo > codes;
    std::vector< std::reference_wrapper< const ShannonFanoTable > > chosenTables;
    using ConstAtFuncType = const ShannonFanoTable& (std::vector< ShannonFanoTable >::*) (std::size_t) const;
    auto chooserFunction = static_cast< ConstAtFuncType >(&std::vector< ShannonFanoTable >::at);
    auto tableChooser = std::bind(chooserFunction, &vectorOfTables, std::placeholders::_1);
    std::transform(validIndexes.begin(), validIndexes.end(), std::back_inserter(chosenTables), tableChooser);

    CodeInfoFunctor functor;
    std::transform(chosenTables.begin(), chosenTables.end(), std::back_inserter(codes), functor);
    out << CodeInfoHeader{};
    std::copy(codes.begin(), codes.end(), std::ostream_iterator< CodeInfo >(out, "\n"));
  }

  void ableCoding(const std::vector< ShannonFanoTable >& vectorOfTables, std::istream& in, std::ostream& out)
  {
    iofmtguard ifmtguard(in);
    iofmtguard ofmtguard(out);

    std::string filename;
    in >> std::noskipws >> DelimiterIO{ ' ' } >> filename;
    if (!in || filename.empty())
    {
      in.setstate(std::ios::failbit);
      throw std::invalid_argument("Имя файла не может быть пустым");
    }
    std::string text = readFileContents(filename);

    std::size_t encodingsAmount;
    in >> EncodingAmount{ encodingsAmount, vectorOfTables.size() };
    if (!in)
    {
      throw std::invalid_argument("Неверное количество кодировок");
    }
    std::vector< std::size_t > encodingIndexes;
    in >> EncodingIndexes{ encodingIndexes, encodingsAmount };
    if (!in)
    {
      throw std::invalid_argument("Ошибка чтения номеров кодировок");
    }

    std::vector< std::size_t > validIndexes;
    auto less_than_size = std::bind(std::less< std::size_t >(), std::placeholders::_1, vectorOfTables.size());
    std::copy_if(encodingIndexes.begin(), encodingIndexes.end(), std::back_inserter(validIndexes), less_than_size);

    CanEncodeTransformer functor{ text, vectorOfTables };
    std::vector< std::string > canEncodeResults;
    std::transform(validIndexes.begin(), validIndexes.end(), std::back_inserter(canEncodeResults), functor);

    using namespace std::placeholders;
    std::vector< CanEncodePrinter > printers;
    auto begin = validIndexes.begin();
    auto end = validIndexes.end();
    auto builder = std::bind(canEncodePrinterBuilder, std::ref(out), _1, _2);
    std::transform(begin, end, canEncodeResults.begin(), std::back_inserter(printers), std::move(builder));
    out << CanEncodeHeader{};
    std::copy(printers.begin(), printers.end(), std::ostream_iterator< CanEncodePrinter >{ out, "\n" });
  }

  void visualize(const std::vector< ShannonFanoTable >& vectorOfTables, std::istream& in, std::ostream& out)
  {
    iofmtguard ifmtguard{ in };
    iofmtguard ofmtguard{ out };
    std::size_t encodingNumber;
    in >> EncodingNumber{ encodingNumber, vectorOfTables.size() };
    if (!in)
    {
      throw std::invalid_argument("Неверный номер кодировки");
    }
    std::size_t encodingIndex = encodingNumber - 1;
    const ShannonFanoTable& table = vectorOfTables.at(encodingIndex);

    std::vector< Entry > entries;

    std::transform(table.symbols().begin(), table.symbols().end(), std::back_inserter(entries), entryCreator);
    std::sort(entries.begin(), entries.end(), compareEntryCodes);

    std::string prevCode;

    for (const auto& entry: entries)
    {
      size_t common = 0;
      while (common < prevCode.size() && common < entry.code.size() && prevCode[common] == entry.code[common])
        ++common;
      std::string indent;
      for (size_t i = 0; i < entry.code.size(); ++i)
      {
        bool isLast = (i + 1 == entry.code.size());
        out << indent << (isLast ? "└─ " : "├─ ") << entry.code[i] << "\n";
        indent += isLast ? "    " : "│   ";
      }
      out << indent << "└─ '" << entry.symbol << "'\n";
      prevCode = entry.code;
    }
  }

  void definiteEncode(const std::vector< ShannonFanoTable >& vectorOfTables, std::istream& in, std::ostream& out)
  {
    iofmtguard inguard{ in };
    iofmtguard outguard{ out };
    std::string fileToRead;
    std::string fileToWrite;
    in >> std::noskipws >> DelimiterIO{ ' ' } >> fileToRead;
    if (!in || fileToRead.empty())
    {
      in.setstate(std::ios::failbit);
      throw std::invalid_argument("Имя файла для чтения не может быть пустым");
    }
    in >> std::noskipws >> DelimiterIO{ ' ' } >> fileToWrite;
    if (!in || fileToWrite.empty())
    {
      in.setstate(std::ios::failbit);
      throw std::invalid_argument("Имя файла для записи не может быть пустым");
    }
    std::size_t encodingNumber;
    in >> DelimiterIO{ ' ' } >> EncodingNumber{ encodingNumber, vectorOfTables.size() };
    if (!in)
    {
      throw std::invalid_argument("Неверный номер кодировки");
    }
    std::size_t encodingIndex = encodingNumber - 1;
    std::string text = readFileContents(fileToRead);
    std::string encodedText = "";
    int amountOfSignificantBits = vectorOfTables[encodingIndex].encode(text, encodedText);
    amountOfSignificantBits = amountOfSignificantBits == 0 ? 8 : amountOfSignificantBits;
    writeInFile(fileToWrite, encodedText);
    out << "Файл успешно закодирован\n";
    out << "Результат кодирования записан в файл: " << fileToWrite << '\n';
    out << "Количество значащих бит в последнем байте: " << amountOfSignificantBits << '\n';
  }
}
