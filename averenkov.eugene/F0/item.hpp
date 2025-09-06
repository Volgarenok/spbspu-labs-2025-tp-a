#ifndef ITEM_HPP
#define ITEM_HPP

#include <string>
#include <ostream>

namespace averenkov
{
  struct Item
  {
  public:
    Item(const std::string& n, int w, int v);
    std::string getName() const;
    int getWeight() const;
    int getValue() const;
    void setWeight(int w);
    void setValue(int v);

  private:
    std::string name_;
    int weight_;
    int value_;
  };

  std::ostream& operator<<(std::ostream& out, const Item& item);

}

#endif
