#ifndef KNAPSACK_HPP
#define KNAPSACK_HPP
#include <ostream>

namespace averenkov
{
  class Knapsack
  {
  public:
    explicit Knapsack(int capacity);
    int getCapacity() const;

  private:
    int capacity_;
  };

  std::ostream& operator<<(std::ostream& out, const std::pair< const std::string, Knapsack >& knapsack_pair);
}

#endif
