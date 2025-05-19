#include "data_struct.hpp"

#include <iostream>
#include <string>
#include <vector>

int main()
{
  std::vector<DataStruct> data;
  std::string line;

  while (std::getline(std::cin, line)) {
    if (!isValidRecord(line)) {
      continue;
    }

    DataStruct record{};
    if (parseRecord(line, record)) {
      data.push_back(record);
    }
  }

  sortData(data);

  for (const DataStruct& item : data) {
    printData(std::cout, item);
    std::cout << '\n';
  }

  return 0;
}
