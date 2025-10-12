#include "commands.hpp"
#include <algorithm>
#include <iterator>
#include <numeric>
#include <vector>
#include <functional>
#include <limits>
#include <delimiter.hpp>

#include "survival.hpp"
#include "rest_time.hpp"
#include "race_predictor.hpp"

namespace
{
  class TimeRangeFilter
  {
    time_t start;
    time_t end;
  public:
    TimeRangeFilter(time_t s, time_t e):
      start(s),
      end(e)
    {}
      bool operator()(const std::pair< time_t, dribas::workout >& session) const
      {
        return session.first >= start && session.first <= end;
      }
  };
  struct HeartRateExtractor
  {
    int operator()(const std::pair<time_t, dribas:: workout >& session) const
    {
      return session.second.avgHeart;
    }
  };
  struct CadenceExtractor {
    int operator()(const std::pair< time_t, dribas::workout >& session) const
    {
      return session.second.cadence;
    }
  };
  struct DistanceExtractor
  {
    double operator()(const std::pair< time_t, dribas::workout >& session) const
    {
      return session.second.distance;
    }
  };

  template< class T >
  bool compareStrings(const T& field_val, const std::string& op, const T& value)
  {
    if (op == "==") {
      return field_val == value;}
    if (op == "!=") {
      return field_val != value;}
    if (op == ">") {
      return field_val > value;}
    if (op == "<") {
      return field_val < value;}
    if (op == ">=") {
      return field_val >= value;}
    if (op == "<=") {
      return field_val <= value;}
    throw std::runtime_error("Unknown operator" + op);
  }

  long long parse_duration(int hours, int minutes, int seconds)
  {
    if (hours < 0 || minutes < 0 || minutes > 59 || seconds < 0 || seconds > 59) {
      throw std::runtime_error("Invalid duration format");
    }
    return static_cast< long long >(hours) * 3600 + static_cast< long long >(minutes) * 60 + static_cast< long long>(seconds);
  }

  double get_workout_numeric_param(const dribas::workout& w, int param_id)
  {
    long long value;
    switch (param_id) {
      case 2:
        value = w.avgHeart;
        break;
      case 3:
        value = w.maxHeart;
        break;
      case 4:
        value = w.cadence;
        break;
      case 5:
        value = w.distance;
        break;
      case 6:
        value = w.avgPaceMinPerKm;
        break;
      case 7:
        value = w.timeStart;
        break;
      case 8:
        value = w.timeEnd;
        break;
      default:
        throw std::runtime_error("unsupported parameter ID: ");
    }
    return value;
  }

  class WorkoutFilter
  {
  private:
    int param_id_;
    std::string op_;
    std::string value_str_;
    std::string param_name_for_errors_;
  public:
    WorkoutFilter(int param_id, const std::string& op, const std::string& value_str, const std::string& param_name):
      param_id_(param_id),
      op_(op),
      value_str_(value_str),
      param_name_for_errors_(param_name)
    {}
    bool operator()(const std::pair< const time_t, dribas::workout >& pair) const
    {
      const dribas::workout& w = pair.second;
      bool match = false;
      if (param_id_ == 1) {
        match = compareStrings(w.name, op_, value_str_);
      } else if (param_id_ >= 2 && param_id_ <= 8) {
        match = compareStrings(get_workout_numeric_param(w, param_id_), op_,std::stod(value_str_));
      } else {
        throw std::runtime_error("Unsupported parameter type for comparison: " + param_name_for_errors_);
      }
      return match;
    }
  };
  const dribas::workout& get_workout_from_pair(const std::pair< const time_t, dribas::workout >& entry)
  {
    return entry.second;
  }

  time_t parse_date(int year, int month, int day)
  {
    if (year < 1900) {
      throw std::runtime_error("Year must be 1900 or later");
    }
    if (month < 1 || month > 12) {
      throw std::runtime_error("Month must be between 1 and 12");
    }
    if (day < 1 || day > 31) {
      throw std::runtime_error("Day must be between 1 and 31");
    }
    std::tm tm = {};
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;
    tm.tm_isdst = -1;
    time_t result = std::mktime(&tm);
    if (result == -1) {
      throw std::runtime_error("Invalid date values");
    }
    std::tm* check_tm = std::localtime(&result);
    if (check_tm->tm_year + 1900 != year || check_tm->tm_mon + 1 != month || check_tm->tm_mday != day) {
      throw std::runtime_error("Invalid date (e.g. February 30)");
    }
    return result;
  }

  int get_param(const std::string& str)
  {
    std::map< std::string, int > param_map =
    {
      {"name", 1},
      {"avg_heart", 2},
      {"max_heart", 3},
      {"avg_cadence", 4},
      {"distance", 5},
      {"start_time", 7},
      {"end_time", 8}
    };
    auto it = param_map.find(str);
    return (it != param_map.end()) ? it->second : 0;
  }

  class GetParamValue
  {
  private:
    int param_id_;
    std::string param_name_for_errors_;
  public:
    GetParamValue(int id, const std::string& name):
      param_id_(id),
      param_name_for_errors_(name)
    {}
    double operator()(double current_sum, const std::pair< const time_t, dribas::workout >& pair) const
    {
      const dribas::workout& w = pair.second;
      return current_sum + get_workout_numeric_param(w, param_id_);
    }
  };

  class WorkoutComparator
  {
  private:
    int param_id_;
  public:
    WorkoutComparator(int id):
      param_id_(id)
    {}
    bool operator()(const dribas::workout& a, const dribas::workout& b) const
    {
      double val_a = get_workout_numeric_param(a, param_id_);
      double val_b = get_workout_numeric_param(b, param_id_);
      return val_a > val_b;
    }
  };
}


void dribas::add_training_from_file(std::istream& in, std::ostream& out, suite& trainng)
{
  std::string name;
  in >> name;
  std::ifstream file(name);
  workout training;
  file >> training;
  if (!in && !in.eof()) {
    throw std::invalid_argument("invalid file name");
  }
  auto& main_suite = trainng.at(1);
  main_suite.emplace(training.timeStart, training);
  out << "Trainings: " << training;
}

void dribas::show_all_trainings(std::ostream& out, const suite& trainings)
{
  std::transform(trainings.at(1).begin(), trainings.at(1).end(), std::ostream_iterator< workout >(out), get_workout_from_pair);
}

void dribas::show_trainings_by_date(std::istream& in, std::ostream& out, const suite& trainings)
{
  try {
    size_t year, mounth, day;
    size_t end_year, end_mounth, end_day;
    out << "Enter start date (YYYY-MM-DD): ";
    in >> year >> DelimiterI{ '-' };
    in >> mounth >> DelimiterI{ '-' };
    in >> day;
    out << "Enter end date (YYYY-MM-DD): ";
    in >> end_year >> DelimiterI{ '-' };
    in >> end_mounth >> DelimiterI{ '-' };
    in >> end_day;
    if (!in) {
      throw std::invalid_argument("invalid date");
    }
    auto start_date = parse_date(year, mounth, day);
    auto end_date = parse_date(end_year, end_mounth, end_day) + 86400;
    if (start_date > end_date) {
      out << "Error: start date is after end date\n";
      return;
    }
    auto lower = trainings.at(1).lower_bound(start_date);
    auto upper = trainings.at(1).upper_bound(end_date);
    if (lower == upper) {
      out << "No workouts found for specified period.\n";
    } else {
      std::transform(lower, upper, std::ostream_iterator< workout >(out), get_workout_from_pair);
      out << "Total found: " << std::distance(lower, upper) << " workouts\n";
    }
  } catch (const std::exception& e) {
    out << "Error: " << e.what() << "\n";
  }
}

void dribas::query_trainings(std::istream& in, std::ostream& out, std::map< size_t, std::map< time_t, workout > >& trings)
{
  std::string param, op, value_str;
  in >> param >> op >> value_str;
  int param_id = get_param(param);
  if (param_id == 0) {
    throw std::runtime_error("Invalid parameter: " + param);
  }
  size_t query_id = 1;
  if (!trings.empty()) {
    query_id = trings.rbegin()->first + 1;
  }
  std::map< time_t, workout > filtered;
  auto it_source_workouts = trings.find(1);
  if (it_source_workouts == trings.end()) {
    throw std::runtime_error("Source workout list (ID 1) not found.");
  }
  const auto& source_workouts = it_source_workouts->second;
  WorkoutFilter filter_predicate_obj(param_id, op, value_str, param);
  std::copy_if(source_workouts.begin(), source_workouts.end(), std::inserter(filtered, filtered.end()), filter_predicate_obj);
  trings[query_id] = filtered;
  out << "Query ID: " << query_id << "\n";
  out << "Found " << filtered.size() << " matching workouts\n";
}

void dribas::calculate_average(std::istream& in, std::ostream& out, std::map< size_t, std::map< time_t, workout > >& trings)
{
  std::string param_name;
  size_t training_id;
  in >> param_name >> training_id;
  int param_id = get_param(param_name);
  if (param_id == 0) {
    throw std::runtime_error("Invalid parameter for average calculation: " + param_name);
  }
  if (param_id == 1) {
    throw std::runtime_error("Cannot calculate average for non-numeric parameter: " + param_name);
  }
  auto it_workouts_list = trings.find(training_id);
  if (it_workouts_list == trings.end()) {
    throw std::runtime_error("Training list with ID " + std::to_string(training_id) + " not found.");
  }
  const auto& workouts_to_average = it_workouts_list->second;
  if (workouts_to_average.empty()) {
    out << "No workouts found in list ID " << training_id << " to calculate average.\n";
    return;
  }
  class GetParamValue {
  private:
    int param_id_;
    std::string param_name_for_errors_;
  public:
    GetParamValue(int id, const std::string& name) : param_id_(id), param_name_for_errors_(name) {}
    double operator()(double current_sum, const std::pair<const time_t, dribas::workout>& pair) const {
      const dribas::workout& w = pair.second;
      try {
        return current_sum + get_workout_numeric_param(w, param_id_);
      } catch (const std::exception& e) {
        throw std::runtime_error("Error extracting value for average of parameter '" + param_name_for_errors_ + "': " + e.what());
      }
    }
  };
  double total_sum = std::accumulate(workouts_to_average.begin(), workouts_to_average.end(), 0.0, GetParamValue(param_id, param_name));
  out << "Average " << param_name << " for list ID " << training_id << ": " << total_sum / workouts_to_average.size() << "\n";
}


void dribas::add_training_manual(std::istream& in, std::ostream& out, std::map< size_t, std::map< time_t, workout > >& trainings)
{
  dribas::workout new_workout;
  unsigned int year, month, day, hour, minute;
  unsigned int duration_h, duration_m, duration_s;
  long long calculated_duration_seconds = 0;

  out << "Enter workout name (string): ";
  in >> new_workout.name;
  if (new_workout.name.empty()) {
    throw std::invalid_argument("Workout name cannot be empty.");
  }

  out << "Enter workout date (YYYY-MM-DD HH:MM): ";
  in >> year >> DelimiterI{'-'} >> month >> DelimiterI{'-'} >> day;
  in >> hour >> DelimiterI{':'} >> minute;
  if (!in) {
    throw std::invalid_argument("Invalid date or time format. Please use YYYY-MM-DD HH:MM.");
  }
  time_t date_only_time_t = parse_date(year, month, day);
  std::tm tm_start_struct = *std::localtime(&date_only_time_t);

  if (hour > 23 || minute > 59) {
    throw std::invalid_argument("Invalid hour or minute for workout time.");
  }
  tm_start_struct.tm_hour = hour;
  tm_start_struct.tm_min = minute;
  tm_start_struct.tm_sec = 0;
  tm_start_struct.tm_isdst = -1;
  new_workout.timeStart = std::mktime(&tm_start_struct);
  if (new_workout.timeStart == static_cast<time_t>(-1)) {
    throw std::runtime_error("Failed to convert entered date/time to system time. Possible invalid date/time values.");
  }
  out << "Enter distance (km): ";
  in >> new_workout.distance;
  if (!in || new_workout.distance < 0) {
    throw std::invalid_argument("Invalid distance. Must be a non-negative number.");
  }
  out << "Enter duration (HH:MM:SS): ";
  in >> duration_h >> DelimiterI{':'} >> duration_m >> DelimiterI{':'} >> duration_s;
  if (!in) {
    throw std::invalid_argument("Invalid duration format. Please use HH:MM:SS.");
  }
  calculated_duration_seconds = parse_duration(duration_h, duration_m, duration_s);
  if (calculated_duration_seconds <= 0) {
    throw std::invalid_argument("Duration must be a positive value.");
  }
  new_workout.timeEnd = new_workout.timeStart + calculated_duration_seconds;
  out << "Enter average cadence (steps/min): ";
  in >> new_workout.cadence;
  if (!in || new_workout.cadence < 0) {
    throw std::invalid_argument("Invalid average cadence. Must be a non-negative integer.");
  }
  out << "Enter average heart rate (bpm): ";
  in >> new_workout.avgHeart;
  if (!in || new_workout.avgHeart < 0) {
    throw std::invalid_argument("Invalid average heart rate. Must be a non-negative integer.");
  }
  new_workout.maxHeart = new_workout.avgHeart;
  if (new_workout.distance > 0) {
    new_workout.avgPaceMinPerKm = (static_cast< double >(calculated_duration_seconds) / 60.0) / (new_workout.distance);
  } else {
    new_workout.avgPaceMinPerKm = 0.0;
  }

  auto& main_suite = trainings[1];
  auto result = main_suite.emplace(new_workout.timeStart, new_workout);
  if (!result.second) {
    out << "Warning: Workout at this start time already exists and was not added.\n";
  } else {
    out << "Workout added successfully.\n";
    out << "Added workout details:\n" << new_workout << "\n";
  }
}

void dribas::get_top_trainings(std::istream& in, std::ostream& out, const suite& trainings)
{
  std::string parameter_name;
  size_t n_top;
  in >> parameter_name >> n_top;
  if (!in) {
    throw std::invalid_argument("Invalid input for get_top_trainings. Usage: <parameter> <n>");
  }
  if (n_top == 0) {
    out << "Number of top trainings must be greater than 0.\n";
    return;
  }
  int param_id = get_param(parameter_name);
  if (param_id == 0) {
    throw std::runtime_error("Invalid parameter for top trainings: " + parameter_name);
  }
  if (param_id == 1) {
    throw std::runtime_error("Cannot get top trainings by string parameter: " + parameter_name);
  }
  auto it_main_workouts = trainings.find(1);
  if (it_main_workouts == trainings.end() || it_main_workouts->second.empty()) {
    out << "No trainings available to find top ones.\n";
    return;
  }
  std::vector< dribas::workout > all_workouts;
  auto first = it_main_workouts->second.begin();
  auto second = it_main_workouts->second.end();
  std::transform(first, second, std::back_inserter(all_workouts), get_workout_from_pair);

  WorkoutComparator comparator_obj(param_id);
  std::sort(all_workouts.begin(), all_workouts.end(), comparator_obj);
  out << "Top " << n_top << " trainings by " << parameter_name << ":\n";
  auto begin = all_workouts.begin();
  auto end = all_workouts.begin() + std::min(n_top, all_workouts.size());
  std::copy(begin, end, std::ostream_iterator< dribas::workout >(out, "\n"));

  if (n_top > all_workouts.size() && !all_workouts.empty()) {
    out << "Note: Only " << all_workouts.size() << " trainings available, showing all of them.\n";
  }
}

void dribas::get_under_trainings(std::istream& in, std::ostream& out, const suite& trainings)
{
  std::string parameter_name;
  size_t n_top;
  in >> parameter_name >> n_top;
  if (!in) {
    throw std::invalid_argument("Invalid input for get_under_trainings. Usage: <parameter> <n>");
  }
  if (n_top == 0) {
    out << "Number of under trainings must be greater than 0.\n";
    return;
  }
  int param_id = get_param(parameter_name);
  if (param_id == 0) {
    throw std::runtime_error("Invalid parameter for under trainings: " + parameter_name);
  }
  if (param_id == 1) {
    throw std::runtime_error("Cannot get under trainings by string parameter: " + parameter_name);
  }
  auto it_main_workouts = trainings.find(1);
  if (it_main_workouts == trainings.end() || it_main_workouts->second.empty()) {
    out << "No trainings available to find top ones.\n";
    return;
  }
  std::vector< dribas::workout > all_workouts;
  auto first = it_main_workouts->second.begin();
  auto second = it_main_workouts->second.end();
  std::transform(first, second, std::back_inserter(all_workouts), get_workout_from_pair);

  WorkoutComparator comparator_obj(param_id);
  auto cmp = std::bind(comparator_obj, std::placeholders::_2, std::placeholders::_1);
  std::sort(all_workouts.begin(), all_workouts.end(), cmp);
  out << "Top " << n_top << " trainings by " << parameter_name << ":\n";
  auto begin = all_workouts.begin();
  auto end = all_workouts.begin() + std::min(n_top, all_workouts.size());
  std::copy(begin, end, std::ostream_iterator< dribas::workout >(out, "\n"));

  if (n_top > all_workouts.size() && !all_workouts.empty()) {
    out << "Note: Only " << all_workouts.size() << " trainings available, showing all of them.\n";
  }
}

void dribas::survival_score(std::ostream& out, const suite& suites) {
  try {
    const auto& main_suite = suites.at(1);
    out << calculate_survival_score(main_suite);
  } catch (const std::out_of_range&) {
    out << "Error: No workouts found in main suite (id=1)\n";
  } catch (const std::exception& e) {
    out << "Error: " << e.what() << "\n";
  }
}

void dribas::show_rest(std::istream& in, std::ostream& out, const suite& suites) {
  out << "Enter date (YYYY-MM-DD): ";
  unsigned int year, month, day;
  in >> year >> DelimiterI{'-'} >> month >> DelimiterI{'-'} >> day;
  if (!in) {
    throw std::invalid_argument("Invalid date format. Please use YYYY-MM-DD");
  }
  time_t date = parse_date(year, month, day);
  time_t next_day = date + 86400;
  const auto& main_suite = suites.at(1);
  auto range_start = main_suite.lower_bound(date);
  auto range_end = main_suite.lower_bound(next_day);
  if (range_start == range_end) {
    out << "  No workouts found for specified date.\n";
    return;
  }
  int total_recovery = std::accumulate(range_start, range_end, 0, calculate_workout_recovery);

  StreamGuard guard(out);
  struct tm* date_info = localtime(&date);
  out << "\n=== Recovery Time Estimation ===\n";
  out << "  Date: " << std::put_time(date_info, "%Y-%m-%d") << "\n";
  out << "  Number of workouts: " << std::distance(range_start, range_end) << "\n";
  out << "  Total recovery points: " << total_recovery << "\n";
  out << "  Estimated recovery time: " << (total_recovery / 60) << " hours ";
  out << (total_recovery % 60) << " minutes\n\n";
  out << "  Recovery recommendations:\n";

  if (total_recovery < 1440) {
    out << "  - Light activity tomorrow\n";
  } else if (total_recovery < 2880) {
    out << "  - Rest day or cross-training\n" << "  - Focus on hydration\n";
  } else {
    int days = total_recovery / 1440;
    out << "  - Full rest for " << days << " days\n";
    out << "  - Protein-rich nutrition\n";
    out << "  - Consider massage\n";
  }
  if (total_recovery > 5000) {
    out << "  - Warning: Extreme load detected!\n";
    out << "  - Mandatory rest day\n";
  }
}

void dribas::predict_result(std::ostream& out, const suite& tren)
{
  auto prediction = predict_result(tren.at(1));
  out << prediction << '\n';
}

void dribas::analyze_training_segment(std::istream& in, std::ostream& out, const suite& trainings)
{
  size_t training_id;
  time_t start_time, end_time;
  if (!(in >> training_id >> start_time >> end_time)) {
    throw std::invalid_argument("Invalid arguments for analyze_training_segment");
  }
  if (start_time >= end_time) {
    throw std::invalid_argument("Start time must be before end time");
  }
  auto training_it = trainings.find(training_id);
  if (training_it == trainings.end()) {
    throw std::out_of_range("Training not found");
  }
  const auto& sessions = training_it->second;
  std::vector< std::pair< time_t, workout > > filtered_sessions;
  TimeRangeFilter filter(start_time, end_time);
  std::copy_if(sessions.begin(), sessions.end(), std::back_inserter(filtered_sessions), filter);
  if (filtered_sessions.empty()) {
    throw std::runtime_error("No data found in specified time segment");
  }
  std::vector< int > heart_rates;
  HeartRateExtractor hr_extractor;
  std::transform(filtered_sessions.begin(), filtered_sessions.end(), std::back_inserter(heart_rates), hr_extractor);
  std::vector< int > cadences;
  CadenceExtractor cad_extractor;
  std::transform(filtered_sessions.begin(), filtered_sessions.end(), std::back_inserter(cadences), cad_extractor);

  std::vector< double > distances;
  DistanceExtractor dist_extractor;
  std::transform(filtered_sessions.begin(), filtered_sessions.end(), std::back_inserter(distances), dist_extractor);

  double avg_heart = std::accumulate(heart_rates.begin(), heart_rates.end(), 0.0) / heart_rates.size();
  double avg_cadence = std::accumulate(cadences.begin(), cadences.end(), 0.0) / cadences.size();
  double avg_distance = std::accumulate(distances.begin(), distances.end(), 0.0) / distances.size();

  auto hr_minmax = std::minmax_element(heart_rates.begin(), heart_rates.end());
  auto cad_minmax = std::minmax_element(cadences.begin(), cadences.end());

  out << "Training Segment Analysis (ID: " << training_id << ")\n";
  out << "Time range: " << std::asctime(std::localtime(&start_time));
  out << " - " << std::asctime(std::localtime(&end_time));
  out << "Sessions analyzed: " << filtered_sessions.size() << "\n";
  out << "Average Heart Rate: " << std::fixed << std::setprecision(1) << avg_heart << " bpm\n";
  out << "Min/Max Heart Rate: " << *hr_minmax.first << "/" << *hr_minmax.second << " bpm\n";
  out << "Average Cadence: " << std::fixed << std::setprecision(1) << avg_cadence << " rpm\n";
  out << "Min/Max Cadence: " << *cad_minmax.first << "/" << *cad_minmax.second << " rpm\n";
  out << "Average Distance: " << std::fixed << std::setprecision(2) << avg_distance << " km\n";
}
void dribas::delete_training_by_key(std::istream& in, std::ostream& out, suite& trainings)
{
  unsigned int year, month, day, hour, minute;
  in >> year >> DelimiterI{'-'} >> month >> DelimiterI{'-'} >> day;
  in >> hour >> DelimiterI{':'} >> minute;
  if (!in) {
    throw std::invalid_argument("");
  }
  in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  time_t date_only_time_t;
  try {
    date_only_time_t = parse_date(year, month, day);
  } catch (const std::exception& e) {
    out << "Error date parsing " << e.what() << "\n";
    return;
  }
  std::tm tm_struct = *std::localtime(&date_only_time_t);
  if (hour > 23 || minute > 59) {
    throw std::invalid_argument("");
  }
  tm_struct.tm_hour = hour;
  tm_struct.tm_min = minute;
  tm_struct.tm_sec = 0;
  tm_struct.tm_isdst = -1;
  time_t key_time = std::mktime(&tm_struct);
  if (key_time == static_cast< time_t >(-1)) {
    throw std::runtime_error(".");
  }
  auto& main_suite = trainings[1];
  main_suite.erase(key_time);
}

void dribas::delete_training_suite(std::istream& in, std::ostream& out, suite& trainings)
{
  size_t training_id;
  in >> training_id;
  if (!in) {
    throw std::invalid_argument("Invalid training ID");
  }
  if (training_id == 1) {
    throw std::invalid_argument("Cannot delete the main training suite (ID 1)");
  }
  auto it = trainings.find(training_id);
  size_t removed_count = it->second.size();
  trainings.erase(it);
  out << "Deleted training suite ID " << training_id;
  out<< " containing " << removed_count << " workouts\n";
  out << "Note: The workouts still exist in the main training suite (ID 1)\n";
}
