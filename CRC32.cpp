#include "CRC32.h"

bool cryptEngine::wbqt::CCRC32::mInitialized = false;
unsigned int cryptEngine::wbqt::CCRC32::mTable[256];

void cryptEngine::wbqt::CCRC32::Initialize()
{
	for (int i = 0; i <= 255; ++i)
	{
		mTable[i] = Reflect(i, 8) << 24;

		for (int pos = 0; pos < 8; ++pos)
			mTable[i] = (mTable[i] << 1) ^ ((mTable[i] & (1 << 31)) ? CRC32_POLYNOMIAL : 0);

		mTable[i] = Reflect(mTable[i], 32);
	}
	mInitialized = true;
}

unsigned int cryptEngine::wbqt::CCRC32::Reflect(unsigned int  reflect, const char chr)
{
	unsigned int  ret = 0;
	for (int pos = 1; pos <= chr; ++pos)
	{
		if (reflect & 1)
			ret |= (1 << (chr - pos));
		reflect >>= 1;
	}
	return ret;
}

unsigned int cryptEngine::wbqt::CCRC32::Compute(const unsigned char *data, const unsigned int length)
{
	if (!mInitialized)
		Initialize();

	unsigned int ret = static_cast<unsigned int>(~0);
	for (unsigned int  i = 0; i < length; ++i)
		ret = (ret >> 8) ^ mTable[(ret & 255) ^ data[i]];
	return ret ^ ~0;
}

