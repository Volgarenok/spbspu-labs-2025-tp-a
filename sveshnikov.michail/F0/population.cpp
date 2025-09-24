#include "population.hpp"
#include <functional>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <vector>

namespace
{
  int calcFitness(int total, const std::pair< std::string, sveshnikov::Individual > &p)
  {
    return total + p.second.calc_fitness();
  }

  int calcAge(int total_age, const std::pair< std::string, sveshnikov::Individual > &p)
  {
    return total_age + p.second.get_age();
  }

  bool compareFitness(const std::pair< std::string, sveshnikov::Individual > &a,
      const std::pair< std::string, sveshnikov::Individual > &b)
  {
    return a.second.calc_fitness() < b.second.calc_fitness();
  }

  bool compareAges(const std::pair< std::string, sveshnikov::Individual > &a,
      const std::pair< std::string, sveshnikov::Individual > &b)
  {
    return a.second.get_age() < b.second.get_age();
  }

  struct AgeIncreaser
  {
    size_t years_;

    void operator()(std::pair< const std::string, sveshnikov::Individual > &p) const
    {
      p.second.make_older(years_);
    }
  };

  struct IndividualWithTag
  {
    const sveshnikov::Individual &individ;
    std::string tag;
  };

  std::ostream &operator<<(std::ostream &out, const IndividualWithTag &item)
  {
    return out << item.individ << " " << item.tag;
  }

  struct AddTag
  {
    std::string tag;

    IndividualWithTag operator()(const std::pair< std::string, sveshnikov::Individual > &p) const
    {
      return {p.second, tag};
    }
  };

  struct IndividualWithFitnessAndTag
  {
    const sveshnikov::Individual &individ;
    std::string tag;
  };

  std::ostream &operator<<(std::ostream &out, const IndividualWithFitnessAndTag &item)
  {
    return out << item.individ << " " << item.individ.calc_fitness() << " " << item.tag;
  }

  struct AddTagAndFitness
  {
    std::string tag;

    IndividualWithFitnessAndTag operator()(
        const std::pair< std::string, sveshnikov::Individual > &p) const
    {
      AddTag adder{tag};
      IndividualWithTag ind_with_tag = adder(p);
      return {ind_with_tag.individ, tag};
    }
  };

  struct IsNotInPopulation
  {
    const std::map< std::string, sveshnikov::Individual > &population_;
    const std::map< std::string, sveshnikov::Individual > &cemetery_;

    bool operator()(const std::pair< std::string, sveshnikov::Individual > &p) const
    {
      bool isNotAlive = population_.find(p.first) == population_.end();
      bool isNotInCemetery = cemetery_.find(p.first) == cemetery_.end();
      return isNotAlive && isNotInCemetery;
    }
  };
}

void sveshnikov::Population::add(const Individual &individ)
{
  const std::string &name = individ.get_name();
  if (population_.find(name) != population_.cend() || cemetery_.find(name) != cemetery_.cend())
  {
    throw std::invalid_argument("ERROR: this name is already taken!");
  }
  population_[name] = individ;
}

void sveshnikov::Population::remove(const std::string &name)
{
  auto it = population_.find(name);
  if (it != population_.end())
  {
    cemetery_.insert(std::move(*it));
    population_.erase(it);
  }
}

void sveshnikov::Population::mutate(const std::string &name, const Genotype &genotype)
{
  auto it = population_.find(name);
  if (it != population_.end())
  {
    it->second.mutate(genotype);
  }
  else
  {
    throw std::invalid_argument("ERROR: individual not found in population!");
  }
}

int sveshnikov::Population::calc_fitness(const std::string &name) const
{
  return get_individual(name).calc_fitness();
}

void sveshnikov::Population::printPedigreeLine(const std::string &childName,
    const Individual &child, const std::string &parentName, std::ostream &out) const
{
  out << childName << ' ' << child.get_genotype() << " -> ";

  auto individ = population_.find(parentName);
  if (individ == population_.end())
  {
    individ = cemetery_.find(parentName);
    if (individ == cemetery_.end())
    {
      out << "<NO SUCH INDIVIDUAL>\n";
      return;
    }
  }
  out << parentName << ' ' << individ->second.get_genotype() << '\n';
}

void sveshnikov::Population::print_pedigree(const std::string &name, std::ostream &out) const
{
  auto individ = population_.find(name);
  if (individ == population_.end())
  {
    individ = cemetery_.find(name);
    if (individ == cemetery_.end())
    {
      throw std::invalid_argument("ERROR: individual not found!");
    }
  }

  const auto &parents = individ->second.get_parents();
  if (!parents.first.empty() && !parents.second.empty())
  {
    printPedigreeLine(name, individ->second, parents.first, out);
    printPedigreeLine(name, individ->second, parents.second, out);
    print_pedigree(parents.first, out);
    print_pedigree(parents.second, out);
  }
}

void sveshnikov::Population::make_older(size_t years)
{
  std::for_each(population_.begin(), population_.end(), AgeIncreaser{years});
}

void sveshnikov::Population::crossover(const std::string &name, const std::string &parent1,
    const std::string &parent2)
{
  const Individual &p1 = get_individual(parent1);
  const Individual &p2 = get_individual(parent2);
  Individual child = p1.crossover(name, p2);
  add(child);
}

void sveshnikov::Population::print_list(std::ostream &out, const std::string &life_specifier) const
{
  using out_iter = std::ostream_iterator< IndividualWithFitnessAndTag >;
  if (life_specifier != "<ALIVE>" && life_specifier != "<DIED>" && life_specifier != "<ALL>")
  {
    throw std::invalid_argument("ERROR: unknown life specifier!");
  }
  if (life_specifier == "<ALIVE>" || life_specifier == "<ALL>")
  {
    auto p_begin = population_.begin();
    auto p_end = population_.end();
    std::transform(p_begin, p_end, out_iter(out, "\n"), AddTagAndFitness{"<ALIVE>"});
  }
  if (life_specifier == "<DIED>" || life_specifier == "<ALL>")
  {
    auto c_begin = cemetery_.begin();
    auto c_end = cemetery_.end();
    std::transform(c_begin, c_end, out_iter(out, "\n"), AddTagAndFitness{"<DIED>"});
  }
}

size_t sveshnikov::Population::get_size() const noexcept
{
  return population_.size();
}

const sveshnikov::Individual &sveshnikov::Population::get_individual(const std::string &name) const
{
  auto it = population_.find(name);
  if (it == population_.cend())
  {
    throw std::invalid_argument("ERROR: individual not found!");
  }
  return it->second;
}

int sveshnikov::Population::get_average_fitness() const
{
  if (population_.empty())
  {
    return 0;
  }
  int total = std::accumulate(population_.begin(), population_.end(), 0, calcFitness);
  return total / population_.size();
}

int sveshnikov::Population::get_min_fitness() const
{
  if (population_.empty())
  {
    return 0;
  }
  auto it = std::min_element(population_.begin(), population_.end(), compareFitness);
  return it->second.calc_fitness();
}

int sveshnikov::Population::get_max_fitness() const
{
  if (population_.empty())
  {
    return 0;
  }
  auto it = std::max_element(population_.begin(), population_.end(), compareFitness);
  return it->second.calc_fitness();
}

int sveshnikov::Population::get_average_age() const
{
  if (population_.empty())
  {
    return 0;
  }
  int total = std::accumulate(population_.begin(), population_.end(), 0, calcAge);
  return total / population_.size();
}

int sveshnikov::Population::get_min_age() const
{
  if (population_.empty())
  {
    return 0;
  }
  auto it = std::min_element(population_.begin(), population_.end(), compareAges);
  return it->second.get_age();
}

int sveshnikov::Population::get_max_age() const
{
  if (population_.empty())
  {
    return 0;
  }
  auto it = std::max_element(population_.begin(), population_.end(), compareAges);
  return it->second.get_age();
}

void sveshnikov::Population::select(int survival_threshold)
{
  auto it = population_.begin();
  while (it != population_.end())
  {
    if (it->second.calc_fitness() < survival_threshold)
    {
      cemetery_.insert(std::move(*it));
      it = population_.erase(it);
    }
    else
    {
      it++;
    }
  }
}

void sveshnikov::Population::unite(const Population &other)
{
  IsNotInPopulation pred{population_, cemetery_};
  auto begin_p = other.population_.begin();
  auto end_p = other.population_.end();
  std::copy_if(begin_p, end_p, std::inserter(population_, population_.end()), pred);
  if (!other.population_.empty())
  {
    auto begin_c = other.cemetery_.begin();
    auto end_c = other.cemetery_.end();
    std::copy_if(begin_c, end_c, std::inserter(cemetery_, cemetery_.end()), pred);
  }
}

std::istream &sveshnikov::operator>>(std::istream &in, Population &p)
{
  size_t size = 0;
  in >> size;
  for (size_t i = 0; i < size; i++)
  {
    Individual ind;
    std::string life_specifier;
    in >> ind >> life_specifier;
    if (life_specifier == "<ALIVE>")
    {
      p.add(ind);
    }
    else if (life_specifier == "<DIED>")
    {
      p.add(ind);
      p.remove(ind.get_name());
    }
    else
    {
      in.setstate(std::ios::failbit);
      return in;
    }
  }
  return in;
}

std::ostream &sveshnikov::operator<<(std::ostream &out, const Population &p)
{
  using out_iter = std::ostream_iterator< IndividualWithTag >;
  auto p_begin = p.population_.begin();
  auto p_end = p.population_.end();
  std::transform(p_begin, p_end, out_iter(out, "\n"), AddTag{"<ALIVE>"});
  auto c_begin = p.cemetery_.begin();
  auto c_end = p.cemetery_.end();
  std::transform(c_begin, c_end, out_iter(out, "\n"), AddTag{"<DIED>"});
  return out;
}
