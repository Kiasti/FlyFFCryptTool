#include "resourceSettings.h"

#include "fileSys.h"
#include "Util.h"

void res::s::serverSetting::write(const std::string& serverName, std::ofstream& ofs) const
{	
	const long long serverNameLen = static_cast<unsigned char>(serverName.length());
	const long long extLen = static_cast<unsigned char>(extension.length());

	ofs.write(reinterpret_cast<const char*>(&type), sizeof(unsigned char));
	ofs.write(reinterpret_cast<const char*>(&serverNameLen), sizeof(unsigned char));
	ofs.write(reinterpret_cast<const char*>(&extLen), sizeof(unsigned char));
	ofs.write(&serverName[0], serverNameLen * static_cast<long long>(sizeof(char)));
	ofs.write(&extension[0], extLen * static_cast<long long>(sizeof(char)));

	switch (type)
	{
	case file::Type::def:
		case file::Type::smoke:
		{
			ofs.write(reinterpret_cast<const char*>(&encrpytionXor), sizeof(unsigned char));
			break;
		}
		case file::Type::aes1:
		case file::Type::aes2:
		{
			ofs.write(reinterpret_cast<const char*>(&enc_key[0]), static_cast<long long>(enc_key.size()) * static_cast<long long>(sizeof(unsigned char)));
			break;
		}
		case file::Type::florist:
		case file::Type::custom:
		{
			return;
		}
	}
	ofs.write(reinterpret_cast<const char*>(&sub.oneByte), sizeof(bool));
	ofs.write(reinterpret_cast<const char*>(&sub.compression), sizeof(bool));
	ofs.write(reinterpret_cast<const char*>(&sub.largeSupport), sizeof(bool));
}

std::string res::s::serverSetting::read(std::ifstream& ifs)
{
	ifs.read(reinterpret_cast<char*>(&type), sizeof(unsigned char));

	unsigned char serverNameLen;
	unsigned char extLen;
	ifs.read(reinterpret_cast<char*>(&serverNameLen), sizeof(unsigned char));
	ifs.read(reinterpret_cast<char*>(&extLen), sizeof(unsigned char));

	std::string serverName("", serverNameLen);
	extension.resize(extLen);
	ifs.read(&serverName[0], serverNameLen);
	ifs.read(&extension[0], extLen);

	switch (type)
	{
		case file::Type::def:
		case file::Type::smoke:
		{
			ifs.read(reinterpret_cast<char*>(&encrpytionXor), sizeof(unsigned char));
			break;
		}
		case file::Type::aes1:
		case file::Type::aes2:
		{
			ifs.read(reinterpret_cast<char*>(&enc_key[0]), static_cast<long long>(enc_key.size()));
			break;
		}
		case file::Type::florist:
		case file::Type::custom:
			return serverName;
		
	}
	ifs.read(reinterpret_cast<char*>(&sub.oneByte), sizeof(bool));
	ifs.read(reinterpret_cast<char*>(&sub.compression), sizeof(bool));
	ifs.read(reinterpret_cast<char*>(&sub.largeSupport), sizeof(bool));
	return serverName;
}

void res::s::serverSetting::writeStringFile(const std::string& serverName, std::ofstream& ofs) const
{
	ofs << std::to_string(static_cast<unsigned char>(type)) << "\n"
		<< std::to_string(serverName.length()) << "\n"
		<< std::to_string(extension.length()) << "\n"
		<< serverName << "\n"
		<< extension << "\n";

	switch (type)
	{
		case file::Type::def:
		case file::Type::smoke:
		{
			ofs << encrpytionXor << "\n";
			break;
		}
		case file::Type::aes1:
		case file::Type::aes2:
		{
			for (const unsigned char i : enc_key)
				ofs << i << " ";
				
			ofs << "\n";
			break;
		}
		case file::Type::florist:
		case file::Type::custom:
			return;
	}
	ofs << std::to_string(sub.oneByte) << "\n"
		<< std::to_string(sub.compression) << "\n"
		<< std::to_string(sub.largeSupport) << "\n";
}

std::string res::s::serverSetting::readStringFile(std::ifstream& ifs)
{
	std::string temp;
	ifs >> temp;
	type = static_cast<file::Type>(static_cast<unsigned char>(std::stoul(temp)));

	ifs >> temp;
	const unsigned char serverNameSize = static_cast<unsigned char>(std::stoul(temp));
	ifs >> temp;
	const unsigned char extSize = static_cast<unsigned char>(std::stoul(temp));

	std::string serverName("", serverNameSize);
	extension.resize(extSize);
	ifs >> serverName;
	ifs >> extension;
	

	switch (type)
	{
		case file::Type::def:
		case file::Type::florist:
		{
			ifs >> temp;
			encrpytionXor = static_cast<unsigned char>(std::stoul(temp));
			break;
		}
		case file::Type::aes1:
		case file::Type::aes2:
		{
			for (unsigned char& i : enc_key)
			{
				ifs >> temp;
				i = static_cast<unsigned char>(std::stoul(temp));
			}
			break;
		}
		case file::Type::custom:
		{
			return serverName;
		}
	}

	ifs >> temp;
	sub.oneByte = static_cast<bool>(std::stoul(temp));
	
	ifs >> temp;
	sub.compression = static_cast<bool>(std::stoul(temp));

	ifs >> temp;
	sub.largeSupport = static_cast<bool>(std::stoul(temp));
	return serverName;
}


void res::s::setting::loadIgnoreList()
{
	if (exists(clientDirectory))
	{
		if (std::ifstream ifs(clientDirectory.string() + "ext.bin", std::ios::in | std::ios::binary); ifs.good())
		{
			std::string temp;
			while (ifs >> temp)
				ignoreList.emplace_back(std::move(temp));
		}
	}
}



bool res::s::setting::setClientDirectory(const std::string& dir) const
{
	const auto path = std::filesystem::path(dir);
	if (exists(path))
	{
		clientDirectory = path;
		return true;
	}
	if (std::filesystem::create_directories(dir))
	{
		clientDirectory = path;
		return true;
	}
	return false;
}


void extS::ext::readBin()
{
	if (std::ifstream readingFile("ext.bin", std::ios::in | std::ios::binary); readingFile.good())
	{
		unsigned char servers = 0;
		readingFile.read(reinterpret_cast<char*>(&servers), sizeof(unsigned char));

		for (int i = 0; i < servers; ++i)
		{
			std::pair<std::string, serverSetting> info;
			info.first = info.second.read(readingFile);
			list.insert(std::move(info));
		}
	}
}

void extS::ext::saveBin() const
{
	if (std::ofstream ofs("ext.bin", std::ios::out | std::ios::binary); ofs.good())
	{
		unsigned char serverCount = list.size() >= std::numeric_limits<unsigned char>::max() ? std::numeric_limits<unsigned char>::max() : static_cast<unsigned char>(list.size());
		ofs.write(reinterpret_cast<char*>(&serverCount), sizeof(unsigned char));

		for (const auto& iter : list)
			iter.second.write(iter.first, ofs);
	}
}


void extS::ext::readStringFileBin()
{
	if (std::ifstream readingFile("extSettings.txt", std::ios::in | std::ios::binary); readingFile.good())
	{
		unsigned char servers = 0;
		readingFile >> servers;

		for (int i = 0; i < servers; ++i)
		{
			std::pair<std::string, serverSetting> info;
			info.first = info.second.read(readingFile);
			list.insert(std::move(info));
		}
	}
}

void extS::ext::saveStringFileBin() const
{
	if (std::ofstream ofs("extSettings", std::ios::out | std::ios::binary); ofs.good())
	{
		ofs << (list.size() >= std::numeric_limits<unsigned char>::max() ? std::numeric_limits<unsigned char>::max() : static_cast<unsigned char>(list.size()));

		for (const auto& iter : list)
			iter.second.write(iter.first, ofs);
	}
}








void packS::pack::readBin()
{
	if (std::ifstream readingFile("pack.bin", std::ios::in | std::ios::binary); readingFile.good())
		ss.first = ss.second.read(readingFile);
}

void packS::pack::saveBin() const
{
	if (std::ofstream ofs("pack.bin", std::ios::out | std::ios::binary); ofs.good())
		ss.second.write(ss.first, ofs);
}

void packS::pack::readStringFileBin()
{
	if (std::ifstream readingFile("packSettings.txt", std::ios::in | std::ios::binary); readingFile.good())
		ss.first = ss.second.readStringFile(readingFile);
}

void packS::pack::saveStringFileBin() const
{
	if (std::ofstream ofs("packSettings.txt", std::ios::out | std::ios::binary); ofs.good())
		ss.second.writeStringFile(ss.first, ofs);
}

void res::s::pack::pack::loadListToPack()
{
	if (exists(clientDirectory))
	{
		//ifstream vs fftextparser
		if (std::ifstream ifs(clientDirectory.string() + "packList.txt", std::ios::in | std::ios::binary); ifs.good())
		{
			while (!ifs.eof())
			{
				std::string type;
				ifs >> type;

				if (type == "res")
				{
					std::string resName;
					ifs >> resName;
					
					std::string temp;
					ifs >> temp;
					while (temp != "}")
					{
						std::string fileName;
						ifs >> fileName;

						if (std::filesystem::path p(fileName); exists(p))
							resourceToPath[resName].emplace_back(std::move(p));
						ifs >> temp;
					}										
				}
				else if (type == "bulkFolder")
				{
					std::string folderName;
					ifs >> folderName;
					std::string splitType;
					ifs >> splitType;

					std::string allowSubFolders;
					ifs >> allowSubFolders;
					bool allow = false;
					if (util::str::isNumber(allowSubFolders))
						allow = std::stoi(allowSubFolders);
					else if (allowSubFolders == "true")
						allow = true;
					else
						allow = false;

					//fs::folderTo	ResMap rm;
					if (splitType == "size")
					{
						std::string maxSize;
						ifs >> maxSize;
						if (util::str::isNumber(maxSize))
						{
							auto mSize = std::stoul(maxSize);
							

							
						}
					}
					else if (splitType == "none")
					{
						
					}
					else if (splitType == "letter") // will also subset mvr / sfx / etc?
					{
						// load all data at once

						
					}
					else if (splitType == "bulk")
					{
						
					}
					
				}
				else
				{
					// invalid
				}
				
				/*
				 *  res nameOfRes
				 *  {
				 *     filename
				 *  }
				 *  bulkFolder nameOfFolder splitType(size / letter / none)
				 */
			}

		}
	}
}
