#include "programme-options.hpp"
#include <functional>
#include <algorithm>
#include <fstream>

namespace
{
  struct IndividualRecord
  {
    sveshnikov::Individual individual;
    std::string tag;
    std::string population_name;
  };

  struct IndividualParser
  {
    std::istream &in;
    IndividualRecord &data;
    const std::string &p_name;

    bool operator()()
    {
      sveshnikov::Individual ind;
      if (!(in >> ind))
      {
        return false;
      }

      std::string tag;
      if (!(in >> tag) || (tag != "<ALIVE>" && tag != "<DIED>"))
      {
        in.setstate(std::ios::failbit);
        return false;
      }

      data.individual = ind;
      data.tag = tag;
      data.population_name = p_name;
      return true;
    }
  };

  struct ParentageValidator
  {
    const std::map< std::string, IndividualRecord > &individuals;

    bool operator()(const std::pair< std::string, IndividualRecord > &item) const
    {
      const sveshnikov::Individual &child = item.second.individual;
      const auto &parents = child.get_parents();

      if (parents.first.empty() || parents.second.empty())
      {
        return true;
      }

      auto par1_it = individuals.find(parents.first);
      auto par2_it = individuals.find(parents.second);
      if (par1_it == individuals.end() || par2_it == individuals.end())
      {
        return false;
      }

      const auto &par1 = par1_it->second.individual;
      const auto &par2 = par2_it->second.individual;
      if (child.get_age() > par1.get_age() || child.get_age() > par2.get_age())
      {
        return false;
      }

      try
      {
        sveshnikov::Genotype expected_genotype = par1.get_genotype().crossover(par2.get_genotype());
        if (expected_genotype != child.get_genotype())
        {
          return false;
        }
      }
      catch (const std::exception &)
      {
        return false;
      }
      return true;
    }
  };

  struct IsNumber
  {
    bool operator()(const std::string &s) const
    {
      if (s.empty())
      {
        return false;
      }
      using namespace std::placeholders;
      auto begin = s.begin();
      auto end = s.end();
      return std::all_of(begin, end, std::bind(static_cast< int (*)(int) >(std::isdigit), _1));
    }
  };
}

bool sveshnikov::check_format(std::istream &in)
{
  std::map< std::string, IndividualRecord > individuals;
  std::string population_name;
  size_t population_size;

  while (in >> population_name >> population_size)
  {
    for (size_t i = 0; i < population_size; ++i)
    {
      IndividualRecord data;
      IndividualParser parser{in, data, population_name};
      if (!parser())
      {
        return false;
      }
      individuals[data.individual.get_name()] = data;
    }
  }

  ParentageValidator validator{individuals};
  return std::all_of(individuals.begin(), individuals.end(), validator);
}

void sveshnikov::print_help_manual(std::ostream &out)
{
  out << "GENETIC ECOSYSTEM SIMULATOR - HELP MANUAL\n";
  out << "==========================================\n\n";

  out << "This program simulates evolutionary processes in populations where each\n";
  out << "individual is characterized by a genetic code. It models inheritance,\n";
  out << "mutations, natural selection, and genetic recombination.\n\n";

  out << "GENOTYPE FORMAT:\n";
  out << "- 12 nucleotides (4 genes of 3 symbols each)\n";
  out << "- Valid symbols: A (adenine), T (thymine), C (cytosine), G (guanine)\n";
  out << "- A pairs only with T, G pairs only with C within a gene\n";
  out << "- Count(A) must equal Count(T), Count(G) must equal Count(C)\n";
  out << "- Enclosed in angle brackets: <ATC GCT ATA GCG>\n\n";

  out << "COMMAND REFERENCE:\n";
  out << "==================\n\n";

  out << "1. ADD_POPULATION <POPULATION_NAME>\n";
  out << "   - Create a new population\n";
  out << "   - Error if population already exists: <THE POPULATION IS ALREADY EXIST>\n\n";

  out << "2. REMOVE_POPULATION <POPULATION_NAME>\n";
  out << "   - Remove the specified population\n\n";

  out << "3. ADD <POPULATION_NAME> <NAME> <GENOTYPE> [AGE]\n";
  out << "   - Add a new individual to the population\n";
  out << "   - Age is optional (default: 0)\n";
  out << "   - Error if individual already exists: <THE INDIVIDUAL IS ALREADY IN POPULATION>\n";
  out << "   - Example: ADD Zheka <ATC GCT ATA GCG> 48\n\n";

  out << "4. REMOVE <POPULATION_NAME> <NAME>\n";
  out << "   - Remove the specified individual from the population\n\n";

  out << "5. MUTATE <POPULATION_NAME> <NAME> <NEW_GENOTYPE>\n";
  out << "   - Change the individual's genotype\n\n";

  out << "6. FITNESS <POPULATION_NAME> <NAME>\n";
  out << "   - Calculate and display the individual's fitness index\n";
  out << "   - Calculation: base + epistasis + dominance\n";
  out << "     - base = sum of all gene values (A=0, C=1, G=2, T=3)\n";
  out << "     - epistasis = (gene1 * gene2) - (gene3 * gene4)\n";
  out << "     - dominance = gene2 - gene3\n\n";

  out << "7. PEDIGREE <POPULATION_NAME> <NAME>\n";
  out << "   - Display the individual's pedigree as an adjacency list\n";
  out << "   - Format: <name> <genotype> -> <parent_name> <parent_genotype>\n";
  out << "   - Missing parents shown as: <NO SUCH INDIVIDUAL>\n\n";

  out << "8. CROSSOVER <POPULATION_NAME> <NEW_NAME> <NAME1> <NAME2>\n";
  out << "   - Create a new individual by combining parents' genotypes\n";
  out << "   - Age set to 0\n";
  out << "   - Error if no valid combination: <INCOMPATIBLE GENOTYPES>\n\n";

  out << "9. LIST <POPULATION_NAME> <ALIVE|DIED|ALL>\n";
  out << "   - Display individuals list\n";
  out << "   - ALIVE: currently living, DIED: deceased, ALL: all individuals\n";
  out << "   - Format: <name> <age> <genotype> <parent1> <parent2> <fitness> <status>\n\n";

  out << "10. STATS <POPULATION_NAME>\n";
  out << "    - Show population statistics:\n";
  out << "      - Size, average/min/max fitness, average/min/max age\n\n";

  out << "11. SAVE <POPULATION_NAME> <FILE_NAME>\n";
  out << "    - Save population to file\n";
  out << "    - Error if file not found: <NON-EXISTENT FILE>\n\n";

  out << "12. SELECT <POPULATION_NAME> <SURVIVAL_THRESHOLD>\n";
  out << "    - Remove individuals with fitness below threshold\n\n";

  out << "13. YEARS <POPULATION_NAME> <N>\n";
  out << "    - Increase all individuals' age by N years\n\n";

  out << "14. UNITE <NEW_POPULATION_NAME> <POPULATION_NAME1> <POPULATION_NAME2>\n";
  out << "    - Merge two populations into a new one\n";
  out << "    - Duplicate names use values from first population\n\n";

  out << "15. LIST_POPULATION\n";
  out << "    - Display all population names\n\n";

  out << "GENERAL ERROR MESSAGES:\n";
  out << "- <NO SUCH POPULATION>: Population not found\n";
  out << "- <NO SUCH INDIVIDUAL>: Individual not found\n";
  out << "- <INVALID COMMAND>: Unknown command or incorrect format\n\n";

  out << "PROGRAM USAGE:\n";
  out << "- Start with population file: " << '\n';
  out << "  ./program [population_file]" << '\n';
  out << "- Check population file: --check" << '\n';
  out << "- Show help: --help" << '\n';
}
