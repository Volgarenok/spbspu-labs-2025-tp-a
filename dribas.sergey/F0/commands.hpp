#ifndef CMDS_HPP
#define CMDS_HPP

#include <map>
#include "workout.hpp"

namespace dribas
{
  using suite = std::map< size_t, std::map< time_t, dribas::workout > >;
  void add_training_manual(std::istream&, std::ostream&, suite&);
  void add_training_from_file(std::istream&, std::ostream&, suite&);
  void show_all_trainings(std::ostream&, const suite&);
  void show_trainings_by_date(std::istream&, std::ostream&, const suite&);
  void query_trainings(std::istream&, std::ostream&, suite&);
  void calculate_average(std::istream&, std::ostream&, suite&);
  void get_top_trainings(std::istream&, std::ostream&, const suite&);
  void get_under_trainings(std::istream&, std::ostream&, const suite&);
  void survival_score(std::ostream&, const suite&);
  void show_rest(std::istream&, std::ostream&, const suite&);
  void predict_result(std::ostream&, const suite&);
  void analyze_training_segment(std::istream&, std::ostream&, const suite&);
  void delete_training_by_key(std::istream&, std::ostream&, suite&);
  void delete_training_suite(std::istream&, std::ostream&, suite&);
}



#endif
