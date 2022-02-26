#pragma once

namespace cryptEngine::wbqt
{
	constexpr unsigned long long POLY = 0xC96C5795D7870F42;
	class CRC64
	{
		static unsigned long long mTable[256];
		static bool mInitialized;
		static void InitTable();
	
		public:
			static unsigned long long Compute(const unsigned char* data, unsigned long long size);
	};
}