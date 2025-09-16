#include "individual.hpp"
#include <functional>
#include <algorithm>
#include <stdexcept>

sveshnikov::Individual::Individual(const std::string &name, const Genotype &genes, size_t age = 0):
  name_(name),
  genotype_(genes),
  age_(age),
  parents_("", "")
{}

sveshnikov::Individual::Individual(const std::string &name, const Genotype &genotype,
    const std::string &parent1, const std::string &parent2):
  name_(name),
  genotype_(genotype),
  age_(0),
  parents_(parent1, parent2)
{}

bool sveshnikov::Individual::operator==(const Individual &other) const noexcept
{
  return name_ == other.name_;
}

bool sveshnikov::Individual::operator!=(const Individual &other) const noexcept
{
  return !(*this == other);
}

const std::string &sveshnikov::Individual::get_name() const noexcept
{
  return name_;
}

const sveshnikov::Genotype &sveshnikov::Individual::get_genotype() const noexcept
{
  return genotype_;
}

size_t sveshnikov::Individual::get_age() const noexcept
{
  return age_;
}

const std::pair< std::string, std::string > &sveshnikov::Individual::get_parents() const
{
  return parents_;
}

void sveshnikov::Individual::make_older(size_t years) noexcept
{
  age_ += years;
}

void sveshnikov::Individual::mutate(const Genotype &genotype)
{
  genotype_ = genotype;
}

int sveshnikov::Individual::calc_fitness() const
{
  return genotype_.calc_fitness();
}

sveshnikov::Individual sveshnikov::Individual::crossover(const std::string &name,
    const Individual &other) const
{
  Genotype new_genotype = genotype_.crossover(other.genotype_);
  return Individual(name, new_genotype, name_, other.name_);
}

std::istream &sveshnikov::operator>>(std::istream &in, Individual &individual)
{
  std::string name;
  Genotype genotype;
  size_t age = 0;

  in >> name >> genotype;
  if (in.peek() != std::char_traits< char >::eof())
  {
    in >> age;
  }
  individual = Individual(name, genotype, age);
  return in;
}

std::ostream &sveshnikov::operator<<(std::ostream &out, const Individual &individual)
{
  out << individual.get_name();
  out << " " << individual.calc_fitness();
  out << " " << individual.get_age();
  out << " " << individual.get_genotype();
  return out;
}
