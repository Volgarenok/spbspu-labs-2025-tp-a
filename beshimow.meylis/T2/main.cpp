#include "data_struct.hpp"

#include <vector>
#include <iterator>
#include <algorithm>

int main() {
  std::vector<data_struct_t> data_vec;

  std::istream_iterator<data_struct_t> input_begin(std::cin);
  std::istream_iterator<data_struct_t> input_end;

  std::copy(input_begin, input_end, std::back_inserter(data_vec));

  std::sort(data_vec.begin(), data_vec.end(), [](const data_struct_t &a, const data_struct_t &b) {
    if (a.key1 != b.key1) {
      return a.key1.real() < b.key1.real() ||
             (a.key1.real() == b.key1.real() && a.key1.imag() < b.key1.imag());
    }
    if (a.key2 != b.key2) {
      return a.key2 < b.key2;
    }
    return a.key3.size() < b.key3.size();
  });

  std::ostream_iterator<data_struct_t> out_iter(std::cout, "\n");
  std::copy(data_vec.begin(), data_vec.end(), out_iter);

  return 0;
}
