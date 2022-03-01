#include "resourceSettingsRes.h"
#include "resourceSettings.h"

// todo: string version and maybe version using ffparser


//
// Forsaken
//
rSetting::ForsakenGlobalInfo::ForsakenGlobalInfo(std::ifstream& ifs, const bool bin)
{
	if (bin)
		read(ifs);
	else
		readStringFile(ifs);
}

void rSetting::ForsakenGlobalInfo::read(std::ifstream& ifs)
{
	ifs.read(sha_hash, static_cast<std::streamsize>(std::size(sha_hash)));
	ifs.read(secondExt, static_cast<std::streamsize>(std::size(secondExt)));
	ifs.read(reinterpret_cast<char*>(&saltPos), sizeof(cryptEngine::saltPos));
}

void rSetting::ForsakenGlobalInfo::write(std::ofstream& ofs) const
{
	ofs.write(sha_hash, static_cast<std::streamsize>(std::size(sha_hash)));
	ofs.write(secondExt, static_cast<std::streamsize>(std::size(secondExt)));
	ofs.write(reinterpret_cast<const char*>(&saltPos), sizeof(cryptEngine::saltPos));
}

void rSetting::ForsakenGlobalInfo::readStringFile(std::ifstream& ifs)
{
	ifs >> sha_hash;
	ifs >> secondExt;
	
	std::string tmp;
	ifs >> tmp;
	saltPos = static_cast<cryptEngine::saltPos>(std::stoi(tmp));
}

void rSetting::ForsakenGlobalInfo::writeStringFile(std::ofstream& ofs) const
{
	ofs << sha_hash << '\t' << secondExt << '\t' << std::to_string(static_cast<int>(saltPos)) << '\n';
}



//
// Equinox
//
rSetting::EquinoxGlobalInfo::EquinoxGlobalInfo(std::ifstream& ifs, const bool bin)
{
	if (bin)
		read(ifs);
	else
		readStringFile(ifs);
}

void rSetting::EquinoxGlobalInfo::read(std::ifstream& ifs)
{
	ifs.read(compareStr, static_cast<std::streamsize>(std::size(compareStr)));
	ifs.read(reinterpret_cast<char*>(keyArr.data()), static_cast<std::streamsize>(std::size(keyArr)));
}

void rSetting::EquinoxGlobalInfo::write(std::ofstream& ofs) const
{
	ofs.write(compareStr, static_cast<std::streamsize>(std::size(compareStr)));
	ofs.write(reinterpret_cast<const char*>(keyArr.data()), static_cast<std::streamsize>(std::size(keyArr)));
}

void rSetting::EquinoxGlobalInfo::readStringFile(std::ifstream& ifs)
{
	ifs >> compareStr;

	char wew[256]{};
	ifs >> wew;
	memcpy(keyArr._Elems, wew, std::size(wew));
}

void rSetting::EquinoxGlobalInfo::writeStringFile(std::ofstream& ofs) const
{
	ofs << compareStr << '\n';
	
	char wew[256]{};
	std::ranges::copy(keyArr, wew);
	ofs << wew << '\n';
}



// 
// InsigniaGlobalInfo
// 

rSetting::InsigniaGlobalInfo::InsigniaGlobalInfo(std::ifstream& ifs, const bool bin)
{
	if (bin)
		read(ifs);
	else
		readStringFile(ifs);
}

void rSetting::InsigniaGlobalInfo::read(std::ifstream& ifs)
{
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
}

void rSetting::InsigniaGlobalInfo::write(std::ofstream& ofs) const
{
	ofs.write(reinterpret_cast<const char*>(&hdrKeyLen), sizeof(unsigned long));

	for (const auto& key : hdrKeys)
		ofs.write(reinterpret_cast<const char*>(&key), sizeof(unsigned char));

	for (int i = 0; i < 10; ++i)
	{
		ofs.write(reinterpret_cast<const char*>(&fileTypeToKey[i].first), sizeof(unsigned char));
		ofs.write(reinterpret_cast<const char*>(&fileTypeToKey[i].second), sizeof(unsigned char));
	}
	ofs.write(reinterpret_cast<const char*>(&offsetPos), sizeof(unsigned short));
}

void rSetting::InsigniaGlobalInfo::readStringFile(std::ifstream& ifs)
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
}

void rSetting::InsigniaGlobalInfo::writeStringFile(std::ofstream& ofs) const
{
	ofs << std::to_string(hdrKeyLen) << '\n';
	for (unsigned long i = 0; i < hdrKeyLen; ++i)
		ofs << hdrKeys[i] << ' ';

	ofs << '\n';
	for (int i = 0; i < 10; ++i)
		ofs << std::to_string(fileTypeToKey[i].first) << ' ' << std::to_string(fileTypeToKey[i].second) << '\n';

	ofs << std::to_string(offsetPos) << '\n';
}



//
//CloudGlobalInfo
//
rSetting::CloudGlobalInfo::CloudGlobalInfo(std::ifstream& ifs, const bool bin)
{
	if (bin)
		read(ifs);
	else
		readStringFile(ifs);
}

void rSetting::CloudGlobalInfo::read(std::ifstream& ifs)
{
	ifs.read(reinterpret_cast<char*>(keyTable.data()), static_cast<std::streamsize>(std::size(keyTable)));
}

void rSetting::CloudGlobalInfo::write(std::ofstream& ofs) const
{
	ofs.write(reinterpret_cast<const char*>(keyTable.data()), static_cast<std::streamsize>(std::size(keyTable)));
}

void rSetting::CloudGlobalInfo::readStringFile(std::ifstream& ifs)
{	
	std::string tmp;
	for (unsigned long i = 0; i < std::size(keyTable); ++i)
	{
		ifs >> tmp;
		keyTable[i] = static_cast<unsigned char>(std::stoi(tmp));
	}
}

void rSetting::CloudGlobalInfo::writeStringFile(std::ofstream& ofs) const
{	
	for (const auto& elem : keyTable)
		ofs << std::to_string(elem) << ' ';
	ofs << '\n';
}



//
// MoonGlobalInfo
//
rSetting::MoonGlobalInfo::MoonGlobalInfo(std::ifstream& ifs, const bool bin)
{
	if (bin)
		read(ifs);
	else
		readStringFile(ifs);
}

void rSetting::MoonGlobalInfo::read(std::ifstream& ifs)
{
	ifs.read(reinterpret_cast<char*>(&hardcodedKey), sizeof(unsigned char));
}

void rSetting::MoonGlobalInfo::write(std::ofstream& ofs) const
{
	ofs.write(reinterpret_cast<const char*>(&hardcodedKey), sizeof(unsigned char));
}

void rSetting::MoonGlobalInfo::readStringFile(std::ifstream& ifs)
{
	std::string tmp;
	ifs >> tmp;
	hardcodedKey = static_cast<unsigned char>(std::stoul(tmp));
}

void rSetting::MoonGlobalInfo::writeStringFile(std::ofstream& ofs) const
{
	ofs << std::to_string(hardcodedKey) << '\n';
}




//
// AesGlobalInfo
//

rSetting::AesGlobalInfo::AesGlobalInfo(std::ifstream& ifs, const bool bin)
{
	if (bin)
		AesGlobalInfo::read(ifs);
	else
		AesGlobalInfo::readStringFile(ifs);
}

void rSetting::AesGlobalInfo::read(std::ifstream& ifs)
{
	ifs.read(reinterpret_cast<char*>(keyArr.data()), static_cast<std::streamsize>(std::size(keyArr)));
}

void rSetting::AesGlobalInfo::write(std::ofstream& ofs) const
{
	ofs.write(reinterpret_cast<const char*>(keyArr.data()), static_cast<std::streamsize>(std::size(keyArr)));
}

void rSetting::AesGlobalInfo::readStringFile(std::ifstream& ifs)
{	
	for (auto& elem : keyArr)
	{
		std::string tmp;
		ifs >> tmp;
		elem = static_cast<unsigned char>(std::stoul(tmp));
	}
}

void rSetting::AesGlobalInfo::writeStringFile(std::ofstream& ofs) const
{
	for (const auto& elem : keyArr)
		ofs << std::to_string(elem);
}



//
// inners
//
void rSetting::DefaultInnerInfo::read(std::ifstream& ifs)
{
	ifs.read(reinterpret_cast<char*>(&bEncrpytion), sizeof(bool));
	ifs.read(reinterpret_cast<char*>(&xorKey), sizeof(unsigned char));
	ifs.read(ver, static_cast<std::streamsize>(std::size(ver)));
}

void rSetting::DefaultInnerInfo::write(std::ofstream& ofs) const
{
	ofs.write(reinterpret_cast<const char*>(&bEncrpytion), sizeof(bool));
	ofs.write(reinterpret_cast<const char*>(&xorKey), sizeof(unsigned char));
	ofs.write(ver, static_cast<std::streamsize>(std::size(ver)));
}

void rSetting::DefaultInnerInfo::readStringFile(std::ifstream& ifs)
{
	std::string tmp;
	ifs >> tmp;

	bEncrpytion = static_cast<bool>(std::stoi(tmp));
	ifs >> tmp;

	xorKey = static_cast<unsigned char>(std::stoul(tmp));
	ifs >> ver;
}

void rSetting::DefaultInnerInfo::writeStringFile(std::ofstream& ofs) const
{
	ofs << std::to_string(bEncrpytion) << ' ' << std::to_string(xorKey) << ' ' << ver << '\n';
}



//
// InsigniaInnerInfo
//
void rSetting::InsigniaInnerInfo::read(std::ifstream& ifs)
{
	ifs.read(reinterpret_cast<char*>(&fileType), sizeof(unsigned char));
}

void rSetting::InsigniaInnerInfo::write(std::ofstream& ofs) const
{
	ofs.write(reinterpret_cast<const char*>(&fileType), sizeof(unsigned char));
}

void rSetting::InsigniaInnerInfo::readStringFile(std::ifstream& ifs)
{
	std::string tmp;
	ifs >> tmp;
	fileType = static_cast<unsigned char>(std::stoul(tmp));
}

void rSetting::InsigniaInnerInfo::writeStringFile(std::ofstream& ofs) const
{
	ofs << std::to_string(fileType) << '\n';
}



//
// AesInnerInfo
//
void rSetting::AesInnerInfo::read(std::ifstream& ifs)
{
	ifs.read(reinterpret_cast<char*>(&bEncrpyt), sizeof(bool));
}

void rSetting::AesInnerInfo::write(std::ofstream& ofs) const
{
	ofs.write(reinterpret_cast<const char*>(&bEncrpyt), sizeof(bool));
}

void rSetting::AesInnerInfo::readStringFile(std::ifstream& ifs)
{
	std::string tmp;
	ifs >> tmp;
	bEncrpyt = static_cast<bool>(std::stoul(tmp));
}

void rSetting::AesInnerInfo::writeStringFile(std::ofstream& ofs) const
{
	ofs << std::to_string(bEncrpyt) << '\n';
}



//
// EquinoxInnerInfo
//

void rSetting::EquinoxInnerInfo::read(std::ifstream& ifs)
{
	ifs.read(reinterpret_cast<char*>(&hdrLenKey), sizeof(unsigned char));
	ifs.read(reinterpret_cast<char*>(&innerKey), sizeof(unsigned char));
	ifs.read(reinterpret_cast<char*>(&version), sizeof(unsigned short));
	ifs.read(reinterpret_cast<char*>(&bEncrpyt), sizeof(bool));
	ifs.read(reinterpret_cast<char*>(&fileKey), sizeof(unsigned char));
}

void rSetting::EquinoxInnerInfo::write(std::ofstream& ofs) const
{
	ofs.write(reinterpret_cast<const char*>(&hdrLenKey), sizeof(unsigned char));
	ofs.write(reinterpret_cast<const char*>(&innerKey), sizeof(unsigned char));
	ofs.write(reinterpret_cast<const char*>(&version), sizeof(unsigned short));
	ofs.write(reinterpret_cast<const char*>(&bEncrpyt), sizeof(bool));
	ofs.write(reinterpret_cast<const char*>(&fileKey), sizeof(unsigned char));
}

void rSetting::EquinoxInnerInfo::readStringFile(std::ifstream& ifs)
{
	std::string tmp;
	ifs >> tmp;

	hdrLenKey = static_cast<unsigned char>(std::stoul(tmp));
	ifs >> tmp;
	innerKey = static_cast<unsigned char>(std::stoul(tmp));
	ifs >> tmp;
	version = static_cast<unsigned short>(std::stoul(tmp));
	ifs >> tmp;
	bEncrpyt= static_cast<bool>(std::stoul(tmp));
	ifs >> tmp;
	fileKey = static_cast<unsigned char>(std::stoul(tmp));
}

void rSetting::EquinoxInnerInfo::writeStringFile(std::ofstream& ofs) const
{
	ofs << std::to_string(hdrLenKey) << ' ' << std::to_string(innerKey) << ' ' << std::to_string(version) << ' ' << std::to_string(bEncrpyt)
		<< ' ' << std::to_string(fileKey);
}



//
// MoonInnerInfo
//

void rSetting::MoonInnerInfo::read(std::ifstream& ifs)
{
	ifs.read(reinterpret_cast<char*>(&key), sizeof(unsigned char));
	ifs.read(reinterpret_cast<char*>(&bEncrpyt), sizeof(bool));
	ifs.read(ver, static_cast<std::streamsize>(std::size(ver)));
}

void rSetting::MoonInnerInfo::write(std::ofstream& ofs) const
{
	ofs.write(reinterpret_cast<const char*>(&key), sizeof(unsigned char));
	ofs.write(reinterpret_cast<const char*>(&bEncrpyt), sizeof(bool));
	ofs.write(ver, static_cast<std::streamsize>(std::size(ver)));
}

void rSetting::MoonInnerInfo::readStringFile(std::ifstream& ifs)
{
	std::string tmp;
	ifs >> tmp;
	key = static_cast<unsigned char>(std::stoul(tmp));
	ifs >> tmp;
	bEncrpyt = static_cast<bool>(std::stoul(tmp));
	ifs >> ver;
}

void rSetting::MoonInnerInfo::writeStringFile(std::ofstream& ofs) const
{
	ofs << std::to_string(key) << ' ' << std::to_string(bEncrpyt) << ' ' << ver << '\n';
}




//
// Main Info
//
void rSetting::MainInfo::read(std::ifstream& ifs)
{
	ifs.read(reinterpret_cast<char*>(&type), sizeof(file::Type));
	ifs.read(reinterpret_cast<char*>(&id), sizeof(int));
	ifs.read(serverName, static_cast<std::streamsize>(std::size(serverName)));
	ifs.read(extension, static_cast<std::streamsize>(std::size(extension)));
}


void rSetting::MainInfo::write(std::ofstream& ofs) const
{
	ofs.write(reinterpret_cast<const char*>(&type), sizeof(file::Type));
	ofs.write(reinterpret_cast<const char*>(&id), sizeof(int));
	ofs.write(serverName, static_cast<std::streamsize>(std::size(serverName)));
	ofs.write(extension, static_cast<std::streamsize>(std::size(extension)));
}

void rSetting::MainInfo::readStringFile(std::ifstream& ifs)
{
	std::string tmp;
	ifs >> tmp;
	type = static_cast<file::Type>(std::stoi(tmp));

	ifs >> tmp;
	id = std::stoi(tmp);
	ifs >> serverName;
	ifs >> extension;
}

void rSetting::MainInfo::writeStringFile(std::ofstream& ofs) const
{
	ofs << std::to_string(static_cast<int>(type)) << '\t' << std::to_string(id) << '\n' << serverName << '\n' << extension;
}


