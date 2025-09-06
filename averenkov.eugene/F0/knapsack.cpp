#include "knapsack.hpp"

averenkov::Knapsack::Knapsack(int capacity):
  capacity_(capacity)
{}

int averenkov::Knapsack::getCapacity() const
{
  return capacity_;
}

std::ostream& averenkov::operator<<(std::ostream& out, const std::pair< const std::string, Knapsack >& knapsack_pair)
{
  out << "Knapsack: " << knapsack_pair.first;
  out << ", Capacity: " << knapsack_pair.second.getCapacity() << "\n";
  return out;
}
