#include "data_struct.hpp"
#include "stream_guard.hpp"
#include "input_struct.hpp"

bool nikitin::operator<(const DataStruct& lhs, const DataStruct& rhs)
{
  if (lhs.key1 != rhs.key1)
  {
    return lhs.key1 < rhs.key1;
  }
  else if (lhs.key2 != rhs.key2)
  {
    return std::abs(lhs.key2) < std::abs(rhs.key2);
  }
  return lhs.key3.length() < lhs.key3.length();
}

std::ostream& nikitin::operator<<(std::ostream& out, const DataStruct& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }

  StreamGuard guard(out);
  out << "(:key1 " << dest.key1 << "ll";
  out << std::fixed << std::setprecision(1);
  out << ":key2 #c(" << dest.key2.real() << " " << dest.key2.imag() << ")";
  out << ":key3 " << "\"" << dest.key3 << "\":)";
  return out;
}

std::istream& nikitin::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  DataStruct temp;
  {
    using sep = DelimiterIO;
    using ll = LongLongIO;
    using cmp = ComplexIO;
    using str = StringIO;

    in >> sep{'('} >> sep{':'};
    for (int i = 0; i < 3; ++i)
    {
      std::string key;
      in >> key;
      if (key == "key1")
      {
        in >> ll{temp.key1} >> sep{':'};
      }
      else if (key == "key2")
      {
        in >> cmp{temp.key2} >> sep{':'};
      }
      else if (key == "key3")
      {
        in >> str{temp.key3} >> sep{':'};
      }
      else
      {
        in.setstate(std::ios_base::failbit);
        return in;
      }
    }
    in >> sep{')'};
  }
  if (in)
  {
    dest = temp;
  }
  return in;
}
