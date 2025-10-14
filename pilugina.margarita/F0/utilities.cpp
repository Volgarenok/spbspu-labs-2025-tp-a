#include "utilities.hpp"
#include <istream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <sstream>

namespace
{
  struct AppendIfMissing
  {
    explicit AppendIfMissing(std::vector< std::string > &v):
      to_(&v)
    {}

    int operator()(const std::string &s) const
    {
      if (std::find(to_->begin(), to_->end(), s) == to_->end())
      {
        to_->push_back(s);
      }
      return 0;
    }

  private:
    std::vector< std::string > *to_;
  };

  struct DictEntryFormatter
  {
    std::string operator()(const std::pair< const std::string, std::vector< std::string > > &entry) const
    {
      std::ostringstream line;
      line << entry.first << ": ";
      std::copy(entry.second.begin(), entry.second.end(), std::ostream_iterator< std::string >(line, ", "));
      return line.str();
    }
  };
}

void pilugina::printTranslations(const std::vector< std::string > &ts, std::ostream &os)
{
  if (ts.empty())
  {
    return;
  }

  std::copy(ts.begin(), ts.end(), std::ostream_iterator< std::string >(os, ", "));
}

void pilugina::printDictionary(const dictionary &d, std::ostream &os)
{
  std::vector< std::string > lines;
  lines.reserve(d.size());

  std::transform(d.begin(), d.end(), std::back_inserter(lines), DictEntryFormatter {});
  std::copy(lines.begin(), lines.end(), std::ostream_iterator< std::string >(os, ""));
}

pilugina::ApplyMergeTranslations::ApplyMergeTranslations(dictionary &dst):
  dst_(&dst)
{}

int pilugina::ApplyMergeTranslations::operator()(
    const std::pair< const std::string, std::vector< std::string > > &p) const
{
  auto it = dst_->find(p.first);
  if (it == dst_->end())
  {
    return 0;
  }
  const std::vector< std::string > &from = p.second;
  std::vector< std::string > &to = it->second;

  std::vector< int > dummy(from.size(), 0);
  std::transform(from.begin(), from.end(), dummy.begin(), AppendIfMissing(to));
  return 0;
}

bool pilugina::read3Words(std::istream &in, std::string &a, std::string &b, std::string &c)
{
  return static_cast< bool >(in >> a >> b >> c);
}
bool pilugina::read2Words(std::istream &in, std::string &a, std::string &b)
{
  return static_cast< bool >(in >> a >> b);
}
bool pilugina::read1Word(std::istream &in, std::string &a)
{
  return static_cast< bool >(in >> a);
}

bool pilugina::fileExists(const std::string &name)
{
  std::ifstream f(name.c_str());
  return f.good();
}

pilugina::MissingIn::MissingIn(const dictionary &d):
  d_(&d)
{}

bool pilugina::MissingIn::operator()(const std::pair< const std::string, std::vector< std::string > > &p) const
{
  return d_->find(p.first) == d_->end();
}

