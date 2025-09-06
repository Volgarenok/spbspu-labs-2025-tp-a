#include "item.hpp"

#include <iostream>


averenkov::Item::Item(const std::string& n, int w, int v):
  name_(n),
  weight_(w),
  value_(v)
{}

std::string averenkov::Item::getName() const
{
  return name_;
}

int averenkov::Item::getWeight() const
{
  return weight_;
}

int averenkov::Item::getValue() const
{
  return value_;
}

void averenkov::Item::setWeight(int w)
{
  weight_ = w;
}
void averenkov::Item::setValue(int v)
{
  value_ = v;
}

std::ostream& averenkov::operator<<(std::ostream& os, const Item& item)
{
  os << "Item: ";
  os << item.getName();
  os << ", Weight: ";
  os<< item.getWeight();
  os << ", Value: ";
  os << item.getValue() << "\n";
  return os;
}
