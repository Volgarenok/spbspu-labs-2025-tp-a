#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <map>
#include <string>
#include <vector>
#include <utility>
#include <iostream>

namespace pilugina
{
  using dictionary = std::map< std::string, std::vector< std::string > >;
  using dictionaries = std::map< std::string, dictionary >;

  bool read3Words(std::istream &in, std::string &a, std::string &b, std::string &c);
  bool read2Words(std::istream &in, std::string &a, std::string &b);
  bool read1Word(std::istream &in, std::string &a);
  bool fileExists(const std::string &name);
  void printTranslations(const std::vector< std::string > &ts, std::ostream &os);
  void printDictionary(const dictionary &d, std::ostream &os);

  struct ApplyMergeTranslations
  {
    explicit ApplyMergeTranslations(dictionary &dst);
    int operator()(const std::pair< const std::string, std::vector< std::string > > &p) const;

  private:
    dictionary *dst_;
  };

  struct MissingIn
  {
    explicit MissingIn(const dictionary &d);
    bool operator()(const std::pair< const std::string, std::vector< std::string > > &p) const;

  private:
    const dictionary *d_;
  };
}

#endif
