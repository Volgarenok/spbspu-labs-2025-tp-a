#ifndef KNAPSACK_HPP
#define KNAPSACK_HPP

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
}

#endif
