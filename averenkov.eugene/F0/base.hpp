#ifndef BASE_HPP
#define BASE_HPP

#include <vector>
#include <map>
#include <string>
#include <memory>
#include "item.hpp"
#include "kit.hpp"
#include "knapsack.hpp"

namespace averenkov
{
  class Base
  {
  public:
    Base();
    std::vector< std::shared_ptr< Item > > items;
    std::map< std::string, Kit > kits;
    std::map< std::string, Knapsack > knapsacks;
    Knapsack current_knapsack;
  };
}

#endif
