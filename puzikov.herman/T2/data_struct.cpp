#include "data_struct.hpp"
#include "input_wrapper_structs.hpp"
#include "output_wrapper_structs.hpp"
#include "format_guard.hpp"

#include <istream>

bool puzikov::operator<(const DataStruct &a, const DataStruct &b)
{
  if (a.key1 != b.key1)
  {
    return a.key1 < b.key1;
  }
  else if (a.key2 != b.key2)
  {
    return a.key2 < b.key2;
  }
  else
  {
    return a.key3.size() < b.key3.size();
  }
}

std::istream &puzikov::operator>>(std::istream &in, DataStruct &dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  in >> input::CharacterIO {'('} >> input::CharacterIO {':'};
  for (std::size_t i = 0; i < 3; ++i)
  {
    std::string key;
    in >> key;
    if (key == "key1")
    {
      in >> input::ULLValue {dest.key1};
    }
    else if (key == "key2")
    {
      in >> input::PairValue {dest.key2};
    }
    else if (key == "key3")
    {
      in >> input::StringValue {dest.key3};
    }
    else
    {
      in.setstate(std::ios::failbit);
      break;
    }
  }

  in >> input::CharacterIO {')'};
  return in;
}

std::ostream &puzikov::operator<<(std::ostream &out, const DataStruct &src)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  FormatGuard guard(out);

  out << "(:";
  out << "key1 " << output::ULLValue {src.key1} << ":";
  out << "key2 " << output::PairValue {src.key2} << ":";
  out << "key3 " << output::StringValue {src.key3} << ":";
  out << ")";

  return out;
}

