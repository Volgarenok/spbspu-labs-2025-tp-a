#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <string>

namespace nehvedovich
{
  struct data_struct
  {
    unsigned long long key1;
    char key2;
    std::string key3;
  };

  std::istream &operator>>(std::istream &in, data_struct &dest);
  std::ostream &operator<<(std::ostream &out, const data_struct &dest);
  bool operator<(const data_struct &lhs, const data_struct &rhs);
} // namespace nehvedovich

#endif
