#ifndef GEN_HPP
#define GEN_HPP
#include <string>
#include <iostream>

namespace sveshnikov
{
  class Gene
  {
  public:
    Gene() = default;
    explicit Gene(const std::string &val);

    bool operator==(const Gene &other) const;
    bool operator!=(const Gene &other) const;

    const std::string &get_nucleotides() const noexcept;
    int calc_fitness() const;

  private:
    std::string nucleotides_;
  };

  std::istream &operator>>(std::istream &in, Gene &gene);
  std::ostream &operator<<(std::ostream &out, const Gene &gene);
}

#endif
