#include "command_processing.hpp"
#include <iterator>

namespace
{
  bool is_equal_id(const std::string& rhs)
  {
    return rhs.size() == 7;
  }

  double get_type_factor(const std::string& type)
  {
    if (type == "MISSILE")
    {
      return 1.5;
    }
    if (type == "FIGHTER")
    {
      return 1.3;
    }
    if (type == "UAV")
    {
      return 1.1;
    }
    if (type == "DRONE")
    {
      return 1.0;
    }
    return 1.0;
  }

  double get_score(const lanovenko::Target& target)
  {
    const double norm_distance = std::min(target.distance / 25000.0, 1.0);
    const double norm_height = target.height / 10000.0;
    const double norm_speed = target.speed / 2000.0;

    double distance_score = 1.0 - norm_distance;
    double height_score = 1.0 - std::abs(0.5 - norm_height / 2.0);
    double speed_score = 1.0 - norm_speed;

    double type_weight = 0.4;
    double distance_weight = 0.3;
    double height_weight = 0.2;
    double speed_weight = 0.1;

    double type_score = get_type_factor(target.type);

    double score = (type_score * type_weight) + (distance_score * distance_weight);
    score += (height_score * height_weight) + (speed_score * speed_weight);
    return std::max(0.0, std::min(score, 1.0));
  }

  double get_cost(const lanovenko::Target& target)
  {
    double cost = 0.0;
    if (target.type == "MISSILE")
    {
      cost += 3.0;
    }
    else if (target.type == "DRONE")
    {
      cost += 1.0;
    }
    else if (target.type == "UAV")
    {
      cost += 1.5;
    }
    else
    {
      cost += 2.5;
    }
    cost += static_cast< double >(target.distance) / 10000.0;
    cost += std::abs(static_cast< double >(target.height) - 5000.0) / 5000.0;
    cost += static_cast< double >(target.speed) / 1000.0;
    return cost;
  }

  double get_threat_factor(const std::string& type)
  {
    if (type == "MISSILE")
    {
      return 1.0;
    }
    else if (type == "FIGHTER")
    {
      return 0.8;
    }
    else if (type == "UAV")
    {
      return 0.6;
    }
    else if (type == "DRONE")
    {
      return 0.4;
    }
    return 0.4;
  }

  double get_threat(const lanovenko::Target& target)
  {
    double type_threat = get_threat_factor(target.type);

    double distance_norm = std::min(target.distance / 50000.0, 1.0);
    double height_norm = target.height / 15000.0;
    double speed_norm = target.speed / 3000.0;

    double distance_factor = 1.0 - distance_norm;
    double height_factor = 1.0 - (height_norm / 2.0);
    double speed_factor = speed_norm;

    double type_weight = 0.5;
    double distance_weight = 0.3;
    double height_weight = 0.1;
    double speed_weight = 0.1;

    double threat = type_threat * type_weight + distance_factor * distance_weight;
    threat += height_factor * height_weight + speed_factor * speed_weight;
    return std::max(0.0, std::min(threat, 1.0));
  }

  bool cost_comparator(std::pair< std::string, lanovenko::Target >& l, std::pair< std::string, lanovenko::Target >& r)
  {
    double a = get_cost(l.second);
    double b = get_cost(r.second);
    return a < b;
  }

  bool score_comparator(std::pair< std::string, lanovenko::Target >& l, std::pair< std::string, lanovenko::Target >& r)
  {
    double a = get_score(l.second);
    double b = get_score(r.second);
    return a > b;
  }

  bool threat_comparator(std::pair< std::string, lanovenko::Target >& l, std::pair< std::string, lanovenko::Target >& r)
  {
    double a = get_threat(l.second);
    double b = get_threat(r.second);
    return a > b;
  }

  bool is_empty_pantsir(const lanovenko::Pantsir& rhs)
  {
    return rhs.ammunition < 200 || rhs.missiles == 0;
  }

  class TargetProcessor
  {
  public:
    TargetProcessor(lanovenko::Pantsir& p):
      pantsir(p)
    {};
    bool operator()(const std::pair< const std::string, lanovenko::Target >& rhs) const
    {
      size_t needed = static_cast< size_t >(std::round(get_cost(rhs.second)));
      if (pantsir.missiles >= needed && pantsir.barell_wear < 100)
      {
        pantsir.missiles -= needed;
        pantsir.ammunition -= needed;
        pantsir.barell_wear += needed * 2;
        pantsir.opening_hours += needed * 5;
        pantsir.targets_destroyed++;
        return true;
      }
      return false;
    }
  private:
    lanovenko::Pantsir& pantsir;
  };

  class ShotPrinter
  {
  public:
    ShotPrinter(std::ostream& out, size_t count, size_t total):
      os(out),
      shot_count(count),
      total_shots(total)
    {};
    std::string operator()()
    {
      std::string res = "    " + std::to_string(shot_count++) + ". ";
      res += (shot_count > total_shots ? "TARGET DESTROYED\n" : "MISS\n");
      return res;
    }
  private:
    std::ostream& os;
    size_t shot_count;
    size_t total_shots;
  };

  class AttackPrinter
  {
  public:
    AttackPrinter(std::ostream& out, size_t& cnt):
      os(out),
      counter(cnt)
    {};
    void operator()(const std::pair< const std::string, lanovenko::Target >& rhs) const
    {
      size_t missiles = static_cast< size_t >(std::round(get_cost(rhs.second)));
      os << counter++ << ". " << rhs.second.unique_code << " (";
      os << rhs.second.type << ") - " << missiles;
      os << (missiles > 1 ? " MISSILES:\n" : " MISSILE:\n");
      ShotPrinter printer(os, 1, missiles);
      std::generate_n(std::ostream_iterator< std::string >(os), missiles, printer);
    }
  private:
    std::ostream& os;
    size_t& counter;
  };

  std::string get_id(const std::pair< std::string, lanovenko::Target >& rhs)
  {
    return rhs.first;
  }

  struct EraseFromMap
  {
    void operator()(std::pair< const std::string, lanovenko::targets >& rhs) const
    {
      rhs.second.erase(id);
    }
    const std::string& id;
  };

  bool is_equal_type(const std::string& rhs)
  {
    return rhs == "UAV" || rhs == "MISSILE" || rhs == "DRONE" || rhs == "FIGHTER";
  }

  bool is_same_line(const std::pair< std::string, lanovenko::Target >& lhs, const std::string& rhs)
  {
    return lhs.second.type == rhs;
  }

  bool present_with_type(const std::map< std::string, lanovenko::Target >& rhs, const std::string& type)
  {
    size_t finded = 0;
    using namespace std::placeholders;
    auto equal_t = std::bind(is_same_line, _1, type);
    finded = std::count_if(rhs.begin(), rhs.end(), equal_t);
    return finded > 0 ? true : false;
  }

  void print_target(const std::pair< const std::string, lanovenko::Target >& rhs, std::ostream& out)
  {
    out << "TARGET: " << rhs.first << '\n';
    out << rhs.second;
  }

  void print_pantsir(const lanovenko::Pantsir& rhs, std::ostream& out)
  {
    out << "=== PANTSIR STATUS ===\n";
    out << "[OPERATIONAL DATA]\n";
    out << rhs;
    out << "[MAINTENANCE WARNINGS]\n";
    bool warnings_present = false;
    if (rhs.opening_hours > 2000)
    {
      out << "! Continuous operation exceeds 2000 hours - diagnostic required\n";
      warnings_present = true;
    }
    if (rhs.barell_wear > 70)
    {
      out << "! Cannon barrel wear: " << rhs.barell_wear << "\n";
      warnings_present = true;
    }
    if (!warnings_present)
    {
      out << "All systems nominal\n";
    }
  }

  void delete_targets(const std::string& id, lanovenko::targets& trg, lanovenko::targets_sets& trgs)
  {
    if (trg.find(id) == trg.end())
    {
      throw std::logic_error("< INCORRET ID >");
    }
    trg.erase(id);
    std::for_each(trgs.begin(), trgs.end(), EraseFromMap{ id });
  }
}

void lanovenko::create_target(std::istream& in, targets& trg)
{
  std::string id;
  if (!(in >> id) || !is_equal_id(id) || trg.find(id) != trg.end())
  {
    throw std::logic_error("< INCORRECT ID >");
  }
  Target current;
  if (!(in >> current))
  {
    throw std::logic_error("< WRONG INPUT >");
  }
  trg[id] = current;
}

void lanovenko::delete_target(std::istream& in, targets& trg, targets_sets& trgs)
{
  std::string id;
  in >> id;
  delete_targets(id, trg, trgs);
}

void lanovenko::engage_max_targets(std::istream& in, std::ostream& out, targets& trg, targets_sets& trgs, pantsir_s& ps)
{
  std::string current_status, current_name, new_status, new_name;
  in >> current_status >> current_name;
  if (ps.find(current_status) == ps.end() || trgs.find(current_name) == trgs.end())
  {
    throw std::logic_error("< INCORRECT PARAMETR >\n");
  }
  if (trgs[current_name].empty())
  {
    throw std::logic_error("< NO TARGETS >\n");
  }
  if (is_empty_pantsir(ps[current_status]))
  {
    throw std::logic_error("< EMPTY PANTSIR >\n");
  }
  in >> new_status >> new_name;
  ps[new_status] = ps[current_status];
  targets& new_targets = trgs[new_name] = trgs[current_name];
  Pantsir& new_pantsir = ps[new_status];

  std::vector< std::pair< std::string, Target > > targets_vec(new_targets.begin(), new_targets.end());
  std::sort(targets_vec.begin(), targets_vec.end(), cost_comparator);

  size_t target_counter = 1;
  TargetProcessor processor(new_pantsir);
  auto split_it = std::stable_partition(targets_vec.begin(), targets_vec.end(), processor);
  AttackPrinter printer(out, target_counter);
  std::for_each(targets_vec.begin(), split_it, printer);

  std::vector< std::pair< std::string, Target > > destroyed_targets(targets_vec.begin(), split_it);
  std::vector< std::string > destroyed_ids;
  using namespace::std::placeholders;
  auto to_erase = std::bind(delete_targets, _1, std::ref(trg), std::ref(trgs));
  std::transform(destroyed_targets.begin(), destroyed_targets.end(), std::back_inserter(destroyed_ids), get_id);
  std::for_each(destroyed_ids.begin(), destroyed_ids.end(), to_erase);
  trgs.erase(current_name);
}

void lanovenko::engage_balanced(std::istream& in, std::ostream& out, targets& trg, targets_sets& trgs, pantsir_s& ps)
{
  std::string current_status, current_name, new_status, new_name;
  in >> current_status >> current_name;
  if (ps.find(current_status) == ps.end() || trgs.find(current_name) == trgs.end())
  {
    throw std::logic_error("< INCORRECT PARAMETR >\n");
  }
  if (trgs[current_name].empty())
  {
    throw std::logic_error("< NO TARGETS >\n");
  }
  if (is_empty_pantsir(ps[current_status]))
  {
    throw std::logic_error("< EMPTY PANTSIR >\n");
  }
  in >> new_status >> new_name;
  ps[new_status] = ps[current_status];
  targets& new_targets = trgs[new_name] = trgs[current_name];
  Pantsir& new_pantsir = ps[new_status];

  std::vector< std::pair< std::string, Target > > targets_vec(new_targets.begin(), new_targets.end());
  std::sort(targets_vec.begin(), targets_vec.end(), score_comparator);

  size_t target_counter = 1;
  TargetProcessor processor(new_pantsir);
  auto split_it = std::stable_partition(targets_vec.begin(), targets_vec.end(), processor);
  AttackPrinter printer(out, target_counter);
  std::for_each(targets_vec.begin(), split_it, printer);

  std::vector< std::pair< std::string, Target > > destroyed_targets(targets_vec.begin(), split_it);
  std::vector< std::string > destroyed_ids;
  using namespace::std::placeholders;
  auto to_erase = std::bind(delete_targets, _1, std::ref(trg), std::ref(trgs));
  std::transform(destroyed_targets.begin(), destroyed_targets.end(), std::back_inserter(destroyed_ids), get_id);
  std::for_each(destroyed_ids.begin(), destroyed_ids.end(), to_erase);
  trgs.erase(current_name);
}

void lanovenko::engage_top_threats(std::istream& in, std::ostream& out, targets& trg, targets_sets& trgs, pantsir_s& ps)
{
  std::string current_status, current_name, new_status, new_name;
  in >> current_status >> current_name;
  if (ps.find(current_status) == ps.end() || trgs.find(current_name) == trgs.end())
  {
    throw std::logic_error("< INCORRECT PARAMETR >\n");
  }
  if (trgs[current_name].empty())
  {
    throw std::logic_error("< NO TARGETS >\n");
  }
  if (is_empty_pantsir(ps[current_status]))
  {
    throw std::logic_error("< EMPTY PANTSIR >\n");
  }
  in >> new_status >> new_name;
  ps[new_status] = ps[current_status];
  targets& new_targets = trgs[new_name] = trgs[current_name];
  Pantsir& new_pantsir = ps[new_status];

  std::vector< std::pair< std::string, Target > > targets_vec(new_targets.begin(), new_targets.end());
  std::sort(targets_vec.begin(), targets_vec.end(), threat_comparator);

  size_t target_counter = 1;
  TargetProcessor processor(new_pantsir);
  auto split_it = std::stable_partition(targets_vec.begin(), targets_vec.end(), processor);
  AttackPrinter printer(out, target_counter);
  std::for_each(targets_vec.begin(), split_it, printer);

  std::vector< std::pair< std::string, Target > > destroyed_targets(targets_vec.begin(), split_it);
  std::vector< std::string > destroyed_ids;
  using namespace::std::placeholders;
  auto to_erase = std::bind(delete_targets, _1, std::ref(trg), std::ref(trgs));
  std::transform(destroyed_targets.begin(), destroyed_targets.end(), std::back_inserter(destroyed_ids), get_id);
  std::for_each(destroyed_ids.begin(), destroyed_ids.end(), to_erase);
  trgs.erase(current_name);
}

void lanovenko::engage_manual(std::istream& in, std::ostream& out, targets& trg, targets_sets& trgs, pantsir_s& ps)
{
  std::string current_status, current_name, new_status, new_name, id;
  in >> current_status >> current_name;
  if (ps.find(current_status) == ps.end() || trgs.find(current_name) == trgs.end())
  {
    throw std::logic_error("< INCORRECT PARAMETR >\n");
  }
  if (trgs[current_name].empty())
  {
    throw std::logic_error("< NO TARGETS >\n");
  }
  if (is_empty_pantsir(ps[current_status]))
  {
    throw std::logic_error("< EMPTY PANTSIR >\n");
  }
  in >> new_status >> new_name;
  ps[new_status] = ps[current_status];
  targets& new_targets = trgs[new_name] = trgs[current_name];
  Pantsir& new_pantsir = ps[new_status];
  in >> id;
  auto finded = (new_targets.find(id));
  if (finded == new_targets.end())
  {
    throw std::logic_error("< INCORRET ID >");
  }
  std::vector< std::pair< std::string, Target > > targets_vec(new_targets.begin(), new_targets.end());
  using namespace std::placeholders;
  auto first_find = std::bind(is_same_line, _1, id);
  auto finded_vec = std::find_if(targets_vec.begin(), targets_vec.end(), first_find);
  size_t target_counter = 1;
  TargetProcessor processor(new_pantsir);
  processor(*finded_vec);
  AttackPrinter printer(out, target_counter);
  printer(*finded_vec);

  new_targets.erase(finded);
  delete_targets(id, trg, trgs);
  trgs.erase(current_name);
}

void lanovenko::solve_threat(std::istream& in, std::ostream& out, targets& trg, targets_sets& trgs, pantsir_s& ps)
{
  std::string current_status, current_name, new_status, new_name, type;
  in >> current_status >> current_name;
  if (ps.find(current_status) == ps.end() || trgs.find(current_name) == trgs.end())
  {
    throw std::logic_error("< INCORRECT PARAMETR >\n");
  }
  if (trgs[current_name].empty())
  {
    throw std::logic_error("< NO TARGETS >\n");
  }
  if (is_empty_pantsir(ps[current_status]))
  {
    throw std::logic_error("< EMPTY PANTSIR >\n");
  }
  in >> new_status >> new_name;
  ps[new_status] = ps[current_status];
  targets& new_targets = trgs[new_name] = trgs[current_name];
  Pantsir& new_pantsir = ps[new_status];
  in >> type;
  if (!is_equal_type(type) || !present_with_type(new_targets, type))
  {
    throw std::logic_error(" < INCORRECT TYPE >\n");
  }
  using namespace std::placeholders;
  auto type_equal_to = std::bind(is_same_line, _1, type);
  std::vector< std::pair< std::string, Target > > targets_vec(new_targets.begin(), new_targets.end());
  size_t target_counter = 1;
  TargetProcessor processor(new_pantsir);
  auto split_it = std::stable_partition(targets_vec.begin(), targets_vec.end(), processor);
  AttackPrinter printer(out, target_counter);
  std::for_each(targets_vec.begin(), split_it, printer);

  std::vector< std::pair< std::string, Target > > destroyed_targets(targets_vec.begin(), split_it);
  std::vector< std::string > destroyed_ids;
  using namespace::std::placeholders;
  auto to_erase = std::bind(delete_targets, _1, std::ref(trg), std::ref(trgs));
  std::transform(destroyed_targets.begin(), destroyed_targets.end(), std::back_inserter(destroyed_ids), get_id);
  std::for_each(destroyed_ids.begin(), destroyed_ids.end(), to_erase);
  trgs.erase(current_name);
}

void lanovenko::target_list(std::istream& in, std::ostream& out, const targets_sets& trgs)
{
  std::string name;
  if (!(in >> name) || trgs.find(name) == trgs.end())
  {
    throw std::logic_error("< INCORRECT NAME >");
  }
  using namespace std::placeholders;
  auto prt_target = std::bind(print_target, _1, std::ref(out));
  const targets& curr = trgs.at(name);
  std::for_each(curr.begin(), curr.end(), prt_target);
}

void lanovenko::all_targets(std::ostream &out, const targets &trs)
{
  using namespace std::placeholders;
  auto print_t = std::bind(print_target, _1, std::ref(out));
  std::for_each(trs.begin(), trs.end(), print_t);
}

void lanovenko::system_status(std::istream& in, std::ostream& out, const pantsir_s& ps)
{
  std::string status_name;
  if (!(in >> status_name) || ps.find(status_name) == ps.end())
  {
    throw std::logic_error("< INCORRECT NAME >");
  }
  print_pantsir(ps.at(status_name), out);
}

void lanovenko::create_target_set(std::istream& in, targets_sets& trgs)
{
  std::string name;
  if (!(in >> name) || trgs.find(name) != trgs.end())
  {
    throw std::logic_error("< INCORRECT NAME >");
  }
  trgs[name] = {};
}

void lanovenko::add_target_to_set(std::istream& in, targets_sets& trgs, targets& trg)
{
  std::string name, id;
  if (!(in >> name) || trgs.find(name) != trgs.end())
  {
    throw std::logic_error("< INCORRECT NAME >");
  }
  if (!(in >> id) || trg.find(id) != trg.end() || trgs[name].find(id) != trgs[name].end())
  {
    throw std::logic_error("< INCORRECT ID >");
  }
  Target current = trg.at(id);
  targets& set = trgs[name];
  set[id] = current;
}

void lanovenko::help(std::ostream &out)
{
  out << "create_target < ID > < TYPE > < DISTANCE > < HEIGHT > < SPEED > < UNIQUE CODE > - Создать цель\n";
  out << "create_targets < NAME > - Создать набор целей\n";
  out << "add_target_to_set < NAME > < ID > - Создать набор целей\n";
  out << "delete_target < ID > - Удалить цель\n";
  out << "delete_target_name < ID > < NAME > - Удалить цель из набора\n";
  out << "engage_max_targets < PANTSIR STATUS > < NAME_TARGETST > < NEW PANTSIR STATUS > < NEW_TARGETS > - ";
  out << "Обстрелять максимум целей\n";
  out << "engage_balanced < PANTSIR STATUS > < NAME TARGETST > < NEW PANTSIR STATUS > < NEW TARGETS > - ";
  out << "Сбалансированная атака целей\n";
  out << "engage_top_threats < PANTSIT STATUS > < NAME_TARGETST > < NEW PANTSIR STATUS > < NEW_TARGETS > - ";
  out << "Атака самых опасных целей\n";
  out << "engage_manual < PANTSIR STATUS > < NAME TARGETST > < NEW PANTSIR STATUS > < NEW TARGETS > < ID > - ";
  out << "Ручная атака цели\n";
  out << "solve_threat < PANTSIR STATUS > < NAME TARGETST > < NEW PANTSIR STATUS > < NEW TARGETS > < TYPE > - ";
  out << "Атака целей определенного типа\n";
  out << "system_status < PANTSIR STATUS NAME > - Вывести данные об установке\n";
  out << "target_list < TARGETS NAME > - показать весь список целей\n";
}
