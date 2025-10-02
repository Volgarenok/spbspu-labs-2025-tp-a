#ifndef KIT_HPP
#define KIT_HPP
#include <string>
#include <memory>
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

    void addItem(std::shared_ptr< const Item > item);
    std::vector< std::weak_ptr< const Item > > getItems() const;
    void removeItem(const std::string& item_name);
    bool containsItem(const std::string& item_name) const;

  private:
    std::string name_;
    std::vector< std::weak_ptr< const Item > > items_;
  };

  struct KitItemPrinter
  {
  public:
    void operator()(const std::weak_ptr< const Item > item) const;
    std::ostream& out;
  };

  std::ostream& operator<<(std::ostream& out, const std::pair<const std::string, Kit>& kit_pair);
}

#endif
