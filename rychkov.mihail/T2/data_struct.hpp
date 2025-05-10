#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <string>
#include <iosfwd>

namespace rychkov
{
  struct DataStruct
  {
    double key1;
    long long key2;
    std::string key3;
  };
  std::istream& operator>>(std::istream& in, DataStruct& link);
  std::ostream& operator<<(std::ostream& in, const DataStruct& link);

  namespace iofmt
  {
    struct nth_ds_field
    {
      size_t field_number;
      DataStruct& link;
    };
    std::istream& operator>>(std::istream& in, nth_ds_field wrapper);

    template< class T, size_t Id = 0 >
    class io_literal;
    template< class T, size_t Id >
    extern std::istream& operator>>(std::istream& in, io_literal< T, Id >&& wrapper);
    template< class T, size_t Id >
    extern std::ostream& operator<<(std::ostream& out, const io_literal< T, Id >&& wrapper);

    template< class T, size_t Id >
    class io_literal
    {
    public:
      using value_type = T;
      static constexpr size_t id = Id;

      io_literal(const value_type& init):
        value_(init),
        link_(value_)
      {}
      io_literal(value_type& init):
        value_(),
        link_(init)
      {}
      friend std::istream& rychkov::iofmt::operator>>< T, Id >(std::istream& in, io_literal&& wrapper);
      friend std::ostream& rychkov::iofmt::operator<<< T, Id >(std::ostream& out, const io_literal&& wrapper);
    private:
      value_type value_;
      value_type& link_;
    };
    using scientific_literal = io_literal< double >;
    using ull_literal = io_literal< long long >;
    using string_literal = io_literal< std::string >;
  }
}

#endif
