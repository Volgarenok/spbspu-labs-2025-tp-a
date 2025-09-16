#ifndef GEN_HPP
#define GEN_HPP
#include <string>
#include <iostream>

namespace sveshnikov
{
  class Gen
  {
  public:
    Gen() = default;
    explicit Gen(const std::string &val);

    bool operator==(const Gen &other) const;
    bool operator!=(const Gen &other) const;

    const std::string &get_nucleotides() const noexcept;
    int calc_fitness() const;

  private:
    std::string nucleotides_;
  };

  std::istream &operator>>(std::istream &in, Gen &gen);
  std::ostream &operator<<(std::ostream &out, const Gen &gen);
}

#endif
