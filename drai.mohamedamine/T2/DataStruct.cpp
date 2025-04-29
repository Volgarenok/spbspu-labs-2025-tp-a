#include "DataStruct.h"

namespace nspace
{
    // Parse ULL LIT (e.g., 89u11, 89ULL)
    std::istream& operator>>(std::istream& in, UllLitIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry) return in;
        
        std::string input;
        in >> input;

        // Remove 'u' or 'ULL' suffix (if present)
        size_t suffix_pos = input.find_first_not_of("0123456789");
        if (suffix_pos != std::string::npos)
        {
            input = input.substr(0, suffix_pos);
        }

        std::istringstream iss(input);
        iss >> dest.ref;
        return in;
    }

    // Parse ULL BIN (strictly 0b or 0B prefix)
    std::istream& operator>>(std::istream& in, UllBinIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        in >> std::ws;
        if (in.peek() == '0')
        {
            in.get();
            char next = in.peek();
            if (next == 'b' || next == 'B')
            {
                in.get();
                in >> std::setbase(2) >> dest.ref;
            }
            else
            {
                in.unget();
                in.setstate(std::ios::failbit);
            }
        }
        else
        {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    // Parse DataStruct
    std::istream& operator>>(std::istream& in, DataStruct& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry) return in;
        int streamStartPos = in.tellg();
        DataStruct input;
        int keyNumber;

        in >> DelimiterIO{ '(' };
        for (int i = 0; i < 3; i++)
        {
            in >> DelimiterIO{ ':' };
            in >> LabelIO{ "key" };
            in >> KetNumIO{ keyNumber };
            if (!in) break;

            switch (keyNumber)
            {
            case 1:
                in >> UllLitIO{ input.key1 };  // Read as ULL LIT
                break;
            case 2:
                in >> UllBinIO{ input.key2 };  // Read as ULL BIN
                break;
            case 3:
                in >> StringValueIO{ input.key3 };
                break;
            }
        }
        in >> DelimiterIO{ ':' } >> DelimiterIO{ ')' };

        if (in) dest = input;
        else
        {
            in.clear();
            in.seekg(streamStartPos);
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    // Output DataStruct
    std::ostream& operator<<(std::ostream& out, const DataStruct& src)
    {
        std::ostream::sentry sentry(out);
        if (!sentry) return out;
        iofmtguard fmtguard(out);
        out << "(:key1 " << src.key1 << "u"  // ULL LIT format (e.g., 89u)
            << ":key2 0b" << std::setbase(2) << src.key2  // ULL BIN format (e.g., 0b1010)
            << ":key3 " << src.key3 << ":)";
        return out;
    }

    // Comparison function (sort by key1, then key2, then key3 length)
    bool compare(DataStruct a, DataStruct b)
    {
        if (a.key1 != b.key1) return a.key1 < b.key1;
        if (a.key2 != b.key2) return a.key2 < b.key2;
        return a.key3.length() < b.key3.length();
    }

    // (Rest of the operators remain the same)
}