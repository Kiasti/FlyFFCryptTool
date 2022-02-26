#pragma once

namespace cryptEngine::wbqt
{
	constexpr unsigned int CRC32_POLYNOMIAL = 0x04C11DB7;
	class CCRC32
	{
		static bool mInitialized;
		static unsigned int  mTable[256];

		static void Initialize();
		static unsigned int Reflect(unsigned int reflect, char chr);

		public:
			static unsigned int Compute(const unsigned char* data, unsigned int length);
	};
}
