#include "utilities.hpp"
#include <istream>
#include <fstream>
#include <algorithm>

namespace
{
  struct Prefixer
  {
    explicit Prefixer(const std::string &prefix):
      prefix_(prefix)
    {}

    std::string operator()(const std::string &s) const
    {
      return prefix_ + s;
    }

  private:
    std::string prefix_;
  };

  struct AppendTo
  {
    AppendTo(std::string &tgt):
      target_(&tgt)
    {}

    char operator()(const std::string &s) const
    {
      (*target_) += s;
      return 0;
    }

  private:
    std::string *target_;
  };

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
}

namespace pilugina
{
  std::string joinedTranslations(const std::vector< std::string > &ts)
  {
    if (ts.empty())
    {
      return std::string();
    }

    std::string result = ts.front();

    if (ts.size() > 1)
    {
      std::vector< std::string > restPrefixed;
      restPrefixed.reserve(ts.size() - 1);
      std::transform(ts.begin() + 1, ts.end(), std::back_inserter(restPrefixed), Prefixer(", "));
      std::vector< char > sink(restPrefixed.size(), 0);
      std::transform(restPrefixed.begin(), restPrefixed.end(), sink.begin(), AppendTo(result));
    }

    return result;
  }

  std::string WordLineFormatter::operator()(const std::pair< const std::string, std::vector< std::string > > &p) const
  {
    const std::string joined = joinedTranslations(p.second);
    return p.first + ": " + joined;
  }

  ApplyMergeTranslations::ApplyMergeTranslations(dictionary &dst):
    dst_(&dst)
  {}

  int ApplyMergeTranslations::operator()(const std::pair< const std::string, std::vector< std::string > > &p) const
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

  bool read3Words(std::istream &in, std::string &a, std::string &b, std::string &c)
  {
    return static_cast< bool >(in >> a >> b >> c);
  }
  bool read2Words(std::istream &in, std::string &a, std::string &b)
  {
    return static_cast< bool >(in >> a >> b);
  }
  bool read1Word(std::istream &in, std::string &a)
  {
    return static_cast< bool >(in >> a);
  }

  bool fileExists(const std::string &name)
  {
    std::ifstream f(name.c_str(), std::ios::in | std::ios::binary);
    return static_cast< bool >(f);
  }

  MissingIn::MissingIn(const dictionary &d):
    d_(&d)
  {}

  bool MissingIn::operator()(const std::pair< const std::string, std::vector< std::string > > &p) const
  {
    return d_->find(p.first) == d_->end();
  }

  PrefixAndAppendToOstream::PrefixAndAppendToOstream(std::ostream &o):
    out_(&o)
  {}

  char PrefixAndAppendToOstream::operator()(const std::string &s) const
  {
    (*out_) << "; " << s;
    return 0;
  }

  PrintLine::PrintLine(std::ostream &o):
    out_(&o)
  {}

  std::string PrintLine::operator()(const std::pair< const std::string, std::vector< std::string > > &p) const
  {
    return p.first + ": " + joinedTranslations(p.second);
  }

  void PrintLine::flush(const std::string &s) const
  {
    (*out_) << s << '\n';
  }

  WriteLine::WriteLine(std::ostream &o):
    out_(&o)
  {}

  char WriteLine::operator()(const std::string &s) const
  {
    (*out_) << s << '\n';
    return 0;
  }
}
