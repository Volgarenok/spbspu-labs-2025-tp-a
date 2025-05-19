#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP

#include <string>
#include <vector>
#include <utility>
#include <istream>
#include <ostream>

struct DataStruct
{
  double key1;
  std::pair<long long, unsigned long long> key2;
  std::string key3;
};

bool isValidRecord(const std::string& line);
bool parseRecord(const std::string& line, DataStruct& data);
void sortData(std::vector<DataStruct>& data);
void printData(std::ostream& out, const DataStruct& data);

#endif
