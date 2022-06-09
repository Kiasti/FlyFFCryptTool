#include "resourceMemLoad.h"

#include "CRC64.h"
#include "cryptEngine.h"

res::mLoad::load::load(std::string arch, std::string file) : archiveName(std::move(arch)), fileName(std::move(file)) 
{
}

std::string res::mLoad::load::operator()(const vp_Forsaken& df) const
{
	std::ranges::transform(fileName, fileName.begin(), [](const unsigned char c) { return std::tolower(c); });
	const std::string cpyName = cryptEngine::getSha256(fileName);

	std::filesystem::path tempPath = archiveName;
	tempPath.replace_extension(".fly");
	if (exists(tempPath))
	{
		archiveName = tempPath.string();
		if (std::ifstream readingFile(archiveName, std::ios::in | std::ios::binary); readingFile.good())
		{
			for (int i = 0; i < static_cast<int>(df.first.fileNumber); ++i)
			{
				if (cpyName == df.second[i].fileName)
				{
					std::string tempString;
					tempString.resize(df.second[i].fileSize);

					readingFile.seekg(df.second[i].offset);
					readingFile.read(&tempString[0], df.second[i].fileSize);

					if (!fileName.empty())
					{
						unsigned long fileNameCount = 0;
						for (char& j : tempString)
						{
							const unsigned char fNameChar = fileName[fileNameCount];
							unsigned char result = ~j ^ (fNameChar * -1);
							result = static_cast<unsigned char>((result << 4) | (result >> 4));
							j = static_cast<char>(result);

							fileNameCount++;
							if (fileNameCount >= fileName.length())
								fileNameCount = 0;
						}
					}
					readingFile.close();
					return tempString;
				}
			}
			readingFile.close();
		}
	}
	return "";
}



std::string res::mLoad::load::operator()(const vp_Flyff& df) const
{
	if (std::ifstream readingFile(archiveName, std::ios::in | std::ios::binary); readingFile.good())
	{
		for (int i = 0; i < df.first.fileNumber; ++i)
		{
			if (fileName == df.second[i].fileName)
			{
				std::string tempString;
				tempString.resize(df.second[i].fileSize);

				readingFile.seekg(df.second[i].filePos);
				readingFile.read(&tempString[0], df.second[i].fileSize);

				if (df.first.bEncryption)
					for (auto& c : tempString)
						c = static_cast<char>(df.first.decryption(c));

				readingFile.close();
				return tempString;
			}
		}
		readingFile.close();
	}
	return "";
}


std::string res::mLoad::load::operator()(const vp_Cloud& df) const
{
	if (std::ifstream readingFile(archiveName, std::ios::in | std::ios::binary); readingFile.good())
	{
		for (int i = 0; i < df.first.fileNumber; ++i)
		{
			if (fileName == df.second[i].fileName)
			{
				std::string tempString;
				tempString.resize(df.second[i].fileSize);

				readingFile.seekg(df.second[i].filePos);
				readingFile.read(&tempString[0], df.second[i].fileSize);

				if (df.first.bEncryption)
					for (auto& c : tempString)
						c = static_cast<char>(df.first.decryption(c));

				readingFile.close();
				return tempString;
			}
		}
		readingFile.close();
	}
	return "";
}


std::string res::mLoad::load::operator()(const vp_Insignia& df) const 
{
	if (std::ifstream readingFile(archiveName, std::ios::in | std::ios::binary); readingFile.good())
	{
		for (int i = 0; i < df.first.fileNumber; ++i)
		{
			if (fileName == df.second[i].fileName)
			{
				std::string tempString;
				tempString.resize(df.second[i].fileSize);

				readingFile.seekg(df.second[i].filePos);
				readingFile.read(&tempString[0], df.second[i].fileSize);

				if (df.first.bEncryption)
					for (auto& c : tempString)
						c = static_cast<char>(df.first.decryptionFile(c));

				readingFile.close();
				return tempString;
			}
		}
		readingFile.close();
	}
	return ""; 
}
std::string res::mLoad::load::operator()(const vp_AesGow& df) const 
{ 

	if (std::ifstream readingFile(archiveName, std::ios::in | std::ios::binary); readingFile.good())
	{
		for (int i = 0; i < df.first.fileNumber; ++i)
		{
			if (fileName == df.second[i].fileName)
			{
				if (df.first.bEncryption)
				{
					const unsigned long nReadCount = (df.second[i].filePos - sizeof(int) - sizeof(bool)) % 16;
					const size_t nEncSize = (df.second[i].fileSize + nReadCount + 15) & ~15;
					readingFile.seekg(df.second[i].filePos - nReadCount);

					std::string blockBuffer(nEncSize, '\0');
					std::string encryptedBuffer(nEncSize, '\0');
					readingFile.read(&encryptedBuffer[0], static_cast<long long>(nEncSize));
					readingFile.close();

					for (size_t j = 0; j < nEncSize / 16; ++j)
						cryptEngine::defAes::m_crypt.DecryptBlock(reinterpret_cast<unsigned char*>(&encryptedBuffer[j * 16]), 
							reinterpret_cast<unsigned char*>(&blockBuffer[j * 16]));

					encryptedBuffer.resize(0);
					if (static_cast<size_t>(nReadCount) + static_cast<size_t>(df.second[i].fileSize) <= blockBuffer.size())
						return blockBuffer;
				}
				else
				{
					std::string tempString;
					tempString.resize(df.second[i].fileSize);

					readingFile.seekg(df.second[i].filePos);
					readingFile.read(&tempString[0], df.second[i].fileSize);
					readingFile.close();

					return tempString;
				}
			}
		}
		readingFile.close();
	}
	return "";
}

std::string res::mLoad::load::operator()(const vp_AesAzure& df) const 
{ 
	unsigned long long crc = 0;
	if (!fileName.empty())
	{
		std::ranges::transform(fileName, fileName.begin(), [](const unsigned char c) { return std::tolower(c); });
		crc = cryptEngine::wbqt::CRC64::Compute(reinterpret_cast<const unsigned char*>(&fileName[0]), fileName.length());

		if (fileName == "model\\sfx_waterfall01.sfx")
		{
			std::cout << "wews" << fileName << std::endl;
			return "";
		}
	}

	if (std::ifstream readingFile(archiveName, std::ios::in | std::ios::binary); readingFile.good())
	{
		for (int i = 0; i < df.first.fileNumber; ++i)
		{
			if (crc == df.second[i].crc)
			{
#define UNENCRYPTED_BYTES (sizeof(short) + sizeof(short) + sizeof(bool) + sizeof(int))
				if (df.first.bEncryption)
				{
					const unsigned long nReadCount = (df.second[i].offset - UNENCRYPTED_BYTES) % 16;
					const size_t nEncSize = (df.second[i].size + nReadCount + 15) & ~15;

					std::string blockBuffer(nEncSize, '\0');
					std::string encryptedBuffer(nEncSize, '\0');
					readingFile.seekg(df.second[i].offset - nReadCount);
					readingFile.read(&encryptedBuffer[0], static_cast<long long>(nEncSize));
					readingFile.close();

					cryptEngine::defAes::m_crypt.Decrypt(reinterpret_cast<unsigned char*>(&encryptedBuffer[0]), reinterpret_cast<unsigned char*>(&blockBuffer[0]),
						static_cast<unsigned long>(nEncSize / 16ull), AES::BlockMode::ECB);

					encryptedBuffer.resize(0);
					std::string finalBuff(df.second[i].size, '\0');

					memcpy(&finalBuff[0], &blockBuffer[0] + nReadCount, df.second[i].size);
					return finalBuff;

				}
				std::string tempString;
				tempString.resize(df.second[i].size);

				readingFile.seekg(df.second[i].offset, std::ios::beg);
				readingFile.read(&tempString[0], df.second[i].size);
				readingFile.close();

				file::other::HdrAesAzure::rotr(reinterpret_cast<unsigned char*>(&tempString[0]), df.second[i].size);
				return tempString;
				
			}
		}
		readingFile.close();
	}
	return "";
}

std::string res::mLoad::load::operator()(const vp_Moon& df) const 
{
	if (std::ifstream readingFile(archiveName, std::ios::in | std::ios::binary); readingFile.good())
	{
		for (int i = 0; i < df.first.fileNumber; ++i)
		{
			if (fileName == df.second[i].fileName)
			{
				std::string tempString;
				tempString.resize(df.second[i].fileSize);

				readingFile.seekg(df.second[i].offset);
				readingFile.read(&tempString[0], df.second[i].fileSize);
				readingFile.close();

				if (df.first.isEncrpytion)
					for (auto& c : tempString)
						c = static_cast<char>(df.first.decryption(c));

				return tempString;
			}
		}
		readingFile.close();
	}
	return ""; 
}

std::string res::mLoad::load::operator()(const vp_Equinox& df) const 
{
	if (std::ifstream readingFile(archiveName, std::ios::in | std::ios::binary); readingFile.good())
	{
		for (int i = 0; i < df.first.fileNumber; ++i)
		{
			if (fileName == df.second[i].fileName)
			{
				std::string tempString;
				tempString.resize(df.second[i].fileSize);

				readingFile.seekg(df.second[i].offset);
				readingFile.read(&tempString[0], df.second[i].fileSize);
				readingFile.close();

				if (df.first.bEncrpytion)
					for (auto& c : tempString)
						c = static_cast<char>(file::other::HdrEquinox::decrpytion(c, df.second[i].encKey));

				return tempString;
			}
		}
		readingFile.close();
	}
	return ""; 
}

std::string res::mLoad::load::operator()(const vp_NewFeiFei& df) const
{
	if (fileName.empty())
		return "";

	std::ranges::transform(fileName, fileName.begin(), [](const unsigned char c) { return std::tolower(c); });
	const unsigned long hashId = file::feifei::string_id(fileName.c_str());

	if (std::ifstream readingFile(archiveName, std::ios::in | std::ios::binary); readingFile.good())
	{
		for (int i = 0; i < static_cast<int>(df.first.fileNumber); ++i)
		{
			if (hashId == df.second[i].uid)
			{
				std::string tempString;
				tempString.resize(df.second[i].size);

				readingFile.seekg(df.second[i].offset);
				readingFile.read(&tempString[0], df.second[i].size);
				readingFile.close();

				return tempString;
			}
		}
		readingFile.close();
	}
	return "";
}
