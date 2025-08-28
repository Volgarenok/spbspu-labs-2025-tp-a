#include <string>
#include <map>
#include <functional>
#include <fstream>
#include <iostream>
#include <limits>

#include "workout.hpp"
#include "commands.hpp"

int main()
{
  std::map< size_t, std::map< time_t, dribas::workout > > suite;
  suite[1] = std::map< time_t, dribas::workout >();
  std::map< std::string, std::function< void() > > cmds;
  cmds["add_training_manual"] = std::bind(dribas::add_training_manual, std::ref(std::cin), std::ref(std::cout), std::ref(suite));
  cmds["add_training_from_file"] = std::bind(dribas::add_training_from_file, std::ref(std::cin), std::ref(std::cout), std::ref(suite));
  cmds["show_all_trainings"] = std::bind(dribas::show_all_trainings, std::ref(std::cout), std::cref(suite));
  cmds["show_trainings_by_date"] = std::bind(dribas::show_trainings_by_date, std::ref(std::cin), std::ref(std::cout), std::cref(suite));
  cmds["query_trainings"] = std::bind(dribas::query_trainings, std::ref(std::cin), std::ref(std::cout), std::ref(suite));
  cmds["calculate_average"] = std::bind(dribas::calculate_average, std::ref(std::cin), std::ref(std::cout), std::ref(suite));
  cmds["get_top_trainings"] = std::bind(dribas::get_top_trainings, std::ref(std::cin), std::ref(std::cout), std::cref(suite));
  cmds["get_under_trainings"] = std::bind(dribas::get_under_trainings, std::ref(std::cin), std::ref(std::cout), std::cref(suite));
  cmds["survival_score"] = std::bind(dribas::survival_score, std::ref(std::cout), std::cref(suite));
  cmds["show_rest"] = std::bind(dribas::show_rest, std::ref(std::cin), std::ref(std::cout), std::cref(suite));
  cmds["predict_result"] = std::bind(dribas::predict_result, std::ref(std::cout), std::cref(suite));
  cmds["analyze_training_segment"] = std::bind(dribas::analyze_training_segment, std::ref(std::cin), std::ref(std::cout), std::cref(suite));

  std::string command;
  while (!(std::cin >> command).eof()) {
    try {
      cmds.at(command)();
      std::cout << '\n';
    } catch (std::exception& e) {
      if (std::cin.fail()) {
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }
  return 0;

}
