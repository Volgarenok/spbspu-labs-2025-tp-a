#include <iomanip>

#include "data_struct.hpp"
#include "delimiter.hpp"
#include "input_parsers.hpp"

std::ostream& fedorov::operator<<(std::ostream& out, const DataStruct& value)
{
  std::ostream::sentry guard(out);
  if (!guard) {
    return out;
  }

  std::ios_base::fmtflags original_flags = out.flags();
  std::streamsize original_precision = out.precision();
  out << std::setprecision(1) << std::fixed;
  out << "(:key1 " << value.key1 << "d";
  out << ":key2 #c(" << value.key2.real() << " " << value.key2.imag() << "):";
  out << "key3 \"" << value.key3 << "\":)";
  out.precision(original_precision);
  out.flags(original_flags);
  return out;
}

bool fedorov::operator<(const DataStruct& lhs, const DataStruct& rhs)
{
  if ( lhs.key1 != rhs.key1 ) {
    return lhs.key1 < rhs.key1;
  } else if ( std::abs(lhs.key2) != std::abs(rhs.key2) ) {
    return std::abs(lhs.key2) < std::abs(rhs.key2);
  } else {
    return (lhs.key3.length() < rhs.key3.length());
  }
}

std::istream& fedorov::operator>>(std::istream& in, DataStruct& value)
{
  std::istream::sentry guard(in);
  if ( !guard ) {
    return in;
  }

  using del = DelimiterI;
  size_t count = 0;
  std::string numKey = " ";

  in >> del{'('};
  while ( in && (count < 3) ) {
    in >> del{':'} >> numKey;
    if ( numKey == "key1" ) {
      in >> DoubleKey{value.key1};
      ++count;
    } else if ( numKey == "key2" ) {
      in >> ComplexKey{value.key2};
      ++count;
    } else if ( numKey == "key3" ) {
      in >> StringKey{value.key3};
      ++count;
    } else {
      in.setstate(std::ios::failbit);
    }
  }
  in >> del{':'} >> del{')'};
  return in;
}
