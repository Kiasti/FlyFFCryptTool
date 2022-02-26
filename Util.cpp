#include "Util.h"

unsigned int util::str::countOccurance(const std::string& s, const char delimiter) 
{
	return static_cast<unsigned int>(std::count(s.begin(), s.end(), delimiter)); 
}

std::vector<int> util::str::explodeToNumbers(std::string&& s, const char delimiter)
 {
     const unsigned int count = countOccurance(s, delimiter);
     std::vector<int> res;
     res.reserve(count);
     if (count)
     {
         size_t newPos = 0;
         size_t pos;
         while ((pos = s.find(delimiter, newPos)) != std::string::npos)
         {
             res.push_back(std::stoi(s.substr(newPos, (pos - newPos))));
             newPos = pos + 1;
         }
         if (s.find(delimiter, newPos) == std::string::npos)
             res.push_back(std::stoi(s.substr(newPos, (s.length() - newPos))));
         return res;
     }
     return{};
 }

bool util::str::isNumber(const std::string& s)
{
    return !s.empty() && std::ranges::find_if(s, [](const unsigned char c) { return !std::isdigit(c); }) == s.end();
}
