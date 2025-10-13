#include "dataset.hpp"
#include "huffman.hpp"

bool mazitov::DataSetManager::createDataSet(const std::string& name)
{
  if (dataSets.find(name) != dataSets.end())
  {
    return false;
  }
  dataSets[name] = DataSet();
  return true;
}

bool mazitov::DataSetManager::dataSetExists(const std::string& name) const
{
  return dataSets.find(name) != dataSets.end();
}

mazitov::DataSet* mazitov::DataSetManager::getDataSet(const std::string& name)
{
  auto it = dataSets.find(name);
  if (it != dataSets.end())
  {
    return &it->second;
  }
  return nullptr;
}

bool mazitov::DataSetManager::deleteDataSet(const std::string& name)
{
  return dataSets.erase(name) > 0;
}

bool mazitov::DataSetManager::compressDataSet(const std::string& name)
{
  auto* set = getDataSet(name);
  if (set == nullptr || set->originalText.empty())
  {
    return false;
  }
  set->huffCodes.clear();

  auto freq = buildFreqTable(set->originalText);
  auto root = buildHuffmanTree(freq);
  if (root == nullptr)
  {
    return false;
  }

  generateCodes(root, set->huffCodes);
  return true;
}

std::string mazitov::DataSetManager::getCompressedData(const std::string& name) const
{
  auto it = dataSets.find(name);
  if (it == dataSets.end())
  {
    return "";
  }
  std::string res;
  for (char c : it->second.originalText)
  {
    res += it->second.huffCodes.at(c);
  }
  return res;
}

std::size_t mazitov::DataSetManager::getCompressedSize(const std::string& name) const
{
  if (dataSets.find(name) == dataSets.end())
  {
    return 0;
  }
  return getCompressedData(name).size();
}
