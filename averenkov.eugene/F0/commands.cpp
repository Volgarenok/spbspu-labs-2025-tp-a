#include "commands.hpp"
#include <iostream>
#include <fstream>
#include <numeric>

bool averenkov::ItemFinder::operator()(const std::shared_ptr< Item >& item) const
{
  return item->getName() == name;
}

bool averenkov::ItemFinderPtr::operator()(std::weak_ptr< const Item > weakItem) const
{
  auto item = weakItem.lock();
  return item && item->getName() == name;
}

bool averenkov::KitFinder::operator()(const std::pair< const std::string, Kit >& kit) const
{
  return kit.first == name;
}

void averenkov::KitItemRemover::operator()(std::pair< const std::string, Kit >& kit) const
{
  kit.second.removeItem(name);
}

std::shared_ptr< const averenkov::Item > averenkov::ItemCopier::operator()(std::shared_ptr< const Item > item) const
{
  return item;
}

averenkov::MaskChecker::MaskChecker(int m):
  mask(m)
{}

bool averenkov::MaskChecker::operator()(int pos) const
{
  return mask & (1 << pos);
}

int averenkov::WeightCalculator::operator()(int sum, std::shared_ptr< const Item > item)
{
  return sum + item->getWeight();
}

int averenkov::ValueCalculator::operator()(int sum, std::shared_ptr< const Item > item)
{
  return sum + item->getValue();
}

int averenkov::WeakWeightCalculator::operator()(int sum, std::weak_ptr< const Item > item)
{
  if (auto weak_item = item.lock())
  {
    return sum + weak_item->getWeight();
  }
  return sum;
}

int averenkov::WeakValueCalculator::operator()(int sum, std::weak_ptr< const Item > item)
{
  if (auto weak_item = item.lock())
  {
    return sum + weak_item->getValue();
  }
  return sum;
}

averenkov::WeakItemAdder::WeakItemAdder(Kit& k):
  kit(k)
{}

void averenkov::WeakItemAdder::operator()(std::weak_ptr< const Item > weak_item)
{
  if (auto item = weak_item.lock())
  {
    kit.addItem(item);
  }
}

void averenkov::printItemToOut(std::ostream& out, const std::shared_ptr< Item >& item)
{
  out << *item;
}


void averenkov::printKitToOut(std::ostream& out, const std::pair< const std::string, Kit >& kit_pair)
{
  out << kit_pair;
}

void averenkov::printKnapsackToOut(std::ostream& out, const std::pair< const std::string, Knapsack >& knapsack_pair)
{
  out << knapsack_pair;
}

void averenkov::printHelp(std::ostream& out)
{
  out << "Knapsack Problem Solver - Help Menu\n\n";

  out << "Basic Item Commands:\n";
  out << " add <name> <weight> <value> - Add item to database\n";
  out << " remove <name> - Remove item from database\n";
  out << " edit <name> <new_w> <new_v> - Edit item parameters\n\n";
  out << "Kit Management:\n";
  out << " add_kit <kit_name> - Create new empty kit\n";
  out << " remove_kit <kit_name> - Delete a kit\n";
  out << " add_to_kit <kit> <item> - Add item to kit\n";
  out << " remove_from_kit <kit> <item> - Remove item from kit\n\n";

  out << "Knapsack Operations:\n";
  out << "  add_knapsack <name> <capacity>  - Create new knapsack\n";
  out << "  set_knapsack <name>            - Set active knapsack\n\n";

  out << "Solving Methods:\n";
  out << "  solve <kit> <result_name>       - Auto-select method\n";
  out << "  bruteforce <kit> <result_name>  - Brute-force solution\n";
  out << "  dynamic_prog <kit> <result_name> - Dynamic programming\n";
  out << "  backtracking <kit> <result_name> - Backtracking method\n";
  out << "  branch_and_bound <kit> <result_name> - Branch and bound\n\n";

  out << "Utility Commands:\n";
  out << "  stats                           - Show all database contents\n";
  out << "  reset                           - Clear all kits and knapsacks\n";
  out << "  save <filename>                - Save database to file\n";
  out << "  load <filename>                - Load database from file\n\n";

  out << "Launch Options:\n";
  out << "  knapsack [file]                - Start with database file\n";
  out << "  --help                         - Show this help message\n";
}

void averenkov::addItem(Base& base, const std::vector< std::string >& args)
{
  if (args.size() != 4)
  {
    throw std::invalid_argument("Invalid arguments count for add");
  }

  auto new_item = std::make_shared<Item>(args[1], std::stoi(args[2]), std::stoi(args[3]));

  if (std::any_of(base.items.begin(), base.items.end(), ItemFinder{ args[1] }))
  {
    throw std::invalid_argument("Item already exists");
  }

  base.items.push_back(new_item);
}

void averenkov::removeItem(Base& base, const std::vector< std::string >& args)
{
  if (args.size() != 2)
  {
    throw std::invalid_argument("Invalid arguments count for remove");
  }

  auto it = std::find_if(base.items.begin(), base.items.end(), ItemFinder{ args[1] });
  if (it == base.items.end())
  {
    throw std::invalid_argument("Item not found");
  }
  std::for_each(base.kits.begin(), base.kits.end(), KitItemRemover{ args[1] });

  base.items.erase(it);
}

void averenkov::editItem(Base& base, const std::vector< std::string >& args)
{
  if (args.size() != 4)
  {
    throw std::invalid_argument("Invalid arguments count for edit");
  }

  auto it = std::find_if(base.items.begin(), base.items.end(), ItemFinder{ args[1] });
  if (it == base.items.end())
  {
    throw std::invalid_argument("Item not found");
  }
  auto item = *it;
  item->setWeight(std::stoi(args[2]));
  item->setValue(std::stoi(args[3]));
}

void averenkov::addKit(Base& base, const std::vector< std::string >& args)
{
  if (args.size() != 2)
  {
    throw std::invalid_argument("Invalid arguments count for add_kit");
  }

  if (base.kits.find(args[1]) != base.kits.end())
  {
    throw std::invalid_argument("Kit already exists");
  }

  base.kits.emplace(args[1], Kit(args[1]));
}

void averenkov::removeKit(Base& base, const std::vector< std::string >& args)
{
  if (args.size() != 2)
  {
    throw std::invalid_argument("Invalid arguments count for remove_kit");
  }

  if (base.kits.erase(args[1]) == 0)
  {
    throw std::invalid_argument("Kit not found");
  }
}

void averenkov::addToKit(Base& base, const std::vector< std::string >& args)
{
  if (args.size() != 3)
  {
    throw std::invalid_argument("Invalid arguments count for add_to_kit");
  }

  auto kit_it = base.kits.find(args[1]);
  if (kit_it == base.kits.end())
  {
    throw std::invalid_argument("Kit not found");
  }


  auto item_it = std::find_if(base.items.begin(), base.items.end(), ItemFinder{ args[2] });
  if (item_it == base.items.end())
  {
    throw std::invalid_argument("Item not found");
  }

  const auto& items_in_kit = kit_it->second.getItems();
  auto found = std::find_if(items_in_kit.begin(), items_in_kit.end(), ItemFinderPtr{ args[2] });

  if (found != items_in_kit.end())
  {
    throw std::invalid_argument("Item already in kit");
  }

  kit_it->second.addItem(*item_it);
}

void averenkov::removeFromKit(Base& base, const std::vector< std::string >& args)
{
  if (args.size() != 3)
  {
    throw std::invalid_argument("Invalid arguments count for remove_from_kit");
  }

  auto kit_it = base.kits.find(args[1]);
  if (kit_it == base.kits.end())
  {
    throw std::invalid_argument("Kit not found");
  }

  const auto& items_in_kit = kit_it->second.getItems();
  auto it = std::find_if(items_in_kit.begin(), items_in_kit.end(), ItemFinderPtr{ args[2] });

  if (it == items_in_kit.end())
  {
    throw std::invalid_argument("Item not found in kit");
  }

  kit_it->second.removeItem(args[2]);
}

void averenkov::addKnapsack(Base& base, const std::vector< std::string >& args)
{
  if (args.size() != 3)
  {
    throw std::invalid_argument("Invalid arguments count for add_knapsack");
  }

  if (base.knapsacks.find(args[1]) != base.knapsacks.end())
  {
    throw std::invalid_argument("Knapsack already exists");
  }

  base.knapsacks.emplace(args[1], Knapsack(std::stoi(args[2])));
}

void averenkov::setKnapsack(Base& base, const std::vector< std::string >& args)
{
  if (args.size() != 2)
  {
    throw std::invalid_argument("Invalid arguments count for set_knapsack");
  }

  if (base.knapsacks.find(args[1]) == base.knapsacks.end())
  {
    throw std::invalid_argument("Knapsack not found");
  }

  base.current_knapsack = base.knapsacks.find(args[1])->second;
}

void averenkov::showStats(const Base& base, const std::vector< std::string >& args)
{
  if (args.size() != 1)
  {
    throw std::invalid_argument("stats command takes no arguments");
  }

  auto printItem = std::bind(printItemToOut, std::ref(std::cout), std::placeholders::_1);
  auto printKit = std::bind(printKitToOut, std::ref(std::cout), std::placeholders::_1);
  auto printKnapsack = std::bind(printKnapsackToOut, std::ref(std::cout), std::placeholders::_1);

  std::cout << "=== Items ===\n";
  std::for_each(base.items.begin(), base.items.end(), printItem);
  std::cout << "\n=== Kits ===\n";
  std::for_each(base.kits.begin(), base.kits.end(), printKit);
  std::cout << "\n=== Knapsacks ===\n";
  std::for_each(base.knapsacks.begin(), base.knapsacks.end(), printKnapsack);
  std::cout << "\n=== Current Knapsack ===\n";
  std::cout << "Capacity: " << base.current_knapsack.getCapacity() << "\n";
}

void averenkov::reset(Base& base, const std::vector< std::string >& args)
{
  if (args.size() != 1)
  {
    throw std::invalid_argument("reset command takes no arguments");
  }

  base.kits.clear();
  base.knapsacks.clear();
}

void averenkov::saveToFile(const Base& base, const std::vector< std::string >& args)
{
  if (args.size() < 2)
  {
    throw std::invalid_argument("Usage: save <filename>");
  }

  const std::string& filename = args[1];
  std::ofstream out(filename);
  if (!out)
  {
    throw std::runtime_error("Cannot open file for writing: " + filename);
  }

  auto printItem = std::bind(printItemToOut, std::ref(out), std::placeholders::_1);
  auto printKit = std::bind(printKitToOut, std::ref(out), std::placeholders::_1);
  auto printKnapsack = std::bind(printKnapsackToOut, std::ref(out), std::placeholders::_1);

  out << "=== Items ===\n";
  std::for_each(base.items.begin(), base.items.end(), printItem);

  out << "\n=== Kits ===\n";
  std::for_each(base.kits.begin(), base.kits.end(), printKit);

  out << "\n=== Knapsacks ===\n";
  std::for_each(base.knapsacks.begin(), base.knapsacks.end(), printKnapsack);

  out << "\n=== Current Knapsack ===\n";
  out << "Capacity: " << base.current_knapsack.getCapacity() << "\n";
}

void averenkov::loadFromFile(Base& base, const std::vector< std::string >& args)
{
  if (args.size() < 2)
  {
    throw std::invalid_argument("Usage: load <filename>");
  }

  const std::string& filename = args[1];
  std::ifstream in(filename);
  if (!in)
  {
    throw std::runtime_error("Cannot open file for reading: " + filename);
  }

  base = Base();

  std::string line;

  if (std::getline(in, line))
  {
    size_t pos = 0;
    while (pos < line.size())
    {
      std::string name;
      int weight, value;
      size_t space_pos = line.find(' ', pos);
      if (space_pos == std::string::npos)
      {
        break;
      }
      name = line.substr(pos, space_pos - pos);
      pos = space_pos + 1;
      space_pos = line.find(' ', pos);
      if (space_pos == std::string::npos)
      {
        break;
      }
      weight = std::stoi(line.substr(pos, space_pos - pos));
      pos = space_pos + 1;
      space_pos = line.find(' ', pos);
      value = std::stoi(line.substr(pos, (space_pos == std::string::npos) ? line.size() - pos : space_pos - pos));
      pos = (space_pos == std::string::npos) ? line.size() : space_pos + 1;
      auto new_item = std::make_shared< Item >(name, weight, value);
      base.items.push_back(new_item);
    }
  }

  if (std::getline(in, line))
  {
    size_t pos = 0;
    while (pos < line.size())
    {
      size_t space_pos = line.find(' ', pos);
      std::string kitName = line.substr(pos, (space_pos == std::string::npos) ? line.size() - pos : space_pos - pos);
      pos = (space_pos == std::string::npos) ? line.size() : space_pos + 1;
      auto kitIt = base.kits.find(kitName);
      if (kitIt == base.kits.end())
      {
        kitIt = base.kits.emplace(kitName, Kit(kitName)).first;
      }
      Kit& kit = kitIt->second;
      while (pos < line.size())
      {
        space_pos = line.find(' ', pos);
        std::string itemName = line.substr(pos, (space_pos == std::string::npos) ? line.size() - pos : space_pos - pos);
        pos = (space_pos == std::string::npos) ? line.size() : space_pos + 1;
        ItemFinder finder{ itemName };
        auto it = std::find_if(base.items.begin(), base.items.end(), finder);
        if (it != base.items.end())
        {
          kit.addItem(*it);
        }
      }
    }
  }

  if (std::getline(in, line))
  {
    size_t pos = 0;
    while (pos < line.size())
    {
      size_t space_pos = line.find(' ', pos);
      if (space_pos == std::string::npos)
      {
        break;
      }
      std::string knapsackName = line.substr(pos, space_pos - pos);
      pos = space_pos + 1;
      space_pos = line.find(' ', pos);
      int capacity = std::stoi(line.substr(pos, (space_pos == std::string::npos) ? line.size() - pos : space_pos - pos));
      pos = (space_pos == std::string::npos) ? line.size() : space_pos + 1;
      base.knapsacks.emplace(knapsackName, Knapsack(capacity));
    }
  }

  if (std::getline(in, line))
  {
    int capacity = std::stoi(line);
    base.current_knapsack = Knapsack(capacity);
  }

}
