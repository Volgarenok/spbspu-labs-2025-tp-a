#include "input_output_processing.hpp"

namespace
{
  bool is_equal_id(const std::string& rhs)
  {
    return rhs.size() == 7;
  }
}

void lanovenko::input_pantsir_status(std::istream& in, std::map< std::string, Pantsir >& pantsir_status_sets)
{
  std::string status_name;
  Pantsir current;
  if (!(in >> status_name) || !(in >> current ))
  {
    throw std::logic_error("< INPUT ERROR >\n");
  }
  pantsir_status_sets[status_name] = current;
}

void lanovenko::input_targets(std::istream& in, std::map< std::string, Target >& rhs)
{
  std::string id;
  while (!(in >> id).eof())
  {
    Target current;
    if (!is_equal_id(id) || !(std::cin >> current))
    {
      throw std::logic_error("< INPUT ERROR >\n");
    }
    if (rhs.find(id) != rhs.end())
    {
      throw std::logic_error("< TARGET ALREADY EXISTS\n");
    }
    rhs[id] = current;
  }
}
