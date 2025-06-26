#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <stdexcept>
#include "base.hpp"

namespace averenkov
{
  struct ItemFinder
  {
    const std::string& name;
    bool operator()(const Item& item) const;
  };

  struct ItemFinderPtr
  {
    const std::string& name;
    bool operator()(const Item* item) const;
  };

  struct KitFinder
  {
    const std::string& name;
    bool operator()(const std::pair< const std::string, Kit >& kit) const;
  };

  struct KitItemRemover
  {
    const std::string& name;
    void operator()(std::pair< const std::string, Kit >& kit) const;
  };

  class ItemCopier
  {
  public:
    const Item* operator()(const Item* item) const;
  };

  struct MaskChecker
  {
    explicit MaskChecker(int m);
    bool operator()(int pos) const;
  private:
    int mask;
  };

  struct WeightCalculator
  {
    int operator()(int sum, const Item* item);
  };

  struct ValueCalculator
  {
    int operator()(int sum, const Item* item);
  };

  struct ItemAdder
  {
    explicit ItemAdder(Kit& k);
    void operator()(const Item* item);
  private:
    Kit& kit;
  };

  class ItemPrinter
  {
  public:
    std::ostream& out;
    void operator()(const Item& item) const;
  };

  class KitItemPrinter
  {
  public:
    std::ostream& out;
    void operator()(const Item* item) const;
  };

  class KitPrinter
  {
  public:
    std::ostream& out;
    void operator()(const std::pair< const std::string, Kit >& kit_pair) const;
  };

  class KnapsackPrinter
  {
  public:
    std::ostream& out;
    void operator()(const std::pair< const std::string, Knapsack >& knapsack_pair) const;
  };

  void printHelp(std::ostream& out);
  void addItem(Base& base, const std::vector< std::string >& args);
  void removeItem(Base& base, const std::vector< std::string >& args);
  void editItem(Base& base, const std::vector< std::string >& args);
  void addKit(Base& base, const std::vector< std::string >& args);
  void removeKit(Base& base, const std::vector< std::string >& args);
  void addToKit(Base& base, const std::vector< std::string >& args);
  void removeFromKit(Base& base, const std::vector< std::string >& args);
  void addKnapsack(Base& base, const std::vector< std::string >& args);
  void setKnapsack(Base& base, const std::vector< std::string >& args);
  void showStats(const Base& base, const std::vector< std::string >& args);
  void reset(Base& base, const std::vector< std::string >& args);
  void saveToFile(const Base& base, const std::vector< std::string >& args);
  void loadFromFile(Base& base, const std::vector< std::string >& args);
}

#endif
