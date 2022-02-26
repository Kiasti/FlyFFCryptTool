#include "CRC64.h"

bool cryptEngine::wbqt::CRC64::mInitialized = false;
unsigned long long cryptEngine::wbqt::CRC64::mTable[256];

void cryptEngine::wbqt::CRC64::InitTable()
{
	for (int i = 0; i < 256; ++i)
	{
		unsigned long long crc = i;
		for (int j = 0; j < 8; ++j)
		{
			if (crc & 1)
			{
				crc >>= 1;
				crc ^= POLY;
			}
			else
				crc >>= 1;
		}
		mTable[i] = crc;
	}
	mInitialized = true;
}

unsigned long long cryptEngine::wbqt::CRC64::Compute(const unsigned char * data, const unsigned long long size)
{
	unsigned long long crc = 0;

	if (!mInitialized)
		InitTable();

	for (unsigned long long i = 0; i < size; ++i)
	{
		const unsigned char index = static_cast<unsigned char>(data[i] ^ crc);
		const unsigned long long lookup = mTable[index];

		crc >>= 8;
		crc ^= lookup;
	}
	return crc;
}