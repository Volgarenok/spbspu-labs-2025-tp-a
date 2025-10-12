#ifndef DATASET_HPP
#define DATASET_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <cstddef>

namespace mazitov
{
  struct DataSet
  {
    std::string originalText;
    std::string compressedBits;
    std::unordered_map< char, std::string > huffCodes;
  };

  class DataSetManager
  {
  public:
    bool createDataSet(const std::string &);
    bool dataSetExists(const std::string &) const;
    bool deleteDataSet(const std::string &);
    bool compressDataSet(const std::string &);
    DataSet* getDataSet(const std::string &);
    std::string getCompressedData(const std::string &) const;
    std::size_t getCompressedSize(const std::string &) const;
  private:
    std::map< std::string, DataSet > dataSets;
  };
}

#endif
