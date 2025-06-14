#include "DataStruct.hpp"
#include <cmath>
#include <iomanip>
#include "IOStreamGuard.hpp"

namespace {

  std::istream& input_value(std::istream& in, int* ids, int id, gavrilova::DataStruct& dest)
  {
    using namespace gavrilova;
    if (!in) {
      return in;
    }
    if (ids[id] == 0) {
      in >> DoubleI{dest.key1};
    } else if (ids[id] == 1) {
      in >> LongLongIO{dest.key2};
    } else if (ids[id] == 2) {
      in >> StringWithQuotsIO{dest.key3};
    } else {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

}

std::istream& gavrilova::operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  char c;
  in >> c;
  if (in && c != dest.exp) {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& gavrilova::operator>>(std::istream& in, SymbolIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  char c;
  in >> c;
  if (in && (std::tolower(c) != std::tolower(dest.exp))) {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& gavrilova::operator>>(std::istream& in, DoubleI&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  in >> dest.ref >> SymbolIO{'d'};
  return in;
}

std::istream& gavrilova::operator>>(std::istream& in, LongLongIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  in >> dest.ref >> SymbolIO{'l'} >> SymbolIO{'l'};
  return in;
}

std::istream& gavrilova::operator>>(std::istream& in, StringWithQuotsIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  std::getline(in >> DelimiterIO{'"'}, dest.ref, '"');
  return in;
}

std::istream& gavrilova::operator>>(std::istream& in, StringI_with_spaceIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  std::getline(in, dest.ref, ' ');
  return in;
}

std::istream& gavrilova::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }

  DataStruct input;
  using sep = DelimiterIO;

  int ids[] = {0, 1, 2};

  in >> sep{'('};
  in >> sep{':'};
  {
    std::string key = "";
    in >> key;
    if (key == "key2") {
      std::swap(ids[0], ids[1]);
    } else if (key == "key3") {
      std::swap(ids[0], ids[2]);
    } else if (key != "key1") {
      in.setstate(std::ios::failbit);
    }
    ::input_value(in, ids, 0, input);
  }

  in >> sep{':'};

  {
    std::string key = "";
    in >> key;
    if (key == "key1" && ids[0] == 0) {
      in.setstate(std::ios::failbit);
    } else if (key == "key2" && ids[0] == 1) {
      in.setstate(std::ios::failbit);
    } else if (key == "key3" && ids[0] == 2) {
      in.setstate(std::ios::failbit);
    } else if (key == "key3" && (ids[0] == 1 || ids[0] == 0)) {
      std::swap(ids[1], ids[2]);
    } else if (key == "key1" && ids[0] == 2) {
      std::swap(ids[1], ids[2]);
    } else if (key != "key1" && key != "key3" && key != "key2") {
      in.setstate(std::ios::failbit);
    }
    ::input_value(in, ids, 1, input);
  }
  in >> sep{':'};
  {
    std::string key = "";
    in >> key;
    if (key == "key1" && ids[2] != 0) {
      in.setstate(std::ios::failbit);
    } else if (key == "key2" && ids[2] != 1) {
      in.setstate(std::ios::failbit);
    } else if (key == "key1" && ids[2] != 0) {
      in.setstate(std::ios::failbit);
    } else if (key != "key1" && key != "key3" && key != "key2") {
      in.setstate(std::ios::failbit);
    }
    ::input_value(in, ids, 2, input);
  }
  in >> sep{':'};
  in >> sep{')'};
  if (in) {
    dest = input;
  }
  return in;
}

std::ostream& gavrilova::operator<<(std::ostream& out, const DoubleO& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry) {
    return out;
  }
  IOStreamGuard fmtguard(out);
  out << dest.ref;
  out << ((std::fmod(dest.ref, 1.0) == 0) ? ".0d" : "d");
  return out;
}

std::ostream& gavrilova::operator<<(std::ostream& out, const DataStruct& src)
{
  std::ostream::sentry sentry(out);
  if (!sentry) {
    return out;
  }
  out << "(:key1 " << DoubleO{src.key1};
  out << ":key2 " << src.key2 << "ll:";
  out << "key3 \"" << src.key3 << "\":)";
  return out;
}

bool gavrilova::compare(const DataStruct& lhs, const DataStruct& rhs)
{
  if (lhs.key1 != rhs.key1) {
    return lhs.key1 < rhs.key1;
  }
  if (lhs.key2 != rhs.key2) {
    return lhs.key2 < rhs.key2;
  }
  return lhs.key3.length() < rhs.key3.length();
}
