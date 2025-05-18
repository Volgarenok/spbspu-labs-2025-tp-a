#include "data_struct.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int main()
{
  std::vector<DataStruct> dataVec;

  try
  {
    std::istream_iterator<DataStruct> inputBegin(std::cin);
    std::istream_iterator<DataStruct> inputEnd;
    bool hasValidData = false;
    for (auto it = inputBegin; it != inputEnd; ++it)
    {
      dataVec.push_back(*it);
      hasValidData = true;
    }

    if (!hasValidData)
    {
      std::cerr << "Atleast one supported record type\n";
      return 1;
    }

    std::sort(dataVec.begin(), dataVec.end(),
      [](const DataStruct& a, const DataStruct& b)
      {
        if (a.key1.real() != b.key1.real())
          return a.key1.real() < b.key1.real();
        if (a.key1.imag() != b.key1.imag())
          return a.key1.imag() < b.key1.imag();
        if (a.key2.first != b.key2.first)
          return a.key2.first < b.key2.first;
        if (a.key2.second != b.key2.second)
          return a.key2.second < b.key2.second;
        return a.key3.size() < b.key3.size();
      });

    std::ostream_iterator<DataStruct> outIter(std::cout, "\n");
    std::copy(dataVec.begin(), dataVec.end(), outIter);
  }
  catch (const std::exception& e)
  {
    std::cerr << "Looks like there is no supported record. Cannot determine input. Test skipped\n";
    return 2;
  }

  return 0;
}
