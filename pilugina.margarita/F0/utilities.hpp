#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <map>
#include <string>
#include <vector>
#include <utility>
#include <ostream>
#include <istream>
#include <fstream>
#include <algorithm>

namespace pilugina
{
  using dictionary = std::map< std::string, std::vector< std::string > >;
  using dictionaries = std::map< std::string, dictionary >;

  std::string joinedTranslations(const std::vector< std::string > &ts);

  struct WordLineFormatter
  {
    std::string operator()(const std::pair< const std::string, std::vector< std::string > > &p) const;
  };

  struct ApplyMergeTranslations
  {
    explicit ApplyMergeTranslations(dictionary &dst);
    int operator()(const std::pair< const std::string, std::vector< std::string > > &p) const;

  private:
    dictionary *dst_;
  };

  bool read3(std::istream &in, std::string &a, std::string &b, std::string &c);
  bool read2(std::istream &in, std::string &a, std::string &b);
  bool read1(std::istream &in, std::string &a);

  bool fileExists(const std::string &name);

  struct Prefixer
  {
    explicit Prefixer(const std::string &prefix);
    std::string operator()(const std::string &s) const;

  private:
    std::string prefix_;
  };

  struct MissingIn
  {
    explicit MissingIn(const dictionary &d);
    bool operator()(const std::pair< const std::string, std::vector< std::string > > &p) const;

  private:
    const dictionary *d_;
  };

  struct PrefixAndAppendToOstream
  {
    explicit PrefixAndAppendToOstream(std::ostream &o);
    char operator()(const std::string &s) const;

  private:
    std::ostream *out_;
  };

  struct PrintLine
  {
    explicit PrintLine(std::ostream &o);
    std::string operator()(const std::pair< const std::string, std::vector< std::string > > &p) const;
    void flush(const std::string &s) const;

  private:
    std::ostream *out_;
  };

  struct WriteLine
  {
    explicit WriteLine(std::ostream &o);
    char operator()(const std::string &s) const;

  private:
    std::ostream *out_;
  };
}

#endif
