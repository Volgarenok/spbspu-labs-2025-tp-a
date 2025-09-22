#ifndef WORDFUNCTORS_HPP
#define WORDFUNCTORS_HPP

#include <string>
#include <vector>
#include <map>
#include "index.hpp"

namespace krylov
{
  struct FindWord
  {
    const std::string& word;
    const IndexDocument& index;
    const std::string& indexName;
    std::ostream& out;
    FindWord(const IndexDocument& index_, const std::string& word_, const std::string& indexName_, std::ostream& out_);
    void operator()() const;
  };

}

#endif
