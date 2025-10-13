#include "kit.hpp"

averenkov::Kit::Kit(const std::string& name):
  name_(name)
{}

void averenkov::Kit::addItem(std::shared_ptr< const Item > item)
{
  if (containsItem(item->getName()))
  {
    throw std::invalid_argument("b");
  }
  items_.push_back(item);
}

std::vector< std::weak_ptr< const averenkov::Item > > averenkov::Kit::getItems() const
{
  return items_;
}

void averenkov::Kit::removeItem(const std::string& item_name)
{
  std::vector< std::weak_ptr< const Item > > new_items;

  for (size_t i = 0; i < items_.size(); ++i)
  {
    std::shared_ptr< const Item > shared_item = items_[i].lock();
    if (shared_item && shared_item->getName() != item_name)
    {
      new_items.push_back(items_[i]);
    }
  }

  items_ = new_items;
}

bool averenkov::Kit::containsItem(const std::string& item_name) const
{
  for (size_t i = 0; i < items_.size(); ++i)
  {
    std::shared_ptr< const Item > shared_item = items_[i].lock();
    if (shared_item && shared_item->getName() == item_name)
    {
      return true;
    }
  }
  return false;
}

void averenkov::KitItemPrinter::operator()(const std::weak_ptr< const Item > item) const
{
  if (auto shared_item = item.lock())
  {
    out << "    - " << shared_item->getName() << "\n";
  }
}

std::ostream& averenkov::operator<<(std::ostream& os, const std::pair< const std::string, Kit >& kit_pair)
{
  os << "Kit: " << kit_pair.first << "\n";
  os << "  Items:\n";
  auto its = kit_pair.second.getItems();
  std::for_each(its.begin(), its.end(), KitItemPrinter{ os });
  return os;
}
