#include <limits>
#include "command_processing.hpp"
#include "input_output_processing.hpp"

int main(int arg, char** argv)
{
  using namespace lanovenko;
  if (arg == 2 && std::string(argv[1]) == "--help")
  {
    help(std::cout);
  }
  targets trg;
  targets_sets trgs;
  pantsir_s ps;
  std::cout << "< ENTER INITIAL PANTSIR STATUS >\n";
  try
  {
    input_pantsir_status(std::cin, ps);
    input_targets(std::cin, trg);
  }
  catch (const std::logic_error& e)
  {
    std::cerr << e.what();
    return 1;
  }
  commands_t commands;
  commands["create_target"] = std::bind(create_target, std::ref(std::cin), std::ref(trg));
  commands["delete_target"] = std::bind(delete_target, std::ref(std::cin), std::ref(trg), std::ref(trgs));
  commands["engage_max_targets"] = std::bind(engage_max_targets, std::ref(std::cin), std::ref(std::cout),
  std::ref(trg), std::ref(trgs), std::ref(ps));
  commands["engage_balanced"] = std::bind(engage_balanced, std::ref(std::cin), std::ref(std::cout),
   std::ref(trg), std::ref(trgs), std::ref(ps));
  commands["engage_top_threats"] = std::bind(engage_top_threats, std::ref(std::cin), std::ref(std::cout),
   std::ref(trg), std::ref(trgs), std::ref(ps));
  commands["engage_manual"] = std::bind(engage_manual, std::ref(std::cin), std::ref(std::cout),
   std::ref(trg), std::ref(trgs), std::ref(ps));
  commands["solve_threat"] = std::bind(solve_threat, std::ref(std::cin), std::ref(std::cout),
   std::ref(trg), std::ref(trgs), std::ref(ps));
  commands["target_list"] = std::bind(target_list, std::ref(std::cin), std::ref(std::cout), std::cref(trgs));
  commands["system_status"] = std::bind(system_status, std::ref(std::cin), std::ref(std::cout), std::cref(ps));
  commands["create_target_set"] = std::bind(create_target_set, std::ref(std::cin), std::ref(trgs));
  commands["add_target_to_set"] = std::bind(add_target_to_set, std::ref(std::cin), std::ref(trgs), std::ref(trg));
  commands["all_targets"] = std::bind(all_targets, std::ref(std::cout), std::cref(trg));
  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      commands.at(command)();
    }
    catch (const std::out_of_range& e)
    {
      std::cout << "< INVALID COMMAND >\n";
    }
    catch (const std::exception& e)
    {
      std::cout << e.what() << "\n";
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
  return 0;
}
