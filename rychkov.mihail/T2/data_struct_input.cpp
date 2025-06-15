#include "data_struct.hpp"

#include <istream>
#include <cmath>
#include <fmtguard.hpp>
#include <skip_any_of.hpp>

template<>
std::istream& rychkov::iofmt::operator>>< rychkov::iofmt::scientific_literal::value_type,
      rychkov::iofmt::scientific_literal::id >(std::istream& in, scientific_literal&& wrapper)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  fmtguard guard(in);
  long long whole = 0, fractional = 0, exponent = 0;
  in >> whole >> std::noskipws >> anyof(".") >> fractional;
  if (in && (fractional >= 0) && (in >> anyof("e", "E") >> exponent))
  {
    long long temp = fractional, fracLen10 = 0;
    for (; temp != 0; temp /= 10, fracLen10++)
    {}
    fractional = whole >= 0 ? fractional : -fractional;
    wrapper.link_ = (whole + fractional / std::pow(10., fracLen10)) * std::pow(10., exponent);
    return in;
  }
  in.setstate(std::ios::failbit);
  return in;
}
template<>
std::istream& rychkov::iofmt::operator>>< rychkov::iofmt::ull_literal::value_type,
      rychkov::iofmt::ull_literal::id >(std::istream& in, ull_literal&& wrapper)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  return in >> wrapper.link_ >> anyof("LL", "ll");
}
template<>
std::istream& rychkov::iofmt::operator>>< rychkov::iofmt::string_literal::value_type,
      rychkov::iofmt::string_literal::id >(std::istream& in, string_literal&& wrapper)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  return std::getline(in >> anyof("\""), wrapper.link_, '"');
}

std::istream& rychkov::iofmt::operator>>(std::istream& in, nth_ds_field wrapper)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  switch (wrapper.field_number)
  {
  case 0:
    return in >> scientific_literal(wrapper.link.key1);
  case 1:
    return in >> ull_literal(wrapper.link.key2);
  case 2:
    return in >> string_literal(wrapper.link.key3);
  }
  in.setstate(std::ios::failbit);
  return in;
}
std::istream& rychkov::operator>>(std::istream& in, DataStruct& link)
{
  std::istream::sentry sentry(in);
  if (!sentry || !(in >> iofmt::anyof("(")))
  {
    return in;
  }
  field_register< 3 > entered;
  while (!entered.full())
  {
    size_t key_id = -1;
    if (!(in >> iofmt::anyof(&key_id, ":key1", ":key2", ":key3")) || !entered.reg(key_id))
    {}
    else if (in >> iofmt::anyof(true, " ", "\t", "\n") >> iofmt::nth_ds_field{key_id, link})
    {
      continue;
    }
    in.setstate(std::ios::failbit);
    return in;
  }
  return in >> iofmt::anyof(":)");
}
