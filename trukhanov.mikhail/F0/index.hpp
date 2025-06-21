#ifndef INDEX_HPP
#define INDEX_HPP

#include <map>
#include <set>
#include <string>
#include <vector>

namespace trukhanov
{
  using IndexMap = std::map< std::string, std::set< std::size_t > >;

  struct ConcordanceIndex
  {
    IndexMap index;
    std::vector< std::string > lines;
    std::string sourceFile;
  };

  class IndexStorage
  {
  public:
    using IndexContainer = std::map< std::string, ConcordanceIndex >;

    bool hasIndex(const std::string& name) const;
    bool addIndex(const std::string& name, const ConcordanceIndex& index);
    bool removeIndex(const std::string& name);
    const ConcordanceIndex* getIndex(const std::string& name) const;
    ConcordanceIndex* getIndex(const std::string& name);
    const IndexContainer& getAll() const;

  private:
    IndexContainer indexes_;
  };
}

#endif
