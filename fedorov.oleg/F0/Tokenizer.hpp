#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <vector>
#include <string>
#include <cctype>
#include <functional>
#include <algorithm>

namespace crossref
{

  class Tokenizer
  {
  public:
    static std::vector< std::string > tokenize(const std::string &input);
  };

}

#endif
