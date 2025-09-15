#include "gen.hpp"
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

  bool hasSubstr(const std::string &gen, const std::string &nucleotides)
  {
    return gen.find(nucleotides) == gen.npos;
  }

  bool checkComplementarity(const std::string &gen)
  {
    using namespace std::placeholders;
    std::array< std::string, 8 > forbidden_seq = {"CA", "AA", "AC", "AG", "GA", "GT", "GG", "TG"};
    auto is_not_found = std::bind(hasSubstr, gen, _1);
    return std::all_of(forbidden_seq.begin(), forbidden_seq.end(), is_not_found);
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
  }
}

sveshnikov::Gen::Gen(const std::string &nucleotides):
  nucleotides_(nucleotides)
{
  if (nucleotides_.size() != 3)
  {
    throw std::invalid_argument("ERROR: gen must be 3 characters long!");
  }

  if (!std::all_of(nucleotides_.begin(), nucleotides_.end(), checkNucleotideValidation))
  {
    throw std::invalid_argument("ERROR: invalid nucleotide!");
  }

  if (checkComplementarity(nucleotides_))
  {
    throw std::invalid_argument("ERROR: gen is not complementary!");
  }
}

bool sveshnikov::Gen::operator==(const Gen &other) const
{
  return nucleotides_ == other.nucleotides_;
}

bool sveshnikov::Gen::operator!=(const Gen &other) const
{
  return !(*this == other);
}

const std::string &sveshnikov::Gen::get_nucleotides() const noexcept
{
  assert(!nucleotides_.empty());
  return nucleotides_;
}

int sveshnikov::Gen::calc_fitness() const
{
  if (nucleotides_.empty())
  {
    return 0;
  }
  using namespace std::placeholders;
  std::array< size_t, 3 > vals;
  std::transform(nucleotides_.begin(), nucleotides_.end(), vals.begin(),
      std::bind(getNucleotideValue, _1));
  return vals[0] * 16 + vals[1] * 4 + vals[2];
}

std::istream &sveshnikov::operator>>(std::istream &in, Gen &gen)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  std::string nucleotides;
  in >> nucleotides;
  gen = Gen(nucleotides);
  return in;
}

std::ostream &sveshnikov::operator<<(std::ostream &out, const Gen &gen)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  StreamGuard streamguard(out);

  out << gen.get_nucleotides();
  return out;
}
