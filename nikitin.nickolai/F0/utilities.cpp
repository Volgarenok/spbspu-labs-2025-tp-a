#include "utilities.hpp"
#include <istream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <regex>
#include <sstream>

namespace
{
  struct DictEntryFormatter
  {
    std::string operator()(const nikitin::pairType &entry) const
    {
      std::ostringstream line;
      line << entry.first << ": ";
      std::copy(entry.second.begin(), entry.second.end(), std::ostream_iterator< std::string >(line, ", "));
      return line.str();
    }
  };
}

void nikitin::printTranslations(const std::vector< std::string > &ts, std::ostream &os)
{
  if (ts.empty())
  {
    return;
  }

  std::copy(ts.begin(), ts.end(), std::ostream_iterator< std::string >(os, ", "));
}

void nikitin::printDictionary(const dictionary &d, std::ostream &os)
{
  std::vector< std::string > lines;
  lines.reserve(d.size());

  std::transform(d.begin(), d.end(), std::back_inserter(lines), DictEntryFormatter {});
  std::copy(lines.begin(), lines.end(), std::ostream_iterator< std::string >(os, ""));
}

void nikitin::LineToDict::operator()(const std::string &line) const
{
  static const std::regex ws(R"(\s+)");
  using tok_it = std::sregex_token_iterator;

  std::vector< std::string > tokens;
  std::copy(tok_it { line.begin(), line.end(), ws, -1 }, tok_it {}, std::back_inserter(tokens));

  if (tokens.empty() || tokens.size() < 2)
  {
    bad = true;
    return;
  }
  d[tokens.front()] = std::vector< std::string >(std::next(tokens.begin()), tokens.end());
}

nikitin::dictionary::value_type nikitin::BuildEntry::operator()(const std::string &key) const
{
  std::vector< std::string > va, vb, merged;

  const auto ia = a->find(key);
  if (ia != a->end())
  {
    va = ia->second;
  }
  const auto ib = b->find(key);
  if (ib != b->end())
  {
    vb = ib->second;
  }

  std::sort(va.begin(), va.end());
  va.erase(std::unique(va.begin(), va.end()), va.end());
  std::sort(vb.begin(), vb.end());
  vb.erase(std::unique(vb.begin(), vb.end()), vb.end());

  std::set_union(va.begin(), va.end(), vb.begin(), vb.end(), std::back_inserter(merged));

  return dictionary::value_type(key, std::move(merged));
}
