#include "data_struct.hpp"
#include <iomanip>
#include <delimiter.hpp>
#include "stream_guard.hpp"
#include "input_struct.hpp"
#include "output_struct.hpp"

bool mazitov::operator<(const DataStruct &lhs, const DataStruct &rhs)
{
  if (lhs.key1 != rhs.key1)
  {
    return lhs.key1 < rhs.key1;
  }
  if (lhs.key2 != rhs.key2)
  {
    return lhs.key2 < rhs.key2;
  }
  return lhs.key3.size() < rhs.key3.size();
}

std::ostream &mazitov::operator<<(std::ostream &out, const DataStruct &dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  StreamGuard guard(out);
  out << "(:key1 " << std::fixed << std::setprecision(1) << dest.key1 << "d";
  out << ":key2 0b";
  out << UnsignedLongLongBinOutput{dest.key2};
  out << ":key3 \"" << dest.key3 << "\":)";
  return out;
}

std::istream &mazitov::operator>>(std::istream &in, DataStruct &dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  DataStruct data;
  {
    using sep = DelimiterInput;
    using ull = UnsignedLongLongBinInput;
    using dbl = DoubleInput;
    using str = StringInput;

    in >> sep{'('} >> sep{':'};
    for (int i = 0; i < 3; i++)
    {
      std::string key;
      in >> key;

      if (key == "key1")
      {
        in >> dbl{data.key1} >> sep{':'};
      }
      else if (key == "key2")
      {
        in >> ull{data.key2};
      }
      else if (key == "key3")
      {
        in >> str{data.key3} >> sep{':'};
      }
      else
      {
        in.setstate(std::ios::failbit);
        return in;
      }
    }
    in >> sep{')'};
  }
  if (in)
  {
    dest = data;
  }
  return in;
}
