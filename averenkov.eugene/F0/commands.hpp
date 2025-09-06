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
    bool operator()(const std::shared_ptr< Item >& item) const;
  };

  struct ItemFinderPtr
  {
    const std::string& name;
    bool operator()(std::weak_ptr< const Item > weakItem) const;
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
    std::shared_ptr< const Item > operator()(std::shared_ptr< const Item > item) const;
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
    int operator()(int sum, std::shared_ptr< const Item > item);
  };

  struct ValueCalculator
  {
    int operator()(int sum, std::shared_ptr< const Item > item);
  };

  struct WeakWeightCalculator
  {
    int operator()(int sum, std::weak_ptr< const Item > item);
  };

  struct WeakValueCalculator
  {
    int operator()(int sum, std::weak_ptr< const Item > item);
  };

  struct WeakItemAdder
  {
    explicit WeakItemAdder(Kit& k);
    void operator()(std::weak_ptr< const Item > weak_item);
  private:
    Kit& kit;
  };

  void printItemToOut(std::ostream& out, const std::shared_ptr< Item >& item);
  void printKitToOut(std::ostream& out, const std::pair< const std::string, Kit >& kit_pair);
  void printKnapsackToOut(std::ostream& out, const std::pair< const std::string, Knapsack >& knapsack_pair);

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
