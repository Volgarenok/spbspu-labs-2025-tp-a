#ifndef DELEMITER_HPP
#define DELEMITER_HPP

#include <istream>

class DelimiterIstream
{
public:
	explicit DelimiterIstream(const std::string& expected);

	friend std::istream& operator>>(std::istream& in, const DelimiterIstream& delim);

private:
	std::string expected_;
};

#endif
