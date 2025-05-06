#ifndef _DATA_STRUCT
#define _DATA_STRUCT

#include <iostream>
#include <string>
#include <iomanip>

namespace nspace
{
    struct DataStruct
    {
        unsigned long long key1;  // ULL LIT (e.g., 89u11, 89ULL)
        unsigned long long key2;  // ULL BIN (e.g., 0b1010, 0B1010)
        std::string key3;
    };

    struct DelimiterIO { char exp; };
    struct KetNumIO { int& keyN; };
    struct UllLitIO { unsigned long long& ref; };  // For ULL LIT (89u11)
    struct UllBinIO { unsigned long long& ref; };  // For ULL BIN (0b1010)
    struct StringIO { std::string& ref; };
    struct LabelIO { std::string exp; };
    struct StringValueIO { std::string& ref; };

    class iofmtguard {
    public:
        iofmtguard(std::basic_ios<char>& s);
        ~iofmtguard();
    private:
        std::basic_ios<char>& s_;
        char fill_;
        std::streamsize precision_;
        std::basic_ios<char>::fmtflags fmt_;
    };

    // Input operators
    std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
    std::istream& operator>>(std::istream& in, KetNumIO&& dest);
    std::istream& operator>>(std::istream& in, UllLitIO&& dest);
    std::istream& operator>>(std::istream& in, UllBinIO&& dest);
    std::istream& operator>>(std::istream& in, StringValueIO&& dest);
    std::istream& operator>>(std::istream& in, DataStruct& dest);

    // Output operators
    std::ostream& operator<<(std::ostream& out, const DataStruct& dest);

    // Comparison function
    bool compare(DataStruct a, DataStruct b);
}

#endif
