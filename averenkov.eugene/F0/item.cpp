#include "item.hpp"

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

