#include "data_struct.hpp"
#include <bitset>
#include <istream>
#include <input_wrapper_structs.hpp>
#include <format_guard.hpp>
#include "output_wrapper_structs.hpp"

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
  std::bitset< 3 > keys_found;
  bool valid = true;

  in >> input::Character {'('} >> input::Character {':'};

  for (std::size_t i = 0; i < 3 && valid; ++i)
  {
    std::string key;
    in >> key;

    if (key == "key1" && !keys_found[0])
    {
      in >> input::ULLValue {dest.key1};
      keys_found.set(0);
    }
    else if (key == "key2" && !keys_found[1])
    {
      in >> input::PairValue {dest.key2};
      keys_found.set(1);
    }
    else if (key == "key3" && !keys_found[2])
    {
      in >> input::StringValue {dest.key3};
      keys_found.set(2);
    }
    else
    {
      valid = false;
    }
  }

  if (!valid || !keys_found.all())
  {
    in.setstate(std::ios::failbit);
  }

  in >> input::Character {')'};
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
