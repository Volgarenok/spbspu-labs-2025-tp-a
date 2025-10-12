#include "commands.hpp"
#include "dataset.hpp"
#include "huffman.hpp"
#include <algorithm>
#include <iomanip>
#include <stdexcept>
#include <vector>
#include <numeric>
#include <fstream>
#include <iterator>

namespace
{
  struct CharFilter
  {
    const std::string& text;
    explicit CharFilter(const std::string& t):
      text(t)
    {}
    bool operator()(char c) const
    {
      return text.find(c) == std::string::npos;
    }
  };
  struct HexPrinter
  {
    std::ostream& out;
    std::size_t counter;
    explicit HexPrinter(std::ostream& o):
      out(o),
      counter(0)
    {}
    void operator()(unsigned char byte)
    {
      out << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << static_cast< int >(byte);
      counter++;
      if (counter % 8 == 0)
      {
        out << "\n";
      }
      else
      {
        out << " ";
      }
    }
  };
  struct ByteGenerator
  {
    const std::string& bits;
    explicit ByteGenerator(const std::string& b):
      bits(b)
    {}
    unsigned char operator()(std::size_t byteIndex) const
    {
      unsigned char byte = 0;
      std::size_t startBit = byteIndex * 8;
      for (std::size_t j = 0; j < 8 && startBit + j < bits.size(); j++)
      {
        if (bits[startBit + j] == '1')
        {
          byte |= (1 << (7 - j));
        }
      }
      return byte;
    }
  };
  struct BitsGenerator
  {
    std::string& bits;
    explicit BitsGenerator(std::string& b):
      bits(b)
    {}
    void operator()(unsigned char byte) const
    {
      for (int i = 7; i >= 0; i--)
      {
        bits += (byte & (1 << i)) ? '1' : '0';
      }
    }
  };
}

void mazitov::createCommand(DataSetManager& mgmt, std::istream& in, std::ostream& out)
{
  std::string setName;
  in >> setName;
  if (mgmt.dataSetExists(setName))
  {
    throw std::runtime_error("<SET_ALREADY_EXISTS>");
  }
  if (mgmt.createDataSet(setName))
  {
    out << "Set " << setName << " created successfully.\n";
  }
  else
  {
    throw std::runtime_error("<CREATE_FAILED>");
  }
}

void mazitov::getCmds(std::map< std::string, std::function< void(DataSetManager&, std::istream&, std::ostream&) > >& cmds)
{
  cmds["create"] = createCommand;
  cmds["compress"] = compressCommand;
  cmds["decompress"] = decompressCommand;
  cmds["degree"] = degreeCommand;
  cmds["show_original"] = showOriginalCommand;
  cmds["show_compressed"] = showCompressedCommand;
  cmds["delete"] = deleteCommand;
  cmds["merge"] = mergeCommand;
  cmds["subtract"] = subtractCommand;
  cmds["compare_degree"] = compareDegreesCommand;
  cmds["show_code"] = showCodeCommand;
  cmds["load_text"] = loadTextCommand;
  cmds["compressedToBin"] = compressedToBinCommand;
  cmds["loadCompressed"] = loadCompressedCommand;
}

void mazitov::compressCommand(DataSetManager& mgmt, std::istream& in, std::ostream& out)
{
  std::string setName, text;
  in >> setName;
  in >> std::ws;
  std::getline(in, text);
  if (!text.empty() && text[0] == ' ')
  {
    text = text.substr(1);
  }
  if (!text.empty() && text[0] == '"' && text.back() == '"')
  {
    text = text.substr(1, text.size() - 2);
  }
  if (!mgmt.dataSetExists(setName))
  {
    throw std::runtime_error("<SET_NOT_FOUND>");
  }
  auto* dataSet = mgmt.getDataSet(setName);
  if (!text.empty())
  {
    dataSet->originalText = text;
  }
  else if (!dataSet->originalText.empty())
  {
    text = dataSet->originalText;
  }
  else
  {
    throw std::runtime_error("<NO_TEXT_TO_COMPRESS>");
  }
  if (mgmt.compressDataSet(setName))
  {
    dataSet->compressedBits = mgmt.getCompressedData(setName);
    out << "Succesful: text was compressed and add in " << setName << ".\n";
  }
}

void mazitov::decompressCommand(DataSetManager& mgmt, std::istream& in, std::ostream& out)
{
  std::string setName;
  in >> setName;
  if (!mgmt.dataSetExists(setName))
  {
    throw std::runtime_error("<SET_NOT_FOUND>");
  }
  auto* dataSet = mgmt.getDataSet(setName);
  if (dataSet->originalText.empty())
  {
    throw std::runtime_error("<NOT_COMPRESSED_DATA>");
  }
  out << "Original text: \"" << dataSet->originalText << "\"\n";
}

void mazitov::degreeCommand(DataSetManager& mgmt, std::istream& in, std::ostream& out)
{
  std::string setName;
  in >> setName;
  if (!mgmt.dataSetExists(setName))
  {
    throw std::runtime_error("<SET_NOT_FOUND>");
  }
  auto* dataSet = mgmt.getDataSet(setName);
  if (dataSet->originalText.empty() || dataSet->huffCodes.empty())
  {
    throw std::runtime_error("<NO_COMPRESSED_DATA>");
  }

  std::string compressed = mgmt.getCompressedData(setName);
  double originalSize = dataSet->originalText.size() * 8.0;
  double compressedSize = compressed.size();
  double ratio = compressedSize / originalSize;
  out << "Degree ratio: " << std::fixed << std::setprecision(2) << ratio << "\n";
}

void mazitov::showOriginalCommand(DataSetManager& mgmt, std::istream& in, std::ostream& out)
{
  std::string setName;
  in >> setName;
  if (!mgmt.dataSetExists(setName))
  {
    throw std::runtime_error("<SET_NOT_FOUND>");
  }

  auto* dataSet = mgmt.getDataSet(setName);
  if (dataSet->originalText.empty())
  {
    throw std::runtime_error("<NO_ORIGINAL_TEXT_LOAD>");
  }
  out << dataSet->originalText << "\n";
}

void mazitov::showCompressedCommand(DataSetManager& mgmt, std::istream& in, std::ostream& out)
{
  std::string setName;
  in >> setName;
  if (!mgmt.dataSetExists(setName))
  {
    throw std::runtime_error("<SET_NOT_FOUND>");
  }

  auto* ds = mgmt.getDataSet(setName);
  std::string compressedBits = ds->compressedBits;
  if (compressedBits.empty())
  {
    if (ds->originalText.empty() || ds->huffCodes.empty())
    {
      throw std::runtime_error("<NO_COMPRESSED_DATA>");
    }
    compressedBits = mgmt.getCompressedData(setName);
  }
  std::size_t numBytes = (compressedBits.size() + 7) / 8;
  std::vector< std::size_t > ind(numBytes);
  std::iota(ind.begin(), ind.end(), 0);
  std::vector< unsigned char > bytes(numBytes);
  std::transform(ind.begin(), ind.end(), bytes.begin(), ByteGenerator(compressedBits));
  HexPrinter printer(out);
  std::for_each(bytes.begin(), bytes.end(), std::ref(printer));
  if (!bytes.empty() && bytes.size() % 8 != 0)
  {
    out << "\n";
  }
}

void mazitov::deleteCommand(DataSetManager& mgmt, std::istream& in, std::ostream& out)
{
  std::string setName;
  in >> setName;
  if (!mgmt.dataSetExists(setName))
  {
    throw std::runtime_error("<SET_NOT_FOUND>");
  }
  mgmt.deleteDataSet(setName);
  out << "Set: " << setName << "was deleted.\n";
}

void mazitov::mergeCommand(DataSetManager& mgmt, std::istream& in, std::ostream& out)
{
  std::string set1, set2, resultSet;
  in >> set1 >> set2 >> resultSet;
  if (!mgmt.dataSetExists(set1))
  {
    throw std::runtime_error("<SET1_NOT_FOUND>");
  }
  if (!mgmt.dataSetExists(set2))
  {
    throw std::runtime_error("<SET2_NOT_FOUND>");
  }
  if (mgmt.dataSetExists(resultSet))
  {
    throw std::runtime_error("<RESULT_SET_ALREADY_EXISTS>");
  }
  auto* ds1 = mgmt.getDataSet(set1);
  auto* ds2 = mgmt.getDataSet(set2);
  mgmt.createDataSet(resultSet);
  auto* result = mgmt.getDataSet(resultSet);
  result->originalText = ds1->originalText + ds2->originalText;
  mgmt.compressDataSet(resultSet);
  out << "Sets: " << set1 << " and " << set2 << " were merge in " << resultSet << "\n";
}

void mazitov::subtractCommand(DataSetManager& mgmt, std::istream& in, std::ostream& out)
{
  std::string set1, set2, resultSet;
  in >> set1 >> set2 >> resultSet;
  if (!mgmt.dataSetExists(set1))
  {
    throw std::runtime_error("<SET1_NOT_FOUND>");
  }
  if (!mgmt.dataSetExists(set2))
  {
    throw std::runtime_error("<SET2_NOT_FOUND>");
  }
  if (mgmt.dataSetExists(resultSet))
  {
    throw std::runtime_error("<RESULT_SET_ALREADY_EXISTS>");
  }
  mgmt.createDataSet(resultSet);
  auto* ds1 = mgmt.getDataSet(set1);
  auto* ds2 = mgmt.getDataSet(set2);
  auto* result = mgmt.getDataSet(resultSet);
  std::string resultText;
  resultText.resize(ds2->originalText.size());
  auto it = std::copy_if(ds2->originalText.begin(), ds2->originalText.end(), resultText.begin(), CharFilter(ds1->originalText));
  resultText.erase(it, resultText.end());
  result->originalText = resultText;
  mgmt.compressDataSet(resultSet);
  out << "From set2" << " were deleted all codes including in set1\n";
}

void mazitov::compareDegreesCommand(DataSetManager& mgmt, std::istream& in, std::ostream& out)
{
  std::string set1, set2;
  in >> set1 >> set2;
  if (!mgmt.dataSetExists(set1))
  {
    throw std::runtime_error("<SET1_NOT_FOUND>");
  }
  if (!mgmt.dataSetExists(set2))
  {
    throw std::runtime_error("<SET2_NOT_FOUND>");
  }

  auto* ds1 = mgmt.getDataSet(set1);
  auto* ds2 = mgmt.getDataSet(set2);
  if (ds1->originalText.empty() || ds1->huffCodes.empty())
  {
    throw std::runtime_error("<NO_COMPRESSED_DATA_IN_SET1>");
  }
  if (ds2->originalText.empty() || ds2->huffCodes.empty())
  {
    throw std::runtime_error("<NO_COMPRESSED_DATA_IN_SET2>");
  }
  std::string comp1 = mgmt.getCompressedData(set1);
  std::string comp2 = mgmt.getCompressedData(set2);
  double ratio1 = static_cast< double >(comp1.size()) / (ds1->originalText.size() * 8);
  double ratio2 = static_cast< double >(comp2.size()) / (ds2->originalText.size() * 8);
  if (ratio1 < ratio2)
  {
    out << set1 << " better compress (" << ratio1 << " < " << ratio2 << ")\n";
  }
  else if (ratio2 < ratio1)
  {
    out << set2 << " better compress (" << ratio2 << " < " << ratio1 << ")\n";
  }
  else
  {
    out << "Two sets has equal degree of ratio: " << ratio1 << '\n';
  }
}

void mazitov::showCodeCommand(DataSetManager& mgmt, std::istream& in, std::ostream& out)
{
  std::string setName;
  in >> setName;
  if (!mgmt.dataSetExists(setName))
  {
    throw std::runtime_error("<SET_NOT_FOUND>");
  }

  auto* ds = mgmt.getDataSet(setName);
  if (ds->huffCodes.empty())
  {
    throw std::runtime_error("<NO_COMPRESSED_DATA>");
  }
  out << "Huffman codes from set " << setName << ":\n";
  for (const auto& pair : ds->huffCodes)
  {
    out << "  '" << pair.first << "': " << pair.second << "\n";
  }
}

void mazitov::loadTextCommand(DataSetManager& mgmt, std::istream& in, std::ostream& out)
{
  std::string setName, filename;
  in >> setName >> filename;
  if (!mgmt.dataSetExists(setName))
  {
    throw std::runtime_error("<SET_NOT_FOUND>");
  }
  std::ifstream file(filename);
  if (!file)
  {
    throw std::runtime_error("<FILE_NOT_FOUND>");
  }

  std::string content((std::istreambuf_iterator< char >(file)), std::istreambuf_iterator< char >());
  if (content.empty())
  {
    throw std::runtime_error("<TEXT_IS_EMPTY>");
  }

  auto* ds = mgmt.getDataSet(setName);
  ds->originalText = content;
  out << "File " << filename << "was loaded at set " << setName << "\n";
}

void mazitov::compressedToBinCommand(DataSetManager& mgmt, std::istream& in, std::ostream& out)
{
  std::string setName, filename;
  in >> setName >> filename;
  if (!mgmt.dataSetExists(setName))
  {
    throw std::runtime_error("<SET_NOT_FOUND>");
  }
  auto* ds = mgmt.getDataSet(setName);
  if (ds->originalText.empty() || ds->huffCodes.empty())
  {
    throw std::runtime_error("<NO_COMPRESSED_DATA>");
  }

  std::string compressedBits = mgmt.getCompressedData(setName);
  std::size_t numBytes = (compressedBits.size() + 7) / 8;
  std::vector< std::size_t > ind(numBytes);
  std::iota(ind.begin(), ind.end(), 0);
  std::vector< unsigned char > bytes(numBytes);
  std::transform(ind.begin(), ind.end(), bytes.begin(), ByteGenerator(compressedBits));
  std::ofstream file(filename, std::ios::binary);
  file.write(reinterpret_cast< const char* >(bytes.data()), bytes.size());
  file.close();
  out << "Compressed data from set " << setName << "was saved in file " << filename << "\n";
}

void mazitov::loadCompressedCommand(DataSetManager& mgmt, std::istream& in, std::ostream& out)
{
  std::string setName, filename;
  in >> setName >> filename;
  if (!mgmt.dataSetExists(setName))
  {
    throw std::runtime_error("<SET_NOT_FOUND>");
  }
  std::ifstream file(filename, std::ios::binary);
  if (!file)
  {
    throw std::runtime_error("<FILE_NOT_FOUND>");
  }
  file.seekg(0, std::ios::end);
  std::streamsize fileSize = file.tellg();
  file.seekg(0, std::ios::beg);
  if (fileSize <= 0)
  {
    throw std::runtime_error("<COMPRESSED_DATA_EMPTY>");
  }
  std::vector< unsigned char > bytes(fileSize);
  if (!file.read(reinterpret_cast< char* >(bytes.data()), fileSize))
  {
    throw std::runtime_error("<FILE_READ_ERROR>");
  }
  std::string comprBits;
  std::for_each(bytes.begin(), bytes.end(), BitsGenerator(comprBits));
  auto* ds = mgmt.getDataSet(setName);
  ds->compressedBits = comprBits;
  out << "Compressed data loaded from " << filename << " to set " << setName << "\n";
}
