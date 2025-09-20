#include "Date.hpp"

#include <tuple>

bool gavrilova::date::operator<(const Date& a, const Date& b)
{
  return std::tie(a.year, a.month, a.day) < std::tie(b.year, b.month, b.day);
}

bool gavrilova::date::operator==(const Date& a, const Date& b)
{
  return std::tie(a.year, a.month, a.day) == std::tie(b.year, b.month, b.day);
}

std::istream& gavrilova::date::operator>>(std::istream& is, Date& d)
{
  std::istream::sentry sentry(is);
  if (!sentry) {
    return is;
  }

  char sep1 = '\0';
  char sep2 = '\0';
  int temp_day = 0;
  int temp_month = 0;
  int temp_year = 0;

  is >> temp_day;
  is.get(sep1);
  is >> temp_month;
  is.get(sep2);
  is >> temp_year;

  if (is.fail()) {
    return is;
  }

  if (sep1 != '-' || sep2 != '-' || temp_month < 1 || temp_month > 12 ||
      temp_day < 1 || temp_day > 31 || temp_year < 1900 || temp_year > 2100) {
    is.setstate(std::ios_base::failbit);
  } else {
    d.year = temp_year;
    d.month = temp_month;
    d.day = temp_day;
  }

  return is;
}

std::ostream& gavrilova::date::operator<<(std::ostream& os, const Date& d)
{
  std::ostream::sentry sentry(os);
  if (!sentry) {
    return os;
  }

  os << to_string(d);
  return os;
}

std::string gavrilova::date::to_string(const Date& d)
{
  std::string s_day = std::to_string(d.day);
  std::string s_month = std::to_string(d.month);

  if (s_day.length() < 2) {
    s_day = "0" + s_day;
  }
  if (s_month.length() < 2) {
    s_month = "0" + s_month;
  }

  return s_day + "-" + s_month + "-" + std::to_string(d.year);
}
