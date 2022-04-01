#include "resource.h"
#include "CRC16.h"
#include <intsafe.h>
#include <filesystem>
#include <iostream>
#include "cryptEngine.h"

rfile::flyff::Hdr::Hdr(std::ifstream& ifs) : g_byEncryptionKey(0), bEncryption(false), nMergeFileHeaderSize(0), fileNumber(0)
{
	mainRead(ifs);
	if (bEncryption)
		for (auto& c : str)
			c = static_cast<char>(decryption(c));
	
	memcpy(&version, &str[0], 7);
	memcpy(&fileNumber, &str[7], sizeof(short));
}

void rfile::flyff::Hdr::mainRead(std::ifstream& ifs)
{
	ifs.read(reinterpret_cast<char*>(&g_byEncryptionKey), sizeof(unsigned char));
	ifs.read(reinterpret_cast<char*>(&bEncryption), sizeof(bool));
	ifs.read(reinterpret_cast<char*>(&nMergeFileHeaderSize), sizeof(int));

	str.resize(nMergeFileHeaderSize);
	ifs.read(&str[0], nMergeFileHeaderSize);
}

unsigned char rfile::flyff::Hdr::decryption(unsigned char byData) const
{
	byData = ~byData ^ g_byEncryptionKey;
	return static_cast<unsigned char>(byData << 4 | byData >> 4);
}

rfile::flyff::Res::Res(const Hdr& hdr, std::streampos& currentPos) : nFileLength(0), fileSize(0), time_(0), filePos(0)
{
	memcpy(&nFileLength, &hdr.str[currentPos], sizeof(short));
	currentPos += sizeof(short);

	fileName.resize(nFileLength);
	memcpy(&fileName[0], &hdr.str[currentPos], sizeof(decltype(fileName)::value_type) * nFileLength);
	currentPos += static_cast<long long>(sizeof(decltype(fileName)::value_type) * nFileLength);

	memcpy(&fileSize, &hdr.str[currentPos], sizeof(int));
	currentPos += sizeof(int);

	memcpy(&time_, &hdr.str[currentPos], sizeof(unsigned long));
	currentPos += sizeof(unsigned long);

	memcpy(&filePos, &hdr.str[currentPos], sizeof(unsigned long));
	currentPos += sizeof(unsigned long);
}

rfile::flyff::Res::Res(const other::HdrAesGOW& hdr, std::streampos& currentPos) : nFileLength(0), fileSize(0), time_(0), filePos(0)
{
	memcpy(&nFileLength, &hdr.hdrString[currentPos], sizeof(short));
	currentPos += sizeof(short);

	fileName.resize(nFileLength);
	memcpy(&fileName[0], &hdr.hdrString[currentPos], sizeof(decltype(fileName)::value_type) * nFileLength);
	currentPos += static_cast<long long>(sizeof(decltype(fileName)::value_type) * nFileLength);

	memcpy(&fileSize, &hdr.hdrString[currentPos], sizeof(int));
	currentPos += sizeof(int);

	memcpy(&time_, &hdr.hdrString[currentPos], sizeof(unsigned long));
	currentPos += sizeof(unsigned long);

	memcpy(&filePos, &hdr.hdrString[currentPos], sizeof(unsigned long));
	currentPos += sizeof(unsigned long);
}



rfile::other::HdrInsignia::HdrInsignia(std::ifstream& ifs) : hdrSize(0), fileType(11), byEncryptionKey(0xC4), fileNumber(0)
{
	ifs.seekg(-60, std::ios::end);
	ifs.read(reinterpret_cast<char*>(&hdrSize), sizeof(int));
	ifs.read(reinterpret_cast<char*>(&fileType), sizeof(unsigned char));
	ifs.seekg(0, std::ios::beg);

	switch (fileType)
	{
		case 11: byEncryptionKey = 0x63; break;
		case 22: case 44: byEncryptionKey = 0x12; break;
		case 33: byEncryptionKey = 0xF1; break;
		case 55: byEncryptionKey = 0xE2; bEncryption = false; break;
		case 66: byEncryptionKey = 0x98; break;
		case 77: byEncryptionKey = 0xA5; break;
		default: break;
	}

	headerStr.resize(hdrSize);
	ifs.read(&headerStr[0], hdrSize);
	for (auto& c : headerStr)
		c = static_cast<char>(decryptionHdr(c));

	memcpy(&fileNumber, &headerStr[0], sizeof(short));
}

unsigned char rfile::other::HdrInsignia::decryptionHdr(unsigned char byData) const
{
	byData = ~byData ^ encInsignia[decryptPos];
	if (decryptPos < 2047)
		decryptPos++;

	return static_cast<unsigned char>(byData << 4 | byData >> 4);
}

unsigned char rfile::other::HdrInsignia::decryptionFile(unsigned char byData) const
{
	byData = ~byData ^ byEncryptionKey;
	return static_cast<unsigned char>(byData << 4 | byData >> 4);
}

rfile::other::ResInsignia::ResInsignia(const HdrInsignia& hdr, std::streampos& currentPos) : nFileLength(0), fileSize(0), time_(0), filePos(0)
{
	memcpy(&nFileLength, &hdr.headerStr[currentPos], sizeof(short));
	currentPos += sizeof(short);

	fileName.resize(nFileLength);
	memcpy(&fileName[0], &hdr.headerStr[currentPos], sizeof(decltype(fileName)::value_type) * nFileLength);
	currentPos += static_cast<long long>(sizeof(decltype(fileName)::value_type) * nFileLength);

	memcpy(&fileSize, &hdr.headerStr[currentPos], sizeof(int));
	currentPos += sizeof(int);

	memcpy(&time_, &hdr.headerStr[currentPos], sizeof(unsigned long));
	currentPos += sizeof(unsigned long);

	memcpy(&filePos, &hdr.headerStr[currentPos], sizeof(unsigned long));
	currentPos += sizeof(unsigned long);
}


rfile::other::HdrAesGOW::HdrAesGOW(std::ifstream& ifs) : bEncryption(false), nFileHeaderSize(0), fileNumber(0)
{
	ifs.read(reinterpret_cast<char*>(&bEncryption), sizeof(bool));
	ifs.read(reinterpret_cast<char*>(&nFileHeaderSize), sizeof(int));
	if (bEncryption)
	{
		const unsigned long long nBlockHeaderSize = static_cast<unsigned long long>(nFileHeaderSize) + 15 &~ 15;
		hdrString.reserve(nBlockHeaderSize);

		std::string tempString;
		tempString.reserve(nBlockHeaderSize);
		ifs.read(&tempString[0], static_cast<long long>(nBlockHeaderSize));
		
		for (unsigned long long i = 0; i < nBlockHeaderSize / 16ul; ++i)
			cryptEngine::defAes::m_crypt.DecryptBlock(reinterpret_cast<const unsigned char*>(&tempString[i * 16ul]), 
			reinterpret_cast<unsigned char*>(&hdrString[i * 16]));
	}
	else
	{
		hdrString.reserve(nFileHeaderSize);
		ifs.read(&hdrString[0], nFileHeaderSize);
	}
	memcpy(&fileNumber, &hdrString[0], sizeof(short));
}

rfile::other::HdrAesAzure::HdrAesAzure(std::ifstream& ifs): firstCrc(0), fileVersion(0), bEncryption(false), headerSize(0), fileNumber(0)
{
	ifs.read(reinterpret_cast<char*>(&firstCrc), sizeof(unsigned short));
	ifs.read(reinterpret_cast<char*>(&fileVersion), sizeof(short));
	ifs.read(reinterpret_cast<char*>(&bEncryption), sizeof(bool));
	ifs.read(reinterpret_cast<char*>(&headerSize), sizeof(int));
	if (bEncryption)
	{
		const int nBlockHeaderSize = headerSize + (headerSize & 0x0F);
		headerStr.resize(headerSize);
		
		std::string cryptHdr;
		cryptHdr.resize(nBlockHeaderSize);

		ifs.read(&cryptHdr[0], nBlockHeaderSize);
		cryptEngine::defAes::m_crypt.Decrypt(reinterpret_cast<const unsigned char*>(&cryptHdr[0]), reinterpret_cast<unsigned char*>(&headerStr[0]), nBlockHeaderSize >> 4, AES::BlockMode::ECB);
	}
	else
	{
		headerStr.resize(headerSize, 0);
		ifs.read(&headerStr[0], headerSize);
		rotr(reinterpret_cast<unsigned char*>(&headerStr[0]), headerSize);
	}

	if (const unsigned short crc = cryptEngine::flyff::crc16(reinterpret_cast<unsigned char*>(&headerStr[0]), headerSize); crc != firstCrc)
		std::cout << "crc not matching" << std::endl;
	memcpy(&fileNumber, &headerStr[0], sizeof(short));
}

rfile::other::ResAesAzure::ResAesAzure(const HdrAesAzure& hdr, std::streampos& startPos): crc(0), size(0), offset(0), time_(0)
{
	memcpy(this, &hdr.headerStr[startPos], sizeof(ResAesAzure));
	startPos += sizeof(ResAesAzure);
}

void rfile::other::HdrAesAzure::rotr(unsigned char* data, const size_t size)
{
	constexpr unsigned int mask = (8 * sizeof(unsigned char) - 1);
	constexpr int len = 1;

	for (size_t i = 0; i < size; ++i)
		data[i] = static_cast<unsigned char>(data[i] >> len | data[i] << (-len & mask));
}

void rfile::other::HdrAesAzure::rotl(unsigned char* data, const size_t size)
{
	constexpr unsigned int mask = (8 * sizeof(unsigned char) - 1);
	constexpr int len = 1;

	for (size_t i = 0; i < size; ++i)
		data[i] = static_cast<unsigned char>(data[i] << len | data[i] >> (-len & mask));
}



rfile::OF::Hdr::Hdr([[maybe_unused]] std::ifstream& ifs) : fileNumber(0), key(0)
{
}

rfile::OF::Res::Res(const Hdr& hdr, std::streampos& startPos): offset(0), size(0), keyArr4(0)
{
	memcpy(this, &hdr.hdrString[startPos], sizeof(Res));
	startPos += sizeof(Res);
}

rfile::other::HdrMoon::HdrMoon(std::ifstream& ifs) : key(0), isEncrpytion(true), headerLen(0), fileNumber(0)
{
	ifs.read(reinterpret_cast<char*>(&key), sizeof(unsigned char));
	ifs.read(reinterpret_cast<char*>(&isEncrpytion), sizeof(bool));
	ifs.read(reinterpret_cast<char*>(&headerLen), sizeof(unsigned long));

	str.resize(headerLen);
	ifs.read(&str[0], headerLen);
	for (auto& c : str)
		c = static_cast<char>(decryption(c));

	memcpy(&ver, &str[0], 5);
	memcpy(&fileNumber, &str[5], sizeof(short));
}

unsigned char rfile::other::HdrMoon::decryption(unsigned char byData) const
{
	byData = ~byData ^ (key ^ 0x7B);
	return static_cast<unsigned char>(byData << 4 | byData >> 4);
}

rfile::other::ResMoon::ResMoon(const HdrMoon& hdr, std::streampos& currentPos) : offset(0), fileSize(0), fileNameLen(0), time(0)
{
	memcpy(&offset, &hdr.str[currentPos], sizeof(unsigned long));
	currentPos += sizeof(unsigned long);

	memcpy(&fileSize, &hdr.str[currentPos], sizeof(unsigned long));
	currentPos += sizeof(unsigned long);

	memcpy(&fileNameLen, &hdr.str[currentPos], sizeof(short));
	currentPos += sizeof(short);

	memcpy(&time, &hdr.str[currentPos], sizeof(unsigned long));
	currentPos += sizeof(unsigned long);

	fileName.resize(fileNameLen);
	memcpy(&fileName[0], &hdr.str[currentPos], sizeof(decltype(fileName)::value_type) * fileNameLen);
	currentPos += static_cast<long long>(sizeof(decltype(fileName)::value_type) * fileNameLen);
}

rfile::other::HdrEquinox::HdrEquinox() : headerLen(0), xorKey(0), xorKey2(0), ver(0), bEncrpytion(false), fileNumber(0) { }
rfile::other::HdrEquinox::HdrEquinox(std::ifstream& ifs) : headerLen(0), xorKey(0), xorKey2(0), ver(0), bEncrpytion(false), fileNumber(0)
{
	char skip[20];
	ifs.read(skip, 20);
	//if (_strcmpi(skip, "\"RoC Resource File\"")) return

	ifs.read(reinterpret_cast<char*>(&headerLen), sizeof(long long));
	ifs.read(reinterpret_cast<char*>(&xorKey), sizeof(unsigned char));

	headerLen ^= xorKey;
	hdr.resize(headerLen);
	ifs.read(&hdr[0], headerLen);

	ifs.read(reinterpret_cast<char*>(&xorKey2), sizeof(unsigned char));

	for (auto& c : hdr)
		c = static_cast<char>(decrpytion(static_cast<unsigned char>(c), xorKey2));

	memcpy(&ver, &hdr[0], sizeof(short));
	memcpy(&bEncrpytion, &hdr[sizeof(short)], sizeof(bool));
	memcpy(&fileNumber, &hdr[sizeof(short)+sizeof(bool)], sizeof(short));
}

unsigned char rfile::other::HdrEquinox::decrpytion(const unsigned char byData, const unsigned char key)
{
	return eTable[byData ^ key];
}


rfile::other::ResEquinox::ResEquinox() : encKey(0), fileSize(0), time_(0), offset(0), fileNameLen(0) {}
rfile::other::ResEquinox::ResEquinox(const HdrEquinox& hdr, std::streampos& startPos) : encKey(0), fileSize(0), time_(0), offset(0), fileNameLen(0)
{
	memcpy(&encKey, &hdr.hdr[startPos], sizeof(unsigned char));
	startPos += sizeof(unsigned char);

	memcpy(&fileSize, &hdr.hdr[startPos], sizeof(long));
	startPos += sizeof(long);

	memcpy(&time_, &hdr.hdr[startPos], sizeof(long));
	startPos += sizeof(long);

	memcpy(&offset, &hdr.hdr[startPos], sizeof(long));
	startPos += sizeof(long);

	memcpy(&fileNameLen, &hdr.hdr[startPos], sizeof(short));
	startPos += sizeof(short);

	fileName.resize(fileNameLen);
	memcpy(&fileName[0], &hdr.hdr[startPos], sizeof(char) * fileNameLen);
	startPos += static_cast<long long>(sizeof(char) * fileNameLen);
}


rfile::other::HdrForasken::HdrForasken(std::ifstream& ifs) : fileNumber(0)
{
	ifs.read(reinterpret_cast<char*>(&fileNumber), sizeof(unsigned long));
	
	const auto pos = ifs.tellg();
	ifs.seekg(0, std::ios::end);
	
	const auto hdrSize = ifs.tellg() - pos;
	ifs.seekg(pos);

	hdr.resize(hdrSize);
	ifs.read(&hdr[0], hdrSize);
}

rfile::other::ResForsaken::ResForsaken(const HdrForasken& hdr, std::streampos& startPos) : stringSize(0), offset(0), fileSize(0)
{
	unsigned long entropia;
	memcpy(&entropia, &hdr.hdr[startPos], sizeof(unsigned long));
	if (entropia == 0xFFFFFFFF)
		startPos += sizeof(unsigned long);

	memcpy(&stringSize, &hdr.hdr[startPos], sizeof(unsigned long));
	startPos += sizeof(unsigned long);

	fileName.resize(stringSize);
	memcpy(&fileName[0], &hdr.hdr[startPos], sizeof(char) * stringSize);
	startPos += static_cast<long long>(sizeof(char) * stringSize);

	memcpy(&offset, &hdr.hdr[startPos], sizeof(unsigned long));
	startPos += sizeof(unsigned long);

	if (entropia == 0xFFFFFFFF)
	{
		offset = -offset;
		fileSize += 10;
	}
	memcpy(&fileSize, &hdr.hdr[startPos], sizeof(unsigned long));
	startPos += sizeof(unsigned long);
}

// todo : group other structs
rfile::other::HdrCloud::HdrCloud(std::ifstream& ifs) 
{
	mainRead(ifs);
	// should technically work because byte is min size. but havent tested.
	if (static_cast<unsigned char>(bEncryption) == ENCR_MARKER1)
	{
		g_byEncryptionKey = g_byEncryptionKey ^ ENCR_KEY;
		bEncryption = false;
	}
	else if (static_cast<unsigned char>(bEncryption) == ENCR_MARKER2)
	{
		g_byEncryptionKey = g_byEncryptionKey ^ ENCR_KEY;
		bEncryption = true;
	}

	for (auto& c : str)
		c = static_cast<char>(decryption(c));

	memcpy(&version, &str[0], 7);
	memcpy(&fileNumber, &str[7], sizeof(short));
}

rfile::feifei::Hdr::Hdr(std::ifstream& ifs) : fileNumber(0), offset(0)
{
	ifs.read(reinterpret_cast<char*>(&id), sizeof(unsigned int));
	ifs.read(reinterpret_cast<char*>(&fileNumber), sizeof(unsigned int));
	ifs.read(reinterpret_cast<char*>(&offset), sizeof(unsigned int));


	ifs.seekg(offset, std::ios::beg);
	str.resize(fileNumber * sizeof(Res));
	ifs.read(&str[0], static_cast<long long>(sizeof(Res) * fileNumber));
}


rfile::feifei::Res::Res(const Hdr& header, std::streampos & pos) : uid(0), offset(0), size(0), space(0)
{
	memcpy(this, &header.str[pos], sizeof(Res));
	pos += sizeof(Res);
}


// todo: Reverse engineer Insanity Flyff encrpytion.
rFileOther::HdrInsanity::HdrInsanity(std::ifstream& ifs)
{
	ifs.read(reinterpret_cast<char*>(&k525), sizeof(unsigned char)); // =
	ifs.read(reinterpret_cast<char*>(&b), sizeof(unsigned char)); // r

	ifs.read(theBadWord, 16); // "V0.FUCKYOU...."

	ifs.read(reinterpret_cast<char*>(&fileNumber), sizeof(unsigned short));
	ifs.read(reinterpret_cast<char*>(&qq), sizeof(unsigned char)); // 10
}

std::vector<rFileOther::ResInsanity> rFileOther::HdrInsanity::doInHdr(std::ifstream& ifs)
{
	std::vector<ResInsanity> rs;
	rs.reserve(fileNumber);
	for (int i = 0; i < fileNumber; ++i)
		rs.emplace_back(ResInsanity(ifs));

	unsigned char cont = 0;
	ifs.read(reinterpret_cast<char*>(&cont), sizeof(unsigned char));
	if (cont == 8 || cont == 9)
	{
		k525 = 121;
		k526 = 1;
	}
	if (cont == 6)
	{
		k525 = 121;
		k526 = 0; // byteptr
		[[maybe_unused]] unsigned char k527 = 1;
	}
	else if (cont == 7)
	{
		k525 = 121;
		k526 = 1;
		[[maybe_unused]] unsigned char k527 = 1;
	}
	else if (cont == 1)
	{
		k526 = true;
	}

	[[maybe_unused]] unsigned long anotherone = 0;
	ifs.read(reinterpret_cast<char*>(&cont), sizeof(unsigned char));
	// 52c + 55c (file)
	// to func
	return rs;
}

rFileOther::ResInsanity::ResInsanity(std::ifstream& ifs) : fileNameLen(0), size(0), time(0), offset(0)
{
	ifs.read(reinterpret_cast<char*>(&fileNameLen), sizeof(unsigned short));
	fileName.resize(fileNameLen);
	ifs.read(&fileName[0], static_cast<std::streamsize>(fileNameLen * sizeof(char)));
	ifs.read(reinterpret_cast<char*>(&size), sizeof(unsigned long));
	ifs.read(reinterpret_cast<char*>(&time), sizeof(unsigned long));
	ifs.read(reinterpret_cast<char*>(&offset), sizeof(unsigned long));
}

