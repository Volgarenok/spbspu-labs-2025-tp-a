#include "DataStruct.hpp"
#include <iomanip>
#include <vector>
namespace hismatova
{
  std::istream& operator>>(std::istream& in, CharIO&& data)
  {
    std::istream::sentry sen(in);
    if (!sen)
    {
      return in;
    }
    char c = '0';
    in >> c;
    if (in && (c != data.ch))
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }
  std::istream& operator>>(std::istream& in, ULLIO&& data)
  {
    (void)data;
    std::istream::sentry sen(in);
    if (!sen)
    {
      return in;
    }
    char u = '0';
    char l1 = '0';
    char l2 = '0';
    in >> u >> l1 >> l2;
    if (in && (u == 'u' || u == 'U') && l1 == l2 && (l1 == 'l' || l1 == 'L'))
    {
      ;
    }
    else
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }
  std::istream& operator>>(std::istream& in, NumberIO&& data)
  {
    std::istream::sentry sen(in);
    if (!sen)
    {
      return in;
    }
    in >> data.ref >> ULLIO{};
    return in;
  }
  std::istream& operator>>(std::istream& in, ComplexIO&& data)
  {
    std::istream::sentry sen(in);
    if (!sen)
    {
      return in;
    }
    double real;
    double img;
    in >> CharIO{'#'} >> CharIO{'c'} >> CharIO{'('} >> real >> img >> CharIO{')'};
    if (in)
    {
      data.ref = std::complex< double >(real, img);
    }
    return in;
  }
  std::istream& operator>>(std::istream& in, StringIO&& data)
  {
    std::istream::sentry sen(in);
    if (!sen)
    {
      return in;
    }
    return std::getline(in >> CharIO{'"'}, data.ref, '"');
  }
  std::istream& operator>>(std::istream& in, DataStruct& data)
  {
    std::istream::sentry sen(in);
    if (!sen)
    {
      return in;
    }
    DataStruct inp;
    {
      in >> CharIO {'('};
      std::vector< std::string > keys(3);
      for (int i = 0; i < 3; ++i)
      {
        in >> CharIO {':'} >> keys[i];
        if (keys[i] == "key1")
        {
          in >> NumberIO {inp.key1};
        }
        else if (keys[i] == "key2")
        {
          in >> ComplexIO {inp.key2};
        }
        else if (keys[i] == "key3")
        {
          in >> StringIO {inp.key3};
        }
        else
        {
          in.setstate(std::ios::failbit);
        }
      }
      in >> CharIO {':'} >> CharIO{')'};
    }
    if (in)
    {
      data = inp;
    }
    return in;
  }
  std::ostream& operator<<(std::ostream& out, const DataStruct& data)
  {
    out << "(:key1 " << data.key1 << "ull"
      << ":key2 #c(" << std::fixed << std::setprecision(1) << static_cast< double >(data.key2.real()) << " "
      << std::fixed << std::setprecision(1) << static_cast< double >(data.key2.imag()) << ")"
      << ":key3 \"" << data.key3 << "\":)";
    return out;
  }
}
