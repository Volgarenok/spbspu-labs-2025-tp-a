#include "genotype.hpp"
#include <cmath>
#include <numeric>
#include <iterator>
#include <algorithm>
#include <functional>
#include <stream-guard.hpp>
#include <delimeter-io.hpp>

namespace
{
  size_t countNucleotide(const std::vector< sveshnikov::Gene > &genes, char nucleotide)
  {
    size_t count = 0;
    for (size_t i = 0; i < 4; i++)
    {
      const std::string &nuc = genes[i].get_nucleotides();
      count += std::count(nuc.begin(), nuc.end(), nucleotide);
    }
    return count;
  }

  bool checkGenotypeComplementarity(const std::vector< sveshnikov::Gene > &genes)
  {
    std::string nucleotides = "ATCG";
    std::vector< size_t > counts;
    counts.reserve(4);
    using namespace std::placeholders;
    auto count_nucl = std::bind(countNucleotide, std::cref(genes), _1);
    std::transform(nucleotides.begin(), nucleotides.end(), counts.begin(), count_nucl);
    return (counts[0] == counts[1] && counts[2] == counts[3]);
  }
}

sveshnikov::Genotype::Genotype(const std::vector< Gene > &genes):
  genes_(genes)
{
  if (genes_.size() != 4)
  {
    throw std::invalid_argument("ERROR: genotype must contain exactly 4 genes!");
  }
  if (!checkGenotypeComplementarity(genes_))
  {
    throw std::invalid_argument("ERROR: genotype is not complementary!");
  }
}

bool sveshnikov::Genotype::operator==(const Genotype &other) const noexcept
{
  return std::equal(genes_.begin(), genes_.end(), other.genes_.begin());
}

bool sveshnikov::Genotype::operator!=(const Genotype &other) const noexcept
{
  return !(*this == other);
}

const std::vector< sveshnikov::Gene > &sveshnikov::Genotype::get_genes() const noexcept
{
  return genes_;
}

int sveshnikov::Genotype::calc_fitness() const
{
  using namespace std::placeholders;
  std::vector< int > gene_vals;
  gene_vals.reserve(4);
  auto calc_gene_fitness = std::bind(&Gene::calc_fitness, _1);
  std::transform(genes_.begin(), genes_.end(), std::back_inserter(gene_vals), calc_gene_fitness);

  int base = std::accumulate(gene_vals.begin(), gene_vals.end(), 0);
  int epistasis = gene_vals[0] * gene_vals[1] - gene_vals[2] * gene_vals[3];
  int dominance = gene_vals[1] - gene_vals[2];

  return base + epistasis + dominance;
}

sveshnikov::Genotype sveshnikov::Genotype::crossover(const Genotype &other) const
{
  std::vector< Gene > combinations[14];
  for (size_t i = 1; i < 15; ++i)
  {
    combinations[i - 1].reserve(4);
    size_t k = 1;
    for (size_t j = 0; j < 4; j++)
    {
      combinations[i - 1].push_back((i & k) ? other.genes_[j] : genes_[j]);
      k *= 2;
    }
    if (checkGenotypeComplementarity(combinations[i - 1]))
    {
      return Genotype(combinations[i - 1]);
    }
  }
  throw std::runtime_error("ERROR: genotypes are incompatible!");
}

std::istream &sveshnikov::operator>>(std::istream &in, Genotype &genotype)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  std::vector< Gene > genes;
  genes.reserve(4);
  Gene g;
  in >> DelimiterIO{'<'};
  in >> std::noskipws >> g;
  genes.push_back(g);
  in >> std::skipws;
  using in_iter = std::istream_iterator< Gene >;
  std::copy_n(in_iter(in), 3, std::back_inserter(genes));
  in >> std::noskipws >> DelimiterIO{'>'} >> std::skipws;

  if (in)
  {
    genotype = Genotype(genes);
  }
  return in;
}

std::ostream &sveshnikov::operator<<(std::ostream &out, const Genotype &genotype)
{
  std::ostream::sentry sentry(out);
  const std::vector< sveshnikov::Gene > &genes = genotype.get_genes();
  if (!sentry || genes.empty())
  {
    return out;
  }
  StreamGuard guard(out);

  using out_iter = std::ostream_iterator< Gene >;
  out << '<';
  std::copy(genes.begin(), std::prev(genes.end()), out_iter(out, " "));
  out << genes.back() << '>';
  return out;
}
