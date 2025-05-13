#include "data.hpp"
#include "stream_guard.hpp"
#include "test.hpp"
#include <iostream>
<<<<<<< HEAD
#include <cmath>

bool amine::operator<(const DataStruct &lhs, const DataStruct &rhs)
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
=======
#include <iomanip>
#include <cmath>
bool amine::operator<(const DataStruct &lhs, const DataStruct &rhs)
{
    if (lhs.key1 != rhs.key1)
        return lhs.key1 < rhs.key1;
    double lhs_ratio = static_cast<double>(lhs.key2.first) / lhs.key2.second;
    double rhs_ratio = static_cast<double>(rhs.key2.first) / rhs.key2.second;
    if (lhs_ratio != rhs_ratio)
        return lhs_ratio < rhs_ratio;
    return lhs.key3.size() < rhs.key3.size();
>>>>>>> c515838cf784c143137d2c17bd825b2c1b191313
}
std::ostream &amine::operator<<(std::ostream &out, const DataStruct &src)
{
    std::ostream::sentry sentry(out);
    if (!sentry) return out;
    StreamGuard fg(out);
    out << std::scientific << std::setprecision(1);
    out << "(:key1 " << src.key1
        << ":key2 (:N " << src.key2.first << ":D " << src.key2.second << ":)"
        << ":key3 \"" << src.key3 << "\":)";
    return out;
<<<<<<< HEAD
  }
  FormatGuard fg(out);

  out << "(:key1 " << src.key1 << "e+00";
  out << ":key2 (:N " << src.key2.first << ":D " << src.key2.second << ":)";
  out << ":key3 \"" << src.key3 << "\":)";
  return out;
=======
>>>>>>> c515838cf784c143137d2c17bd825b2c1b191313
}
std::istream &amine::operator>>(std::istream &in, DataStruct &dest)
{
<<<<<<< HEAD
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  DataStruct tmp {};
  in >> DelimiterIO {'('};

  std::string field;
  for (int i = 0; i < 3; i++)
  {
    in >> DelimiterIO {':'} >> field;
    if (field == "key1")
    {
      in >> DoubleSCI {tmp.key1};
    }
    else if (field == "key2")
    {
      in >> RationalLSP {tmp.key2};
    }
    else if (field == "key3")
    {
      in >> StringIO {tmp.key3};
=======
    std::istream::sentry sentry(in);
    if (!sentry) return in;
    DataStruct tmp;
    bool has_key1 = false, has_key2 = false, has_key3 = false;
    in >> DelimiterIO{'('} >> DelimiterIO{':'};
    for (int i = 0; i < 3; i++) {
        std::string field;
        in >> field;
        if (field == "key1") {
            in >> DoubleSCI{tmp.key1};
            has_key1 = true;
        }
        else if (field == "key2") {
            in >> RationalIO{tmp.key2};
            has_key2 = true;
        }
        else if (field == "key3") {
            in >> StringIO{tmp.key3};
            has_key3 = true;
        }
        else {
            in.setstate(std::ios::failbit);
        }
        in >> DelimiterIO{':'};
>>>>>>> c515838cf784c143137d2c17bd825b2c1b191313
    }
    in >> DelimiterIO{')'};
    if (in && has_key1 && has_key2 && has_key3)
        dest = tmp;
    else
<<<<<<< HEAD
    {
      in.setstate(std::ios::failbit);
      return in;
    }
  }

  in >> DelimiterIO {':'} >> DelimiterIO {')'};
  if (in)
  {
    dest = tmp;
  }
  return in;
=======
        in.setstate(std::ios::failbit);
    return in;
>>>>>>> c515838cf784c143137d2c17bd825b2c1b191313
}
