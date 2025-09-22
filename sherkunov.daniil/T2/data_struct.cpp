#include "data_struct.hpp"
#include <iostream>
#include <cctype>

namespace sherkunov {
  namespace io_helpers {

    struct DelimiterIO {
      char c;
    };

    struct LexCharIO {
      char c;
    };

    struct LongLongIO {
      long long& ref;
    };

    struct PairIO {
      std::pair< long long, unsigned long long >& ref;
    };

    struct StringIO {
      std::string& ref;
    };

    struct Key2IO {
      std::pair< long long, unsigned long long >& ref;
    };

    std::istream& operator>>(std::istream& input, DelimiterIO&& value);
    std::istream& operator>>(std::istream& input, LexCharIO&& value);
    std::istream& operator>>(std::istream& input, LongLongIO&& value);
    std::istream& operator>>(std::istream& input, PairIO&& value);
    std::istream& operator>>(std::istream& input, StringIO&& value);
    std::istream& operator>>(std::istream& input, Key2IO&& value);

    std::ostream& operator<<(std::ostream& output, const LongLongIO& value);
    std::ostream& operator<<(std::ostream& output, const PairIO& value);
    std::ostream& operator<<(std::ostream& output, const StringIO& value);
  }

  std::istream& io_helpers::operator>>(std::istream& input, DelimiterIO&& value)
  {
    std::istream::sentry s(input);
    if (!s) {
      return input;
    }
    char c = 0;
    if ((input >> c) && (c != value.c)) {
      input.setstate(std::ios::failbit);
    }
    return input;
  }

  std::istream& io_helpers::operator>>(std::istream& input, LexCharIO&& value)
  {
    std::istream::sentry s(input);
    if (!s) {
      return input;
    }
    char c = 0;
    if ((input >> c) && (std::tolower(c) != std::tolower(value.c))) {
      input.setstate(std::ios::failbit);
    }
    return input;
  }

  std::istream& io_helpers::operator>>(std::istream& input, LongLongIO&& value)
  {
    std::istream::sentry s(input);
    if (!s) {
      return input;
    }
    long long temp = 0;
    if (!(input >> temp >> LexCharIO{ 'l' } >> LexCharIO{ 'l' })) {
      return input;
    }
    value.ref = temp;
    return input;
  }

  std::ostream& io_helpers::operator<<(std::ostream& output, const LongLongIO& value)
  {
    std::ostream::sentry s(output);
    if (!s) {
      return output;
    }
    return output << value.ref << "ll";
  }

  std::istream& io_helpers::operator>>(std::istream& input, PairIO&& value)
  {
    std::istream::sentry s(input);
    if (!s) {
      return input;
    }
    long long n = 0;
    unsigned long long d = 0;
    input >> DelimiterIO{ 'N' } >> n >> DelimiterIO{ ':' } >> DelimiterIO{ 'D' } >> d;
    if (input) {
      value.ref = std::make_pair(n, d);
    }
    return input;
  }

  std::ostream& io_helpers::operator<<(std::ostream& output, const PairIO& value)
  {
    std::ostream::sentry s(output);
    if (!s) {
      return output;
    }
    return output << "N " << value.ref.first << ":D " << value.ref.second;
  }

  std::istream& io_helpers::operator>>(std::istream& input, StringIO&& value)
  {
    std::istream::sentry s(input);
    if (!s) {
      return input;
    }
    std::string temp;
    input >> DelimiterIO{ '"' };
    std::getline(input, temp, '"');
    if (input) {
      value.ref = std::move(temp);
    }
    return input;
  }

  std::ostream& io_helpers::operator<<(std::ostream& output, const StringIO& value)
  {
    std::ostream::sentry s(output);
    if (!s) {
      return output;
    }
    return output << '"' << value.ref << '"';
  }

  std::istream& io_helpers::operator>>(std::istream& input, Key2IO&& value)
  {
    return input >> DelimiterIO{ '(' } >> DelimiterIO{ ':' } >> PairIO{ value.ref } >> DelimiterIO{ ':' } >> DelimiterIO{ ')' };
  }

  std::istream& operator>>(std::istream& input, DataStruct& value)
  {
    std::istream::sentry s(input);
    if (!s) {
      return input;
    }
    DataStruct temp{};
    input >> io_helpers::DelimiterIO{ '(' } >> io_helpers::DelimiterIO{ ':' };

    for (int i = 0; i < 3; ++i) {
      std::string key;
      input >> key;

      if (key.size() != 4 || key.substr(0, 3) != "key") {
        input.setstate(std::ios::failbit);
        return input;
      }

      char k = key.back();
      switch (k) {
      case '1':
        input >> io_helpers::LongLongIO{ temp.key1 };
        break;
      case '2':
        input >> io_helpers::Key2IO{ temp.key2 };
        break;
      case '3':
        input >> io_helpers::StringIO{ temp.key3 };
        break;
      default:
        input.setstate(std::ios::failbit);
        return input;
      }

      input >> io_helpers::DelimiterIO{ ':' };
    }

    input >> io_helpers::DelimiterIO{ ')' };
    if (input) {
      value = std::move(temp);
    }
    return input;
  }

  std::ostream& operator<<(std::ostream& output, const DataStruct& value)
  {
    std::ostream::sentry s(output);
    if (!s) {
      return output;
    }
    long long key1_copy = value.key1;
    std::pair< long long, unsigned long long > key2_copy = value.key2;
    std::string key3_copy = value.key3;

    output << "(:key1 " << io_helpers::LongLongIO{ key1_copy };
    output << ":key2 (:" << io_helpers::PairIO{ key2_copy } << ":)";
    output << ":key3 " << io_helpers::StringIO{ key3_copy } << ":)";
    return output;
  }

  bool operator<(const DataStruct& lhs, const DataStruct& rhs)
  {
    if (lhs.key1 != rhs.key1) {
      return lhs.key1 < rhs.key1;
    } else {
      long long lhsCrossProduct = lhs.key2.first * rhs.key2.second;
      long long rhsCrossProduct = rhs.key2.first * lhs.key2.second;
      if (lhsCrossProduct != rhsCrossProduct) {
        return lhsCrossProduct < rhsCrossProduct;
      } else {
        return lhs.key3.size() < rhs.key3.size();
      }
    }
  }
}
