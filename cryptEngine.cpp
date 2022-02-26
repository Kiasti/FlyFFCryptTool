#include "cryptEngine.h"
#include "Cryptopp/Include/aes.h"
#include "Cryptopp/Include/cryptlib.h"
#include "Cryptopp/Include/sha.h"
#include "Cryptopp/Include/files.h"
#include "Cryptopp/Include/modes.h"
#include "Cryptopp/Include/hex.h"
#include "Cryptopp/Include/base64.h"

std::string cryptEngine::getSha256(const std::string& str, std::string&& salt, saltPos&& pos, shaEncode&& shaEn)
{
	std::string temp;
	if (!salt.empty())
	{
		switch (pos)
		{
			case saltPos::beg: temp = salt + str; break;
			case saltPos::end: temp = str + salt; break;
		}
	}
	else
		temp = str;


	std::string digest;
	CryptoPP::SHA256 hash;
	switch (shaEn)
	{
		case shaEncode::hexLowerCase:
		{
			CryptoPP::StringSource foo(temp, true, new CryptoPP::HashFilter(hash, new CryptoPP::HexEncoder(new CryptoPP::StringSink(digest))));
			std::ranges::transform(digest, digest.begin(), [](const unsigned char c) { return std::tolower(c); });
			break;
		}
		case shaEncode::hexUpperCase:
		{
			CryptoPP::StringSource foo(temp, true, new CryptoPP::HashFilter(hash, new CryptoPP::HexEncoder(new CryptoPP::StringSink(digest))));
			break;
		}
		case shaEncode::def:
		{
			hash.Update(reinterpret_cast<const unsigned char*>(temp.data()), temp.size());
			digest.resize(hash.DigestSize());
			hash.Final(reinterpret_cast<unsigned char*>(&digest[0]));
			break;
		}
		case shaEncode::base64:
		{
			CryptoPP::StringSource foo(temp, true, new CryptoPP::HashFilter(hash, new CryptoPP::Base64Encoder(new CryptoPP::StringSink(digest))));
			break;
		}
	}
	return digest;
}

/*
* List of unused currently function of using aes from crypto++. 
*/
void cryptEngine::ppAes::encrpytString(std::string& file)
{
	std::string temp;
	temp.reserve(file.length());

	CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption aes(res::file::OF::key.data(), (res::file::OF::key.size()), res::file::OF::iv.data(), (res::file::OF::iv.size())); 
	CryptoPP::StringSource sss(file, true, new CryptoPP::StreamTransformationFilter(aes, new CryptoPP::StringSink(temp)));
	file = std::move(temp);
}

unsigned long cryptEngine::ppAes::encrpytString(std::string& file, const unsigned char* hash, const size_t sizeofHash)
{
	std::string temp;
	temp.reserve(file.length());

	if (sizeofHash < 64)
		throw "meow";

	unsigned long keyArry = *reinterpret_cast<const unsigned long*>(hash);
	auto ivList = res::file::OF::iv;
	for (size_t i = 8; i < sizeofHash; ++i)
	{
		keyArry ^= *(hash + i);
		i = i + 5;
	}

	auto fileIv = res::file::OF::iv;
	for (int i = 1; i < 5; ++i)
		fileIv[i * 3] = *reinterpret_cast<unsigned char*>(&keyArry + i);

	CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption aes(res::file::OF::key.data(), (res::file::OF::key.size()), fileIv.data(), (fileIv.size()));
	CryptoPP::StringSource sss(file, true, new CryptoPP::StreamTransformationFilter(aes, new CryptoPP::StringSink(temp)));
	file = std::move(temp);
	return keyArry;
}

unsigned long cryptEngine::ppAes::encrpytString(std::string& file, unsigned short crc)
{
	std::string temp;
	temp.reserve(file.length());

	auto fileIv = res::file::OF::iv;
	fileIv[0] = *reinterpret_cast<unsigned char*>(&crc);
	CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption aes(res::file::OF::key.data(), (res::file::OF::key.size()), fileIv.data(), (fileIv.size()));
	CryptoPP::StringSource sss(file, true, new CryptoPP::StreamTransformationFilter(aes, new CryptoPP::StringSink(temp)));
	file = std::move(temp);
	return 0;
}


void cryptEngine::ppAes::decryptString(const std::string& data, const unsigned char k)
{
	std::string output;
	CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption aes;

	auto iv2 = res::file::OF::iv;
	iv2[0] = k;
	aes.SetKeyWithIV(res::file::OF::key.data(), res::file::OF::key.size(), iv2.data(), iv2.size());
	CryptoPP::StringSource sss(data, true, new CryptoPP::StreamTransformationFilter(aes, new CryptoPP::StringSink(output)));
}


unsigned long cryptEngine::ppAes::encryptFile(const std::string& fileName, const std::string& outFile, const unsigned char* hash, const size_t sizeofHash)
{
	unsigned long keyArry = *reinterpret_cast<const unsigned long*>(hash);
	auto ivList = res::file::OF::iv;
	for (size_t i = 8; i < sizeofHash; ++i)
	{
		keyArry ^= *(hash + i);
		i = i + 5;
	}

	auto fileIv = res::file::OF::iv;
	for (int i = 1; i < 5; ++i)
		fileIv[i * 3] = *reinterpret_cast<unsigned char*>(&keyArry + i);

	CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption e(res::file::OF::key.data(), (res::file::OF::key.size()), fileIv.data(), (fileIv.size()));
	CryptoPP::FileSource sss(fileName.c_str(), true, new CryptoPP::StreamTransformationFilter(e, new CryptoPP::FileSink(outFile.c_str())));

	return keyArry;
}
