#include "kit.hpp"

averenkov::ItemNameComparer::ItemNameComparer(const std::string& n):
  name_(n)
{}

averenkov::Kit::Kit(const std::string& name):
  name_(name)
{}

bool averenkov::Kit::compareItemByName(const Item* item, const std::string& name)
{
  return item->getName() == name;
}

bool averenkov::ItemNameComparer::operator()(const Item* item) const
{
  return item->getName() == name_;
}

void averenkov::Kit::addItem(const Item* item)
{
  if (containsItem(item->getName()))
  {
    throw std::invalid_argument("b");
  }
  items_.push_back(item);
}

std::vector< const averenkov::Item* > averenkov::Kit::getItems() const
{
  return items_;
}

void averenkov::Kit::removeItem(const std::string& item_name)
{
  auto it = std::remove_if(items_.begin(), items_.end(), ItemNameComparer(item_name));
  items_.erase(it, items_.end());
}

bool averenkov::Kit::containsItem(const std::string& item_name) const
{
  auto func = std::bind(compareItemByName, std::placeholders::_1, item_name);
  return std::any_of(items_.begin(), items_.end(), func);
}
