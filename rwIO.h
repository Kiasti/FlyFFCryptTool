#pragma once
#include <fstream>

enum class rwIOType : unsigned char
{
	bin, string, ffParser
};

struct rwIO
{
	rwIO() = default;
	explicit rwIO(std::ifstream&, bool) { }
	virtual ~rwIO() = default;

	rwIO(rwIO const&) = default;
	rwIO& operator =(rwIO const&) = default;
	rwIO(rwIO&&) = default;
	rwIO& operator=(rwIO&&) = default;

	virtual void write(std::ofstream&, rwIOType) const { }
	virtual void read(std::ifstream&, rwIOType) { }
};
