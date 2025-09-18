#include "commands.hpp"
#include <functional>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <limits>

void sveshnikov::addPopulation(populations_t &populations, std::istream &in)
{
  std::string name;
  in >> name;
  if (populations.find(name) != populations.end())
  {
    std::cout << "<THE POPULATION IS ALREADY EXIST>\n";
    return;
  }
  Population p;
  populations.insert({name, p});
}

void sveshnikov::removePopulation(populations_t &populations, std::istream &in)
{
  std::string name;
  in >> name;
  if (populations.find(name) == populations.end())
  {
    std::cout << "<NO SUCH POPULATION>\n";
    return;
  }
  populations.erase(name);
}

void sveshnikov::add(populations_t &populations, std::istream &in)
{
  std::string population_name, name;
  Genotype genotype;
  in >> population_name >> name >> genotype;
  if (populations.find(population_name) == populations.end())
  {
    std::cout << "<NO SUCH POPULATION>\n";
    return;
  }
  size_t age = 0;
  if (in.peek() != std::char_traits< char >::eof())
  {
    in >> age;
  }

  try
  {
    Individual individual(name, genotype, age);
    populations[population_name].add(individual);
  }
  catch (const std::invalid_argument &)
  {
    std::cout << "<THE INDIVIDUAL IS ALREADY IN POPULATION>\n";
  }
}

void sveshnikov::removeInds(populations_t &populations, std::istream &in)
{
  std::string population_name, name;
  in >> population_name >> name;
  if (populations.find(population_name) == populations.end())
  {
    std::cout << "<NO SUCH POPULATION>\n";
    return;
  }
  populations[population_name].remove(name);
}

void sveshnikov::mutate(populations_t &populations, std::istream &in)
{
  std::string population_name, name;
  Genotype genotype;
  in >> population_name >> name >> genotype;
  if (populations.find(population_name) == populations.end())
  {
    std::cout << "<NO SUCH POPULATION>\n";
    return;
  }

  try
  {
    populations[population_name].mutate(name, genotype);
  }
  catch (const std::invalid_argument &e)
  {
    std::cout << "<NO SUCH INDIVIDUAL>\n";
    return;
  }
}

void sveshnikov::calcFitness(const populations_t &populations, std::istream &in, std::ostream &out)
{
  std::string population_name, name;
  in >> population_name >> name;
  if (populations.find(population_name) == populations.end())
  {
    out << "<NO SUCH POPULATION>\n";
    return;
  }

  try
  {
    out << populations.find(population_name)->second.calc_fitness(name);
  }
  catch (const std::invalid_argument &e)
  {
    std::cout << "<NO SUCH INDIVIDUAL>\n";
    return;
  }
}

void sveshnikov::printPedigree(const populations_t &populations, std::istream &in,
    std::ostream &out)
{
  std::string population_name, name;
  in >> population_name >> name;
  if (populations.find(population_name) == populations.end())
  {
    out << "<NO SUCH POPULATION>\n";
    return;
  }

  try
  {
    populations.find(population_name)->second.print_pedigree(name, out);
  }
  catch (const std::invalid_argument &e)
  {
    std::cout << "<NO SUCH INDIVIDUAL>\n";
    return;
  }
}

void sveshnikov::crossover(populations_t &populations, std::istream &in)
{
  std::string population_name, new_name, name1, name2;
  in >> population_name >> new_name >> name1 >> name2;
  if (populations.find(population_name) == populations.end())
  {
    std::cout << "<NO SUCH POPULATION>\n";
    return;
  }

  try
  {
    populations[population_name].crossover(new_name, name1, name2);
  }
  catch (const std::invalid_argument &e)
  {
    std::cout << "<NO SUCH INDIVIDUAL>\n";
    return;
  }
  catch (const std::runtime_error &)
  {
    std::cout << "<INCOMPATIBLE GENOTYPES>\n";
  }
}

void sveshnikov::printList(const populations_t &populations, std::istream &in, std::ostream &out)
{
  std::string population_name, specifier;
  in >> population_name >> specifier;
  if (populations.find(population_name) == populations.end())
  {
    out << "<NO SUCH POPULATION>\n";
    return;
  }

  populations.find(population_name)->second.print_list(out, specifier);
}

void sveshnikov::printStats(const populations_t &populations, std::istream &in, std::ostream &out)
{
  std::string population_name;
  in >> population_name;
  if (populations.find(population_name) == populations.end())
  {
    out << "<NO SUCH POPULATION>\n";
    return;
  }
  const Population &p = populations.find(population_name)->second;

  out << "population size: " << p.get_size() << "\n";
  out << "average value of fitness: " << p.get_average_fitness() << "\n";
  out << "min_fitness: " << p.get_min_fitness() << "\n";
  out << "max_fitness: " << p.get_max_fitness() << "\n";
  out << "average value of age: " << p.get_average_age() << "\n";
  out << "min_age: " << p.get_min_age() << "\n";
  out << "max_age: " << p.get_max_age() << "\n";
}

void sveshnikov::selectInds(populations_t &populations, std::istream &in)
{
  std::string population_name;
  int threshold;
  in >> population_name >> threshold;
  if (populations.find(population_name) == populations.end())
  {
    std::cout << "<NO SUCH POPULATION>\n";
    return;
  }
  populations[population_name].select(threshold);
}

void sveshnikov::makeOlder(populations_t &populations, std::istream &in)
{
  std::string population_name;
  size_t years;
  in >> population_name >> years;
  if (populations.find(population_name) == populations.end())
  {
    std::cout << "<NO SUCH POPULATION>\n";
    return;
  }
  populations[population_name].make_older(years);
}

void sveshnikov::unite(populations_t &populations, std::istream &in)
{
  std::string p_name1, p_name2;
  in >> p_name1 >> p_name2;
  auto end = populations.end();
  if ((populations.find(p_name1) == end) || (populations.find(p_name2) == end))
  {
    std::cout << "<NO SUCH POPULATION>\n";
    return;
  }
  populations[p_name1].unite(populations[p_name2]);
}

void sveshnikov::listPopulation(const populations_t &populations, std::ostream &out)
{
  using namespace std::placeholders;
  using out_iter = std::ostream_iterator< std::string >;
  std::transform(populations.begin(), populations.end(), out_iter(out, "\n"),
      std::bind(&populations_t::value_type::first, _1));
}

void sveshnikov::save(const populations_t &populations, std::istream &in, std::ostream &out)
{
  std::string population_name, filename;
  in >> population_name >> filename;
  auto p = populations.find(population_name);
  if (p == populations.end())
  {
    out << "<NO SUCH POPULATION>\n";
    return;
  }
  std::ofstream file(filename);
  if (!file)
  {
    throw std::invalid_argument("<NON-EXISTENT FILE>\n");
  }
  file << population_name << ' ';
  file << p->second.get_size() << '\n';
  p->second.print_list(file, "<ALL>");
}

void sveshnikov::load(populations_t &populations, std::istream &in)
{
  populations.clear();

  while (!in.eof())
  {
    if (in.fail())
    {
      in.clear(in.rdstate() ^ std::ios::failbit);
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    std::string population_name;
    Population p;
    in >> population_name >> p;
    populations.insert({population_name, p});
  }
}
