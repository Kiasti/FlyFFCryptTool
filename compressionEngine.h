#pragma once
#include <string>
#pragma comment (lib, "zlib/Lib/zlibS-x64.lib") 

namespace compressionEngine
{
	class zlibIO
	{
		static constexpr size_t chunk = 256000;
		static void compressString(std::string& file);
		static bool decompressString(std::string& in);
		
		static void compressFile(const std::string& fileName, const std::string& output);		
	};
}
