#include "gene.hpp"
#include <array>
#include <cassert>
#include <algorithm>
#include <functional>
#include <stream-guard.hpp>

namespace
{
  bool checkNucleotideValidation(char n)
  {
    return n == 'A' || n == 'T' || n == 'C' || n == 'G';
  }

  bool checkPair(char a, char b)
  {
    if ((a == 'A' && b != 'T') || (a == 'G' && b != 'C'))
    {
      return false;
    }
    if ((b == 'A' && a != 'T') || (b == 'G' && a != 'C'))
    {
      return false;
    }
    return true;
  }

  bool checkComplementarity(const std::string &gene)
  {
    if (gene.size() != 3)
    {
      return false;
    }
    return checkPair(gene[0], gene[1]) && checkPair(gene[1], gene[2]);
  }

  size_t getNucleotideValue(char n)
  {
    switch (n)
    {
    case 'A':
      return 0;
    case 'C':
      return 1;
    case 'G':
      return 2;
    case 'T':
      return 3;
    }
    throw std::invalid_argument("ERROR: unknown nucleotide!");
  }
}

sveshnikov::Gene::Gene(const std::string &nucleotides):
  nucleotides_(nucleotides)
{
  if (nucleotides_.size() != 3)
  {
    throw std::invalid_argument("ERROR: gene must be 3 characters long!");
  }

  if (!std::all_of(nucleotides_.begin(), nucleotides_.end(), checkNucleotideValidation))
  {
    throw std::invalid_argument("ERROR: invalid nucleotide!");
  }

  if (!checkComplementarity(nucleotides_))
  {
    throw std::invalid_argument("ERROR: gene is not complementary!");
  }
}

bool sveshnikov::Gene::operator==(const Gene &other) const
{
  return nucleotides_ == other.nucleotides_;
}

bool sveshnikov::Gene::operator!=(const Gene &other) const
{
  return !(*this == other);
}

const std::string &sveshnikov::Gene::get_nucleotides() const noexcept
{
  assert(!nucleotides_.empty());
  return nucleotides_;
}

int sveshnikov::Gene::calc_fitness() const
{
  if (nucleotides_.empty())
  {
    return 0;
  }
  using namespace std::placeholders;
  std::array< size_t, 3 > vals;
  auto get_nucl_val = std::bind(getNucleotideValue, _1);
  std::transform(nucleotides_.begin(), nucleotides_.end(), vals.begin(), get_nucl_val);
  return vals[0] * 16 + vals[1] * 4 + vals[2];
}

std::istream &sveshnikov::operator>>(std::istream &in, Gene &gene)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char nucleotides[3];
  in.read(nucleotides, 3);
  if (in.gcount() != 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  gene = Gene(std::string(nucleotides, 3));
  return in;
}

std::ostream &sveshnikov::operator<<(std::ostream &out, const Gene &gene)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  StreamGuard streamguard(out);

  out << gene.get_nucleotides();
  return out;
}
