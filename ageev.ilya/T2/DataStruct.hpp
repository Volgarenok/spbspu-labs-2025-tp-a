#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP
#include <string>
#include <istream>

namespace ageev
{
  struct DataStruct
  {
    double key1;
    double key2;
    std::string key3;

    bool operator<(const DataStruct&) const;
  };

  struct DelimiterIO
  {
    char exp;
  };

  struct DoubleIO
  {
    double& ref;
  };

  struct DoubleSciIO
  {
    double& ref;
  };

  struct StringIO
  {
    std::string& ref;
  };

  struct LabelIO
  {
    std::string exp;
  };

  struct DoubleToSciForm
  {
    double ref;
  };

  class Iofmtguard
  {
  public:
    Iofmtguard(std::basic_ios< char >& s);
    ~Iofmtguard();
  private:
    std::basic_ios< char >& s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, DoubleIO&& dest);
  std::istream& operator>>(std::istream& in, DoubleSciIO&& dest);
  std::istream& operator>>(std::istream& in, StringIO&& dest);
  std::istream& operator>>(std::istream& in, LabelIO&& dest);
  std::istream& operator>>(std::istream& in, DataStruct& dest);
  std::ostream& operator<<(std::ostream& out, const DoubleToSciForm& dest);
  std::ostream& operator<<(std::ostream& out, const DataStruct& src);

}

#endif
