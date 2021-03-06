#include "resourceExtractor.h"

#include <iostream>

#include "compressionEngine.h"
#include "CRC64.h"
#include "cryptEngine.h"
#include "fileSys.h"

void res::extract::all::operator()(const vp_Flyff& df) const
{
	if (std::ifstream readingFile(archiveName, std::ios::in | std::ios::binary); readingFile.good())
	{
		for (int i = 0; i < df.first.fileNumber; ++i)
		{
			if (fileName.empty() || fileName == df.second[i].fileName)
			{
				std::filesystem::path p = archiveName;
				p.remove_filename();

				std::string outStr = p.string() + (!fileName.empty() ? fileName : df.second[i].fileName);
				p = outStr;
				p.remove_filename();

				if (!exists(p))
					create_directories(p);


				std::string tempString;
				tempString.resize(df.second[i].fileSize);

				readingFile.seekg(df.second[i].filePos);
				readingFile.read(&tempString[0], df.second[i].fileSize);

				if (df.first.bEncryption)
					for (auto& c : tempString)
						c = static_cast<char>(df.first.decryption(c));

				{
					std::scoped_lock lck(outputFile);
					std::cout << "extracting " << df.second[i].fileName << " to: " << outStr << "\n";
				}

				std::ofstream ofs(outStr, std::ios::out | std::ios::binary);
				ofs.write(&tempString[0], static_cast<long long>(tempString.length()));
				ofs.close();
			}
		}
		readingFile.close();
	}
}

void res::extract::all::operator()(const vp_Insignia& df) const
{
	if (std::ifstream readingFile(archiveName, std::ios::in | std::ios::binary); readingFile.good())
	{
		for (int i = 0; i < df.first.fileNumber; ++i)
		{
			if (fileName.empty() || fileName == df.second[i].fileName)
			{
				std::filesystem::path p = archiveName;
				p.remove_filename();

				std::string outStr = p.string() + (!fileName.empty() ? fileName : df.second[i].fileName);
				p = outStr;
				p.remove_filename();

				if (!exists(p))
					create_directories(p);


				std::string tempString;
				tempString.resize(df.second[i].fileSize);

				readingFile.seekg(df.second[i].filePos);
				readingFile.read(&tempString[0], df.second[i].fileSize);

				if (df.first.bEncryption)
					for (auto& c : tempString)
						c = static_cast<char>(df.first.decryptionFile(c));


				{
					std::scoped_lock lck(outputFile);
					std::cout << "extracting " << df.second[i].fileName << " to: " << outStr << "\n";
				}

				std::ofstream ofs(outStr, std::ios::out | std::ios::binary);
				ofs.write(&tempString[0], static_cast<long long>(tempString.length()));
				ofs.close();
			}
		}
		readingFile.close();
	}
}


void res::extract::all::operator()(const vp_AesGow& df) const
{
	if (std::ifstream readingFile(archiveName, std::ios::in | std::ios::binary); readingFile.good())
	{
		for (int i = 0; i < df.first.fileNumber; ++i)
		{
			if (fileName.empty() || fileName == df.second[i].fileName)
			{
				std::filesystem::path p = archiveName;
				p.remove_filename(); 

				std::string outStr = p.string() + (!fileName.empty() ? fileName : df.second[i].fileName);
				p = outStr;
				p.remove_filename(); 

				if (!exists(p))
					create_directories(p);

				if (df.first.bEncryption)
				{
					unsigned long nReadCount = (df.second[i].filePos - sizeof(int) - sizeof(bool)) % 16;
					const size_t nEncSize = (df.second[i].fileSize + nReadCount + 15) & ~15;
					readingFile.seekg(df.second[i].filePos - nReadCount);

					std::vector<unsigned char> blockBuffer(nEncSize);
					std::vector<unsigned char> encryptedBuffer(nEncSize);
					readingFile.read(reinterpret_cast<char*>(&encryptedBuffer[0]), static_cast<long long>(nEncSize));

					for (size_t j = 0; j < nEncSize / 16; ++j)
						cryptEngine::defAes::m_crypt.DecryptBlock(&encryptedBuffer[j * 16], &blockBuffer[j * 16]);


					encryptedBuffer.resize(0);
					if (static_cast<size_t>(nReadCount) + static_cast<size_t>(df.second[i].fileSize) <= blockBuffer.size())
					{
						{
							std::scoped_lock lck(outputFile);
							std::cout << "extracting " << df.second[i].fileName << " to: " << outStr << "\n";
						}
						std::ofstream ofs(outStr, std::ios::out | std::ios::binary);
						ofs.write(reinterpret_cast<char*>(&blockBuffer[0]) + nReadCount, df.second[i].fileSize);
						ofs.close();
					}
				}
				else
				{
					std::string tempString;
					tempString.resize(df.second[i].fileSize);

					readingFile.seekg(df.second[i].filePos);
					readingFile.read(&tempString[0], df.second[i].fileSize);

					{
						std::scoped_lock lck(outputFile);
						std::cout << "extracting " << df.second[i].fileName << " to: " << outStr << "\n";
					}
					std::ofstream ofs(df.second[i].fileName, std::ios::out | std::ios::binary);
					ofs.write(&tempString[0], static_cast<long long>(tempString.length()));
					ofs.close();
				}
			}
		}
		readingFile.close();
	}
}

void res::extract::all::operator()(const vp_AesAzure& df) const
{
	unsigned long long crc = 0;
	if (!fileName.empty())
	{
		std::ranges::transform(fileName, fileName.begin(), [](const unsigned char c) { return std::tolower(c); });
		crc = cryptEngine::wbqt::CRC64::Compute(reinterpret_cast<const unsigned char*>(&fileName[0]), fileName.length());
	}

	if (std::ifstream readingFile(archiveName, std::ios::in | std::ios::binary); readingFile.good())
	{
		for (int i = 0; i < df.first.fileNumber; ++i)
		{
			if (crc == 0|| crc == df.second[i].crc)
			{
				
				std::filesystem::path p = archiveName;
				p.remove_filename();

				std::string outStr = p.string() + (!fileName.empty() ? fileName : std::to_string(df.second[i].crc) + ".txt");
				p = outStr;
				p.remove_filename();

				if (!exists(p))
					create_directories(p);

				{
					std::scoped_lock lck(outputFile);
					std::cout << "extracting " << df.second[i].crc << " to: " << outStr << "\n";
				}

				if (df.first.bEncryption)
				{
#define UNENCRYPTED_BYTES (sizeof(short) + sizeof(short) + sizeof(bool) + sizeof(int))
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
					{
						std::ofstream ofs(outStr, std::ios::out | std::ios::binary);
						ofs.write(&finalBuff[0], df.second[i].size);
						ofs.close();
					}
				}
				else
				{
					std::string tempString;
					tempString.resize(df.second[i].size);

					readingFile.seekg(df.second[i].offset, std::ios::beg);
					readingFile.read(&tempString[0], df.second[i].size);
					
					file::other::HdrAesAzure::rotr(reinterpret_cast<unsigned char*>(&tempString[0]), df.second[i].size);

					std::ofstream ofs(outStr, std::ios::out | std::ios::binary);
					ofs.write(&tempString[0], df.second[i].size);
					ofs.close();

				}
			}
		}
		readingFile.close();
	}
}

void res::extract::all::operator()(const vp_Moon& df) const
{
	if (std::ifstream readingFile(archiveName, std::ios::in | std::ios::binary); readingFile.good())
	{
		for (int i = 0; i < df.first.fileNumber; ++i)
		{
			if (fileName.empty() || fileName == df.second[i].fileName)
			{
				std::filesystem::path p = archiveName;
				p.remove_filename();

				std::string outStr = p.string() + (!fileName.empty() ? fileName : df.second[i].fileName);
				p = outStr;
				p.remove_filename();

				if (!exists(p))
					create_directories(p);


				std::string tempString;
				tempString.resize(df.second[i].fileSize);

				readingFile.seekg(df.second[i].offset);
				readingFile.read(&tempString[0], df.second[i].fileSize);

				if (df.first.isEncrpytion)
					for (auto& c : tempString)
						c = static_cast<char>(df.first.decryption(c));

				{
					std::scoped_lock lck(outputFile);
					std::cout << "extracting " << df.second[i].fileName << "\n";
				}

				std::ofstream ofs(outStr, std::ios::out | std::ios::binary);
				ofs.write(&tempString[0], static_cast<long long>(tempString.length()));
				ofs.close();
			}
		}
		readingFile.close();
	}
}

void res::extract::all::operator()(const vp_Equinox& df) const
{
	if (std::ifstream readingFile(archiveName, std::ios::in | std::ios::binary); readingFile.good())
	{
		for (int i = 0; i < df.first.fileNumber; ++i)
		{
			if (fileName.empty() || fileName == df.second[i].fileName)
			{
				std::filesystem::path p = archiveName;
				p.remove_filename();

				std::string outStr = p.string() + (!fileName.empty() ? fileName : df.second[i].fileName);
				p = outStr;
				p.remove_filename();

				if (!exists(p))
					create_directories(p);

				std::string tempString;
				tempString.resize(df.second[i].fileSize);

				readingFile.seekg(df.second[i].offset);
				readingFile.read(&tempString[0], df.second[i].fileSize);

				if (df.first.bEncrpytion)
					for (auto& c : tempString)
						c = static_cast<char>(file::other::HdrEquinox::decrpytion(c, df.second[i].encKey));

				{
					std::scoped_lock lck(outputFile);
					std::cout << "extracting " << df.second[i].fileName << " to: " << outStr << "\n";
				}

				std::ofstream ofs(outStr, std::ios::out | std::ios::binary);
				ofs.write(&tempString[0], static_cast<long long>(tempString.length()));
				ofs.close();
			}
		}
		readingFile.close();
	}
}


void res::extract::all::operator()(const vp_Forsaken& df) const
{
	std::ranges::transform(fileName, fileName.begin(), [](const unsigned char c) { return std::tolower(c); });
	std::string cpyName = fileName;
	if (!fileName.empty())
		cpyName = cryptEngine::getSha256(fileName, cryptEngine::shaHash_Forskaen);

	std::filesystem::path tempPath = archiveName;
	//replace .fast with .fly

	std::string xorKey = fileName;
	tempPath.replace_extension(".fly");
	if (exists(tempPath))
	{
		archiveName = tempPath.string();
		if (std::ifstream readingFile(archiveName, std::ios::in | std::ios::binary); readingFile.good())
		{
			for (int i = 0; i < static_cast<int>(df.first.fileNumber); ++i)
			{
				if (cpyName.empty() || cpyName == df.second[i].fileName)
				{
					std::filesystem::path p = archiveName;
					p.remove_filename();

					std::string outStr = p.string() + (!fileName.empty() ? fileName : df.second[i].fileName);
					p = outStr;
					p.remove_filename();

					if (!exists(p))
						create_directories(p);

					std::string tempString;
					tempString.resize(df.second[i].fileSize);

					readingFile.seekg(df.second[i].offset);
					readingFile.read(&tempString[0], df.second[i].fileSize);
					
					if (!xorKey.empty())
					{
						unsigned long fileNameCount = 0;
						for (char& j : tempString)
						{
							unsigned char fNameChar = xorKey[fileNameCount];
							unsigned char result = ~j ^ fNameChar * -1;
							result = static_cast<unsigned char>((result << 4) | (result >> 4));
							j = static_cast<char>(result);

							fileNameCount++;
							if (fileNameCount >= xorKey.length())
								fileNameCount = 0;
						}
					}

					{
						std::scoped_lock lck(outputFile);
						std::cout << "extracting " << df.second[i].fileName << " to: " << outStr << "\n";
					}

					std::ofstream ofs(outStr, std::ios::out | std::ios::binary);
					ofs.write(&tempString[0], static_cast<long long>(tempString.length()));
					ofs.close();
				}
			}
			readingFile.close();
		}
	}
}



void res::extract::all::operator()(const vp_NewFeiFei& df) const
{
	std::ranges::transform(fileName, fileName.begin(), [](const unsigned char c) { return std::tolower(c); });
	unsigned long hashId = 0;
	const bool isEmpty = fileName.empty();
	if (!isEmpty)
		hashId = file::feifei::string_id(fileName.c_str());

	if (std::ifstream readingFile(archiveName, std::ios::in | std::ios::binary); readingFile.good())
	{
		for (int i = 0; i < static_cast<int>(df.first.fileNumber); ++i)
		{
			if (isEmpty || hashId == df.second[i].uid)
			{
				std::filesystem::path p = archiveName;
				p.remove_filename();

				std::string outStr = p.string() + (!fileName.empty() ? fileName : std::to_string(df.second[i].uid) + ".bin");
				p = outStr;
				p.remove_filename();

				if (!exists(p))
					create_directories(p);

				std::string tempString;
				tempString.resize(df.second[i].size);

				readingFile.seekg(df.second[i].offset);
				readingFile.read(&tempString[0], df.second[i].size);

				{
					std::scoped_lock lck(outputFile);
					std::cout << "extracting " << df.second[i].uid << " to: " << outStr << "\n";
				}

				std::ofstream ofs(outStr, std::ios::out | std::ios::binary);
				ofs.write(&tempString[0], static_cast<long long>(tempString.length()));
				ofs.close();
			}
		}
		readingFile.close();
	}
}


// todo: group vp_cloud and vp_flyff a bit.
void res::extract::all::operator()(const vp_Cloud& df) const
{
	if (std::ifstream readingFile(archiveName, std::ios::in | std::ios::binary); readingFile.good())
	{
		for (int i = 0; i < df.first.fileNumber; ++i)
		{
			if (fileName.empty() || fileName == df.second[i].fileName)
			{
				std::filesystem::path p = archiveName;
				p.remove_filename();

				std::string outStr = p.string() + (!fileName.empty() ? fileName : df.second[i].fileName);
				p = outStr;
				p.remove_filename();

				if (!exists(p))
					create_directories(p);

				std::string tempString;
				tempString.resize(df.second[i].fileSize);

				readingFile.seekg(df.second[i].filePos);
				readingFile.read(&tempString[0], df.second[i].fileSize);

				if (df.first.bEncryption)
					for (auto& c : tempString)
						c = static_cast<char>(df.first.decryption(c));

				{
					std::scoped_lock lck(outputFile);
					std::cout << "extracting " << df.second[i].fileName << " to: " << outStr << "\n";
				}

				std::ofstream ofs(outStr, std::ios::out | std::ios::binary);
				ofs.write(&tempString[0], static_cast<long long>(tempString.length()));
				ofs.close();
			}
		}
		readingFile.close();
	}
}
