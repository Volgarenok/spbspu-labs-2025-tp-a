#ifndef PROCESSING_STRUCTURES_HPP
#define PROCESSING_STRUCTURES_HPP
#include <string>

namespace zakirov
{
  struct Data
  {
    bool operator<(const Data & other);
    unsigned long long key1;
    unsigned long long key2;
    std::string key3;
  };

  struct MinorSymbol
  {
    char symbol;
  };

  struct MinorLetter
  {
    char letter;
  };

  struct UllOctIO
  {
    unsigned long long & ref;
  };

  struct UllHexIO
  {
    unsigned long long & ref;
  };

  struct StringIO
  {
    std::string & exp;
  };

  std::istream & operator>>(std::istream & in, Data & data);
  std::istream & operator>>(std::istream & in, MinorSymbol && sym);
  std::istream & operator>>(std::istream & in, MinorLetter && str);
  std::istream & operator>>(std::istream & in, UllOctIO && num);
  std::istream & operator>>(std::istream & in, UllHexIO && num);
  std::istream & operator>>(std::istream & in, StringIO && str);
  std::ostream & operator<<(std::ostream & out, const Data & src);

}

#endif
