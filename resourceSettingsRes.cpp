#include "resourceSettingsRes.h"
#include "resourceSettings.h"


//
//	File selection
//
void rSetting::FileSelectionList::read(std::ifstream& ifs)
{
	for (auto& elem : _Elems)
	{
		unsigned long amountofEntries = 0;
		ifs.read(reinterpret_cast<char*>(&amountofEntries), sizeof(unsigned long));

		elem.reserve(amountofEntries);
		for (unsigned long j = 0; j < amountofEntries; ++j)
		{
			char tmp[128]{};
			ifs.read(tmp, std::size(tmp));
			elem.emplace_back(tmp);
		}
	}
}

void rSetting::FileSelectionList::write(std::ofstream& ofs) const
{
	for (auto& elem : _Elems)
	{
		unsigned long amountOfEntries = static_cast<unsigned long>(elem.size());
		ofs.write(reinterpret_cast<const char*>(&amountOfEntries), sizeof(unsigned long));
		for (const auto& str : elem)
			ofs.write(&str[0], 128);

	}
}


void rSetting::FileSelectionList::readStringFile(std::ifstream& ifs)
{
	std::string tmp;
	for (auto& elem : _Elems)
	{
		ifs >> tmp;
		const unsigned long amountofEntries = std::stoul(tmp);
		elem.reserve(amountofEntries);

		for (unsigned long j = 0; j < amountofEntries; ++j)
		{
			char tmp2[128]{};
			ifs >> tmp2;
			ifs.read(tmp2, std::size(tmp2));
			elem.emplace_back(tmp2);
		}
	}
}

void rSetting::FileSelectionList::writeStringFile(std::ofstream& ofs) const
{
	for (auto& elem : _Elems)
	{
		const unsigned long amountOfEntries = static_cast<unsigned long>(elem.size());
		ofs << std::to_string(amountOfEntries) << '\n';
		for (const auto& str : elem)
			ofs << str << '\n';
	}
}



//
// ResourceSetting
//
void rSetting::ResourceSetting::read(std::ifstream& ifs)
{
	ptr->read(ifs);
	fs.read(ifs);
}

void rSetting::ResourceSetting::write(std::ofstream& ofs) const
{
	ptr->write(ofs);
	fs.write(ofs);
}

void rSetting::ResourceSetting::readStringFile(std::ifstream& ifs)
{
	ptr->readStringFile(ifs);
	fs.readStringFile(ifs);
}

void rSetting::ResourceSetting::writeStringFile(std::ofstream& ofs) const
{
	ptr->writeStringFile(ofs);
	fs.writeStringFile(ofs);
}


//
// ResPack
//
rSetting::ResPack::ResPack(std::ifstream& ifs, const bool bin)
{
	if (bin)
		ResPack::read(ifs);
	else
		ResPack::readStringFile(ifs);
}

void rSetting::ResPack::read(std::ifstream& ifs)
{
	ifs.read(name, std::size(name));
	rs.read(ifs);	
}

void rSetting::ResPack::write(std::ofstream& ofs) const
{
	ofs.write(name, std::size(name));
	rs.write(ofs);
}

void rSetting::ResPack::readStringFile(std::ifstream& ifs)
{
	ifs >> name;
	rs.readStringFile(ifs);
}

void rSetting::ResPack::writeStringFile(std::ofstream& ofs) const
{
	ofs << name;
	rs.writeStringFile(ofs);
}



//
// ResPackList
//
void rSetting::ResPackList::read(std::ifstream& ifs)
{
	unsigned long count = 0;
	ifs.read(reinterpret_cast<char*>(&count), sizeof(unsigned long));

	reserve(count);
	for (unsigned long i = 0; i < count; ++i)
		emplace_back(ResPack(ifs));
}

void rSetting::ResPackList::write(std::ofstream& ofs) const
{
	const unsigned long count = size();
	ofs.write(reinterpret_cast<const char*>(&count), sizeof(unsigned long));
	for (const auto& ref : *this)
		ref.write(ofs);
}

void rSetting::ResPackList::readStringFile(std::ifstream& ifs)
{
	std::string tmp;
	ifs >> tmp;
	const unsigned long count = std::stoul(tmp);

	reserve(count);
	for (unsigned long i = 0; i < count; ++i)
		emplace_back(ResPack(ifs, false));
}

void rSetting::ResPackList::writeStringFile(std::ofstream& ofs) const
{
	const unsigned long count = static_cast<unsigned long>(size());
	ofs << std::to_string(count) << '\n';

	for (const auto& ref : *this)
		ref.writeStringFile(ofs);
}




//
// ResGolalInf
//
void rSetting::ResGlobalInfo::read(std::ifstream& ifs)
{

}

void rSetting::ResGlobalInfo::write(std::ofstream& ofs) const
{
	ofs.write(reinterpret_cast<const char*>(&pck), sizeof(PackLimit));
	if (pck & size)
		ofs.write(reinterpret_cast<const char*>(&maxSize), sizeof(long long));

	ofs.write(reinterpret_cast<const char*>(&bDoFile), sizeof(bool));
}

void rSetting::ResGlobalInfo::readStringFile(std::ifstream& ifs)
{
	
}

void rSetting::ResGlobalInfo::writeStringFile(std::ofstream& ofs) const
{
	
}





//
// Main Info
//
void rSetting::MainInfo::read(std::ifstream& ifs)
{
	ifs.read(reinterpret_cast<char*>(&type), sizeof(file::Type));
	ifs.read(reinterpret_cast<char*>(&id), sizeof(int));
	ifs.read(serverName, std::size(serverName));
	ifs.read(extension, std::size(extension));
}


void rSetting::MainInfo::write(std::ofstream& ofs) const
{
	ofs.write(reinterpret_cast<const char*>(&type), sizeof(file::Type));
	ofs.write(reinterpret_cast<const char*>(&id), sizeof(int));
	ofs.write(serverName, std::size(serverName));
	ofs.write(extension, std::size(extension));
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



// 
// Single Setting
//

rSetting::Setting::Setting()
{

}

rSetting::Setting::~Setting()
{
	
}

void rSetting::Setting::read(std::ifstream& ifs)
{
	mInfo.read(ifs);

	delete gInfo;
	gInfo = createGlobalInfo(mInfo.type);
	if (gInfo)
		gInfo->read(ifs);
	
	rpl.read(ifs);
}



void rSetting::Setting::write(std::ofstream& ofs) const
{
	mInfo.write(ofs);

	if (gInfo)
		gInfo->write(ofs);

	rpl.write(ofs);
}




void rSetting::Setting::readStringFile(std::ifstream& ifs)
{
	mInfo.readStringFile(ifs);
	delete gInfo;
	gInfo = createGlobalInfo(mInfo.type);
	if (gInfo)
		gInfo->readStringFile(ifs);

	rpl.readStringFile(ifs);
}



void rSetting::Setting::writeStringFile(std::ofstream& ofs) const
{
	mInfo.writeStringFile(ofs);

	if (gInfo)
		gInfo->writeStringFile(ofs);

	rpl.writeStringFile(ofs);
}















