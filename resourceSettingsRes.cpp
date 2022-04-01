#include "resourceSettingsRes.h"
#include "resourceSettings.h"

// todo: string version and maybe version using ffparser


//
// Forsaken
//
rSetting::ForsakenGlobalInfo::ForsakenGlobalInfo(std::ifstream& ifs, const rwIOType type)
{
	read(ifs, type);
}

void rSetting::ForsakenGlobalInfo::read(std::ifstream& ifs, const rwIOType type)
{
	switch (type)
	{
		case rwIOType::bin:
			ifs.read(sha_hash, static_cast<std::streamsize>(std::size(sha_hash)));
			ifs.read(secondExt, static_cast<std::streamsize>(std::size(secondExt)));
			ifs.read(reinterpret_cast<char*>(&saltPos), sizeof(cryptEngine::saltPos));
			break;
		case rwIOType::string:
		{
			ifs >> sha_hash;
			ifs >> secondExt;

			std::string tmp;
			ifs >> tmp;
			saltPos = static_cast<cryptEngine::saltPos>(std::stoi(tmp));
			break;
		}
		case rwIOType::ffParser:
			break;
	}


}

void rSetting::ForsakenGlobalInfo::write(std::ofstream& ofs, const rwIOType type) const
{
	switch (type)
	{
		case rwIOType::bin:
			ofs.write(sha_hash, static_cast<std::streamsize>(std::size(sha_hash)));
			ofs.write(secondExt, static_cast<std::streamsize>(std::size(secondExt)));
			ofs.write(reinterpret_cast<const char*>(&saltPos), sizeof(cryptEngine::saltPos));
			break;
		case rwIOType::string:
			ofs << sha_hash << '\t' << secondExt << '\t' << std::to_string(static_cast<int>(saltPos)) << '\n';
			break;
		case rwIOType::ffParser:
			break;
	}
}



//
// Equinox
//
rSetting::EquinoxGlobalInfo::EquinoxGlobalInfo(std::ifstream& ifs, const rwIOType type)
{
	read(ifs, type);
}

void rSetting::EquinoxGlobalInfo::read(std::ifstream& ifs, const rwIOType type)
{
	switch (type)
	{
		case rwIOType::bin:
			ifs.read(compareStr, static_cast<std::streamsize>(std::size(compareStr)));
			ifs.read(reinterpret_cast<char*>(keyArr.data()), static_cast<std::streamsize>(std::size(keyArr)));
			break;
		case rwIOType::string:
		{
			ifs >> compareStr;

			char wew[256]{};
			ifs >> wew;
			memcpy(keyArr._Elems, wew, std::size(wew));
			break;
		}
		case rwIOType::ffParser:
			break;
	}
}

void rSetting::EquinoxGlobalInfo::write(std::ofstream& ofs, const rwIOType type) const
{
	switch (type)
	{
		case rwIOType::bin:
			ofs.write(compareStr, static_cast<std::streamsize>(std::size(compareStr)));
			ofs.write(reinterpret_cast<const char*>(keyArr.data()), static_cast<std::streamsize>(std::size(keyArr)));
			break;
		case rwIOType::string:
		{
			ofs << compareStr << '\n';

			char wew[256]{};
			std::ranges::copy(keyArr, wew);
			ofs << wew << '\n';
			break;
		}
		case rwIOType::ffParser:
			break;
	}
}





// 
// InsigniaGlobalInfo
// 

rSetting::InsigniaGlobalInfo::InsigniaGlobalInfo(std::ifstream& ifs, const rwIOType type)
{
	read(ifs, type);
}

void rSetting::InsigniaGlobalInfo::read(std::ifstream& ifs, const rwIOType type)
{
	switch (type)
	{
		case rwIOType::bin:
			ifs.read(reinterpret_cast<char*>(&hdrKeyLen), sizeof(unsigned long));

			hdrKeys.reserve(hdrKeyLen);
			for (unsigned long i = 0; i < hdrKeyLen; ++i)
			{
				unsigned char key = 0x00;
				ifs.read(reinterpret_cast<char*>(&key), sizeof(unsigned char));
				hdrKeys.emplace_back(key);
			}

			for (int i = 0; i < 10; ++i)
			{
				ifs.read(reinterpret_cast<char*>(&fileTypeToKey[i].first), sizeof(unsigned char));
				ifs.read(reinterpret_cast<char*>(&fileTypeToKey[i].second), sizeof(unsigned char));
			}
			ifs.read(reinterpret_cast<char*>(&offsetPos), sizeof(unsigned short));
			break;
		case rwIOType::string:
		{
			std::string tmp;
			ifs >> tmp;
			hdrKeyLen = std::stoi(tmp);

			hdrKeys.reserve(hdrKeyLen);
			for (unsigned long i = 0; i < hdrKeyLen; ++i)
			{
				ifs >> tmp;
				unsigned char key = static_cast<unsigned char>(std::stoi(tmp));
				hdrKeys.emplace_back(key);
			}

			for (int i = 0; i < 10; ++i)
			{
				ifs.read(reinterpret_cast<char*>(&fileTypeToKey[i].first), sizeof(unsigned char));
				ifs.read(reinterpret_cast<char*>(&fileTypeToKey[i].second), sizeof(unsigned char));
			}
			ifs.read(reinterpret_cast<char*>(&offsetPos), sizeof(unsigned short));
			break;
		}
		case rwIOType::ffParser:
			break;
	}
}

void rSetting::InsigniaGlobalInfo::write(std::ofstream& ofs, const rwIOType type) const
{
	switch (type)
	{
		case rwIOType::bin:
			ofs.write(reinterpret_cast<const char*>(&hdrKeyLen), sizeof(unsigned long));

			for (const auto& key : hdrKeys)
				ofs.write(reinterpret_cast<const char*>(&key), sizeof(unsigned char));

			for (int i = 0; i < 10; ++i)
			{
				ofs.write(reinterpret_cast<const char*>(&fileTypeToKey[i].first), sizeof(unsigned char));
				ofs.write(reinterpret_cast<const char*>(&fileTypeToKey[i].second), sizeof(unsigned char));
			}
			ofs.write(reinterpret_cast<const char*>(&offsetPos), sizeof(unsigned short));
			break;
		case rwIOType::string:
		{
			ofs << std::to_string(hdrKeyLen) << '\n';
			for (unsigned long i = 0; i < hdrKeyLen; ++i)
				ofs << hdrKeys[i] << ' ';

			ofs << '\n';
			for (int i = 0; i < 10; ++i)
				ofs << std::to_string(fileTypeToKey[i].first) << ' ' << std::to_string(fileTypeToKey[i].second) << '\n';

			ofs << std::to_string(offsetPos) << '\n';
			break;
		}
		case rwIOType::ffParser:
			break;
	}


}



//
//CloudGlobalInfo
//
rSetting::CloudGlobalInfo::CloudGlobalInfo(std::ifstream& ifs, const rwIOType type)
{
	read(ifs, type);
}

void rSetting::CloudGlobalInfo::read(std::ifstream& ifs, const rwIOType type)
{
	switch (type)
	{
		case rwIOType::bin:
			ifs.read(reinterpret_cast<char*>(keyTable.data()), static_cast<std::streamsize>(std::size(keyTable)));
			break;
		case rwIOType::string:
		{
			std::string tmp;
			for (unsigned long i = 0; i < std::size(keyTable); ++i)
			{
				ifs >> tmp;
				keyTable[i] = static_cast<unsigned char>(std::stoi(tmp));
			}
			break;
		}
		case rwIOType::ffParser:
			break;
	}
}

void rSetting::CloudGlobalInfo::write(std::ofstream& ofs, const rwIOType type) const
{
	switch (type)
	{
		case rwIOType::bin:
			ofs.write(reinterpret_cast<const char*>(keyTable.data()), static_cast<std::streamsize>(std::size(keyTable)));
			break;
		case rwIOType::string:
			for (const auto& elem : keyTable)
				ofs << std::to_string(elem) << ' ';
			ofs << '\n';
			break;
		case rwIOType::ffParser:
			break;
	}
}




//
// MoonGlobalInfo
//
rSetting::MoonGlobalInfo::MoonGlobalInfo(std::ifstream& ifs, const rwIOType type)
{
	read(ifs, type);
}

void rSetting::MoonGlobalInfo::read(std::ifstream& ifs, const rwIOType type)
{
	switch (type)
	{
		case rwIOType::bin:
			ifs.read(reinterpret_cast<char*>(&hardcodedKey), sizeof(unsigned char));
			break;
		case rwIOType::string:
		{
			std::string tmp;
			ifs >> tmp;
			hardcodedKey = static_cast<unsigned char>(std::stoul(tmp));
			break;
		}
		case rwIOType::ffParser:
			break;
	}
}

void rSetting::MoonGlobalInfo::write(std::ofstream& ofs, const rwIOType type) const
{
	switch (type)
	{
		case rwIOType::bin:
			ofs.write(reinterpret_cast<const char*>(&hardcodedKey), sizeof(unsigned char));
			break;
		case rwIOType::string:
			ofs << std::to_string(hardcodedKey) << '\n';
			break;
		case rwIOType::ffParser:
			break;
	}
}



//
// AesGlobalInfo
//

rSetting::AesGlobalInfo::AesGlobalInfo(std::ifstream& ifs, const rwIOType type)
{
	AesGlobalInfo::read(ifs, type);
}

void rSetting::AesGlobalInfo::read(std::ifstream& ifs, const rwIOType type)
{
	switch (type)
	{
		case rwIOType::bin:
			ifs.read(reinterpret_cast<char*>(keyArr.data()), static_cast<std::streamsize>(std::size(keyArr)));
			break;
		case rwIOType::string:
			for (auto& elem : keyArr)
			{
				std::string tmp;
				ifs >> tmp;
				elem = static_cast<unsigned char>(std::stoul(tmp));
			}
			break;
		case rwIOType::ffParser:
			break;
	}


}

void rSetting::AesGlobalInfo::write(std::ofstream& ofs, const rwIOType type) const
{
	switch (type)
	{
		case rwIOType::bin:
			ofs.write(reinterpret_cast<const char*>(keyArr.data()), static_cast<std::streamsize>(std::size(keyArr)));
			break;
		case rwIOType::string:
			for (const auto& elem : keyArr)
				ofs << std::to_string(elem);
			break;
		case rwIOType::ffParser:
			break;
	}


}



//
// inners
//
void rSetting::DefaultInnerInfo::read(std::ifstream& ifs, const rwIOType type)
{
	switch (type)
	{
		case rwIOType::bin:
			break;
		case rwIOType::string:
		{
			std::string tmp;
			ifs >> tmp;

			bEncrpytion = static_cast<bool>(std::stoi(tmp));
			ifs >> tmp;

			xorKey = static_cast<unsigned char>(std::stoul(tmp));
			ifs >> ver;
			break;
		}
		case rwIOType::ffParser:
			break;
	}

	ifs.read(reinterpret_cast<char*>(&bEncrpytion), sizeof(bool));
	ifs.read(reinterpret_cast<char*>(&xorKey), sizeof(unsigned char));
	ifs.read(ver, static_cast<std::streamsize>(std::size(ver)));
}

void rSetting::DefaultInnerInfo::write(std::ofstream& ofs, const rwIOType type) const
{
	switch (type)
	{
		case rwIOType::bin:
			ofs.write(reinterpret_cast<const char*>(&bEncrpytion), sizeof(bool));
			ofs.write(reinterpret_cast<const char*>(&xorKey), sizeof(unsigned char));
			ofs.write(ver, static_cast<std::streamsize>(std::size(ver)));
			break;
		case rwIOType::string:
			ofs << std::to_string(bEncrpytion) << ' ' << std::to_string(xorKey) << ' ' << ver << '\n';
			break;
		case rwIOType::ffParser:
			break;
	}
}



//
// InsigniaInnerInfo
//
void rSetting::InsigniaInnerInfo::read(std::ifstream& ifs, const rwIOType type)
{
	switch (type)
	{
		case rwIOType::bin:
			break;
		case rwIOType::string:
		{
			std::string tmp;
			ifs >> tmp;
			fileType = static_cast<unsigned char>(std::stoul(tmp));
			break;
		}
		case rwIOType::ffParser:
			break;
	}

	ifs.read(reinterpret_cast<char*>(&fileType), sizeof(unsigned char));
}

void rSetting::InsigniaInnerInfo::write(std::ofstream& ofs, const rwIOType type) const
{
	switch (type)
	{
		case rwIOType::bin:
			ofs.write(reinterpret_cast<const char*>(&fileType), sizeof(unsigned char));
			break;
		case rwIOType::string:
			ofs << std::to_string(fileType) << '\n';
			break;
		case rwIOType::ffParser:
			break;
	}


}



//
// AesInnerInfo
//
void rSetting::AesInnerInfo::read(std::ifstream& ifs, const rwIOType type)
{
	switch (type)
	{
		case rwIOType::bin:
			ifs.read(reinterpret_cast<char*>(&bEncrpyt), sizeof(bool));
			break;
		case rwIOType::string:
		{
			std::string tmp;
			ifs >> tmp;
			bEncrpyt = static_cast<bool>(std::stoul(tmp));
			break;
		}
		case rwIOType::ffParser:
			break;
	}

}

void rSetting::AesInnerInfo::write(std::ofstream& ofs, const rwIOType type) const
{
	switch (type)
	{
		case rwIOType::bin:
			ofs.write(reinterpret_cast<const char*>(&bEncrpyt), sizeof(bool));
			break;
		case rwIOType::string:
			ofs << std::to_string(bEncrpyt) << '\n';
			break;
		case rwIOType::ffParser:
			break;
	}
}


//
// EquinoxInnerInfo
//

void rSetting::EquinoxInnerInfo::read(std::ifstream& ifs, const rwIOType type)
{
	switch (type)
	{
		case rwIOType::bin:
			ifs.read(reinterpret_cast<char*>(&hdrLenKey), sizeof(unsigned char));
			ifs.read(reinterpret_cast<char*>(&innerKey), sizeof(unsigned char));
			ifs.read(reinterpret_cast<char*>(&version), sizeof(unsigned short));
			ifs.read(reinterpret_cast<char*>(&bEncrpyt), sizeof(bool));
			ifs.read(reinterpret_cast<char*>(&fileKey), sizeof(unsigned char));
			break;
		case rwIOType::string:
		{
			std::string tmp;
			ifs >> tmp;

			hdrLenKey = static_cast<unsigned char>(std::stoul(tmp));
			ifs >> tmp;
			innerKey = static_cast<unsigned char>(std::stoul(tmp));
			ifs >> tmp;
			version = static_cast<unsigned short>(std::stoul(tmp));
			ifs >> tmp;
			bEncrpyt = static_cast<bool>(std::stoul(tmp));
			ifs >> tmp;
			fileKey = static_cast<unsigned char>(std::stoul(tmp));
			break;
		}
		case rwIOType::ffParser:
			break;
	}
}

void rSetting::EquinoxInnerInfo::write(std::ofstream& ofs, const rwIOType type) const
{
	switch (type)
	{
		case rwIOType::bin:
			ofs.write(reinterpret_cast<const char*>(&hdrLenKey), sizeof(unsigned char));
			ofs.write(reinterpret_cast<const char*>(&innerKey), sizeof(unsigned char));
			ofs.write(reinterpret_cast<const char*>(&version), sizeof(unsigned short));
			ofs.write(reinterpret_cast<const char*>(&bEncrpyt), sizeof(bool));
			ofs.write(reinterpret_cast<const char*>(&fileKey), sizeof(unsigned char));
			break;
		case rwIOType::string:
			ofs << std::to_string(hdrLenKey) << ' ' << std::to_string(innerKey) << ' ' << std::to_string(version) << ' ' << std::to_string(bEncrpyt)
				<< ' ' << std::to_string(fileKey);
			break;
		case rwIOType::ffParser:
			break;
	}
}




//
// MoonInnerInfo
//

void rSetting::MoonInnerInfo::read(std::ifstream& ifs, const rwIOType type)
{
	switch (type)
	{
		case rwIOType::bin:
			ifs.read(reinterpret_cast<char*>(&key), sizeof(unsigned char));
			ifs.read(reinterpret_cast<char*>(&bEncrpyt), sizeof(bool));
			ifs.read(ver, static_cast<std::streamsize>(std::size(ver)));
			break;
		case rwIOType::string:
		{
			std::string tmp;
			ifs >> tmp;
			key = static_cast<unsigned char>(std::stoul(tmp));
			ifs >> tmp;
			bEncrpyt = static_cast<bool>(std::stoul(tmp));
			ifs >> ver;
			break;
		}
		case rwIOType::ffParser:
			break;
	}
}

void rSetting::MoonInnerInfo::write(std::ofstream& ofs, const rwIOType type) const
{
	switch (type)
	{
		case rwIOType::bin:
			ofs.write(reinterpret_cast<const char*>(&key), sizeof(unsigned char));
			ofs.write(reinterpret_cast<const char*>(&bEncrpyt), sizeof(bool));
			ofs.write(ver, static_cast<std::streamsize>(std::size(ver)));
			break;
		case rwIOType::string:
			ofs << std::to_string(key) << ' ' << std::to_string(bEncrpyt) << ' ' << ver << '\n';
			break;
		case rwIOType::ffParser:
			break;
	}

}


//
// Main Info
//
void rSetting::MainInfo::read(std::ifstream& ifs, const rwIOType type)
{
	switch (type)
	{
		case rwIOType::bin:
			ifs.read(reinterpret_cast<char*>(&this->type), sizeof(file::Type));
			ifs.read(reinterpret_cast<char*>(&id), sizeof(int));
			ifs.read(serverName, static_cast<std::streamsize>(std::size(serverName)));
			ifs.read(extension, static_cast<std::streamsize>(std::size(extension)));
			break;
		case rwIOType::string:
		{
			std::string tmp;
			ifs >> tmp;
			this->type = static_cast<file::Type>(std::stoi(tmp));

			ifs >> tmp;
			id = std::stoi(tmp);
			ifs >> serverName;
			ifs >> extension;
			break;
		}
		case rwIOType::ffParser:
			break;
	}
}


void rSetting::MainInfo::write(std::ofstream& ofs, const rwIOType type) const
{
	switch (type)
	{
		case rwIOType::bin:
			ofs.write(reinterpret_cast<const char*>(&this->type), sizeof(file::Type));
			ofs.write(reinterpret_cast<const char*>(&id), sizeof(int));
			ofs.write(serverName, static_cast<std::streamsize>(std::size(serverName)));
			ofs.write(extension, static_cast<std::streamsize>(std::size(extension)));
			break;
		case rwIOType::string:
			ofs << std::to_string(static_cast<int>(type)) << '\t' << std::to_string(id) << '\n' << serverName << '\n' << extension;
			break;
		case rwIOType::ffParser:
			break;
	}
}



