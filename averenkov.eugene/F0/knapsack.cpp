#include "knapsack.hpp"

averenkov::Knapsack::Knapsack(int capacity):
  capacity_(capacity)
{}

int averenkov::Knapsack::getCapacity() const
{
  return capacity_;
}
