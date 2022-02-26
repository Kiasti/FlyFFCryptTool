#pragma once
#include <string>
#include "AES.h"
#include "resource.h"
#pragma comment (lib, "Cryptopp/Lib/cryptlib.lib")

namespace cryptEngine
{
	constexpr char shaHash_Forskaen[]{ "m3ntu5d3rHur3ns00hn" };
	class defAes
	{
		public:
			static inline AES m_crypt;
	};
	
	class ppAes
	{
		static void encrpytString(std::string& file);
		static unsigned long encrpytString(std::string& file, const unsigned char* hash, size_t sizeofHash);
		static unsigned long encrpytString(std::string& file, unsigned short crc);
		static void decryptString(const std::string& data, unsigned char k);
		static unsigned long encryptFile(const std::string& fileName, const std::string& outFile, const unsigned char* hash, size_t sizeofHash);
	};
	
	enum class saltPos { beg, end };
	enum class shaEncode { hexLowerCase, hexUpperCase, base64, def };
	std::string getSha256(const std::string& str, std::string&& salt = shaHash_Forskaen, saltPos&& pos = saltPos::beg, shaEncode&& shaEn = shaEncode::hexLowerCase);
}
