#ifndef KIT_HPP
#define KIT_HPP
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <stdexcept>
#include "item.hpp"

namespace averenkov
{
  class Kit
  {
  public:
    Kit(const std::string& n);

    void addItem(const Item* item);
    std::vector< const Item* > getItems() const;
    void removeItem(const std::string& item_name);
    bool containsItem(const std::string& item_name) const;

  private:
    std::string name_;
    std::vector< const Item* > items_;
    static bool compareItemByName(const Item* item, const std::string& name);
  };


  struct ItemNameComparer
  {
  public:
    ItemNameComparer(const std::string& n);
    bool operator()(const Item* item) const;

  private:
    const std::string& name_;

  };
}

#endif
