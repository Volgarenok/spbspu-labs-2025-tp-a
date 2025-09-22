#ifndef INDEX_HPP
#define INDEX_HPP

#include <map>
#include <set>
#include <string>
#include <vector>

namespace krylov
{
  using IndexDictionary = std::map< std::string, std::set< std::size_t > >;

  struct WordEntry
  {
    const std::string& word;
    const std::set< size_t >& lines;
    WordEntry(const std::pair< const std::string, std::set< size_t > >& p);
  };
  std::ostream& operator<<(std::ostream& out, const std::set< size_t >& s);
  std::ostream& operator<<(std::ostream& out, const WordEntry& entry);

  struct IndexDocument
  {
    IndexDictionary index;
    std::vector< std::string > lines;
    std::string sourceFile;
  };
  std::istream& operator>>(std::istream& in, std::vector< std::string >& args);

  class IndexStorage
  {
  public:
    using IndexList = std::map< std::string, IndexDocument >;
  private:
    IndexList indexes_;
  };
}

#endif
