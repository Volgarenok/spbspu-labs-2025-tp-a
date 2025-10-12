#ifndef INDEX_HPP
#define INDEX_HPP

#include <map>
#include <set>
#include <string>
#include <vector>

namespace trukhanov
{
  using IndexMap = std::map< std::string, std::set< std::size_t > >;

  struct WordEntry
  {
    const std::string& word;
    const std::set< size_t >& lines;
    WordEntry(const std::pair< const std::string, std::set< size_t > >& p);
  };

  std::ostream& operator<<(std::ostream& out, const std::set< size_t >& s);
  std::ostream& operator<<(std::ostream& out, const WordEntry& entry);

  struct ConcordanceIndex
  {
    IndexMap index;
    std::vector< std::string > lines;
    std::string sourceFile;
  };

  std::istream& operator>>(std::istream& in, std::vector< std::string >& args);

  class IndexStorage
  {
  public:
    using IndexContainer = std::map< std::string, ConcordanceIndex >;

  private:
    IndexContainer indexes_;
  };
}

#endif
