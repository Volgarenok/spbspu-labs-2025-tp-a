#include "commands.hpp"
#include <iostream>
#include <fstream>
#include <numeric>

bool averenkov::ItemFinder::operator()(const Item& item) const
{
  return item.getName() == name;
}

bool averenkov::ItemFinderPtr::operator()(const Item* item) const
{
  return item->getName() == name;
}

bool averenkov::KitFinder::operator()(const std::pair< const std::string, Kit >& kit) const
{
  return kit.first == name;
}

void averenkov::KitItemRemover::operator()(std::pair< const std::string, Kit >& kit) const
{
  kit.second.removeItem(name);
}

const averenkov::Item* averenkov::ItemCopier::operator()(const averenkov::Item* item) const
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

int averenkov::WeightCalculator::operator()(int sum, const Item* item)
{
  return sum + item->getWeight();
}

int averenkov::ValueCalculator::operator()(int sum, const Item* item)
{
  return sum + item->getValue();
}

averenkov::ItemAdder::ItemAdder(Kit& k):
  kit(k)
{}

void averenkov::ItemAdder::operator()(const Item* item)
{
  kit.addItem(item);
}

void averenkov::ItemPrinter::operator()(const Item& item) const
{
  out << "Item: " << item.getName();
  out << ", Weight: " << item.getWeight();
  out << ", Value: " << item.getValue() << "\n";
}

void averenkov::KitItemPrinter::operator()(const Item* item) const
{
  out << "    - " << item->getName() << "\n";
}

void averenkov::KitPrinter::operator()(const std::pair< const std::string, Kit >& kit_pair) const
{
  out << "Kit: " << kit_pair.first << "\n";
  out << "  Items:\n";
  auto its = kit_pair.second.getItems();
  std::for_each(its.begin(), its.end(), KitItemPrinter{ out });
}

void averenkov::KnapsackPrinter::operator()(const std::pair< const std::string, Knapsack >& knapsack_pair) const
{
  out << "Knapsack: " << knapsack_pair.first;
  out << ", Capacity: " << knapsack_pair.second.getCapacity() << "\n";
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

  if (std::any_of(base.items.begin(), base.items.end(), ItemFinder{ args[1] }))
  {
    throw std::invalid_argument("Item already exists");
  }

  base.items.emplace_back(args[1], std::stoi(args[2]), std::stoi(args[3]));
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

  it->setWeight(std::stoi(args[2]));
  it->setValue(std::stoi(args[3]));
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
  Item* item_ptr = std::addressof(*item_it);

  if (std::find(items_in_kit.begin(), items_in_kit.end(), item_ptr) != items_in_kit.end())
  {
    throw std::invalid_argument("Item already in kit");
  }

  kit_it->second.addItem(std::addressof(*item_it));
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
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  if (base.knapsacks.find(args[1]) == base.knapsacks.end())
  {
    throw std::invalid_argument("<NOT FOUND>");
  }

  base.current_knapsack = base.knapsacks.find(args[1])->second;
}

void averenkov::showStats(const Base& base, const std::vector< std::string >& args)
{
  if (args.size() != 1)
  {
    throw std::invalid_argument("stats command takes no arguments");
  }
  std::cout << "=== Items ===\n";
  std::for_each(base.items.begin(), base.items.end(), ItemPrinter{ std::cout });
  std::cout << "\n=== Kits ===\n";
  std::for_each(base.kits.begin(), base.kits.end(), KitPrinter{ std::cout });
  std::cout << "\n=== Knapsacks ===\n";
  std::for_each(base.knapsacks.begin(), base.knapsacks.end(), KnapsackPrinter{ std::cout });
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
  out << "=== Items ===\n";
  std::for_each(base.items.begin(), base.items.end(), ItemPrinter{ out });

  out << "\n=== Kits ===\n";
  std::for_each(base.kits.begin(), base.kits.end(), KitPrinter{ out });

  out << "\n=== Knapsacks ===\n";
  std::for_each(base.knapsacks.begin(), base.knapsacks.end(), KnapsackPrinter{out});

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
      base.items.emplace_back(name, weight, value);
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
          kit.addItem(std::addressof(*it));
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
