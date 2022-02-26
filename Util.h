#pragma once
#include <string>
#include <vector>

namespace util::str
{
	unsigned int countOccurance(const std::string& s, char delimiter);
	[[nodiscard]] std::vector<int> explodeToNumbers(std::string&& s, char delimiter);
	bool isNumber(const std::string& s);
}
