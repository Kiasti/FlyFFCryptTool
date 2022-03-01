#pragma once
#include <fstream>

struct rwIO
{
	rwIO() = default;
	explicit rwIO(std::ifstream&, bool) { }
	virtual ~rwIO() = default;

	rwIO(rwIO const&) = default;
	rwIO& operator =(rwIO const&) = default;
	rwIO(rwIO&&) = default;
	rwIO& operator=(rwIO&&) = default;

	virtual void write(std::ofstream&) const { }
	virtual void read(std::ifstream&) { }
	virtual void writeStringFile(std::ofstream&) const { }
	virtual void readStringFile(std::ifstream&) { }
};
