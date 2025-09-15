#ifndef GEN_HPP
#define GEN_HPP

#include <iostream>
#include <string>

namespace sveshnikov
{

  class Gen
  {
  public:
    Gen() = default;
    explicit Gen(const std::string &value);

    const std::string &value() const noexcept;
    int fitness() const;

    bool operator==(const Gen &other) const;
    bool operator!=(const Gen &other) const;

  private:
    std::string value_;
  };

  std::istream &operator>>(std::istream &in, Gen &gen);
  std::ostream &operator<<(std::ostream &out, const Gen &gen);

}

#endif
