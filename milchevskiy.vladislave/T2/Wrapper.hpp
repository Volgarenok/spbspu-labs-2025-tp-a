#ifndef WRAPPERS_HPP
#define WRAPPERS_HPP

#include <string>

namespace milchevskiy
{
	struct DelimiterIO
	{
		char exp;
	};

	struct UnsignedLongLongOCT
	{
		unsigned long long& ref;
	};

	struct UnsignedLongLongULL
	{
		unsigned long long& ref;
	};

	struct StringIO
	{
		std::string& ref;
	};

	std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
	std::istream& operator>>(std::istream& in, UnsignedLongLongOCT&& dest);
	std::istream& operator>>(std::istream& in, UnsignedLongLongULL&& dest);
	std::istream& operator>>(std::istream& in, StringIO&& dest);

} // namespace milchevskiy

#endif