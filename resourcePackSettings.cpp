#include "resourcePackSettings.h"
#include "resourceSettingsRes.h"

//
//	File selection
//
res::settings::FileSelectionList::FileSelectionList(std::ifstream& ifs, const bool bin)
{
	if (bin)
		read(ifs);
	else
		readStringFile(ifs);
}

void res::settings::FileSelectionList::read(std::ifstream& ifs)
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

void res::settings::FileSelectionList::write(std::ofstream& ofs) const
{
	for (auto& elem : _Elems)
	{
		unsigned long amountOfEntries = static_cast<unsigned long>(elem.size());
		ofs.write(reinterpret_cast<const char*>(&amountOfEntries), sizeof(unsigned long));
		for (const auto& str : elem)
			ofs.write(&str[0], 128);

	}
}


void res::settings::FileSelectionList::readStringFile(std::ifstream& ifs)
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

void res::settings::FileSelectionList::writeStringFile(std::ofstream& ofs) const
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
res::settings::ResourceSetting::ResourceSetting(std::ifstream& ifs, bool bin)
{
	if (bin)
		read(ifs);
	else
		readStringFile(ifs);
}

void res::settings::ResourceSetting::read(std::ifstream& ifs)
{
	ptr->read(ifs);
	fs.read(ifs);
}

void res::settings::ResourceSetting::write(std::ofstream& ofs) const
{
	ptr->write(ofs);
	fs.write(ofs);
}

void res::settings::ResourceSetting::readStringFile(std::ifstream& ifs)
{
	ptr->readStringFile(ifs);
	fs.readStringFile(ifs);
}

void res::settings::ResourceSetting::writeStringFile(std::ofstream& ofs) const
{
	ptr->writeStringFile(ofs);
	fs.writeStringFile(ofs);
}


//
// ResPack
//
res::settings::ResPack::ResPack(std::ifstream& ifs, const bool bin)
{
	if (bin)
		ResPack::read(ifs);
	else
		ResPack::readStringFile(ifs);
}

void res::settings::ResPack::read(std::ifstream& ifs)
{
	ifs.read(name, std::size(name));
	rs.read(ifs);
}

void res::settings::ResPack::write(std::ofstream& ofs) const
{
	ofs.write(name, std::size(name));
	rs.write(ofs);
}

void res::settings::ResPack::readStringFile(std::ifstream& ifs)
{
	ifs >> name;
	rs.readStringFile(ifs);
}

void res::settings::ResPack::writeStringFile(std::ofstream& ofs) const
{
	ofs << name;
	rs.writeStringFile(ofs);
}



//
// ResPackList
//
res::settings::ResPackList::ResPackList(std::ifstream& ifs, const bool bin)
{
	if (bin)
		read(ifs);
	else
		readStringFile(ifs);
}

void res::settings::ResPackList::read(std::ifstream& ifs)
{
	unsigned long count = 0;
	ifs.read(reinterpret_cast<char*>(&count), sizeof(unsigned long));

	reserve(count);
	for (unsigned long i = 0; i < count; ++i)
		emplace_back(ResPack(ifs));
}

void res::settings::ResPackList::write(std::ofstream& ofs) const
{
	const unsigned long count = static_cast<unsigned long>(size());
	ofs.write(reinterpret_cast<const char*>(&count), sizeof(unsigned long));
	for (const auto& ref : *this)
		ref.write(ofs);
}

void res::settings::ResPackList::readStringFile(std::ifstream& ifs)
{
	std::string tmp;
	ifs >> tmp;
	const unsigned long count = std::stoul(tmp);

	reserve(count);
	for (unsigned long i = 0; i < count; ++i)
		emplace_back(ResPack(ifs, false));
}

void res::settings::ResPackList::writeStringFile(std::ofstream& ofs) const
{
	const unsigned long count = static_cast<unsigned long>(size());
	ofs << std::to_string(count) << '\n';

	for (const auto& ref : *this)
		ref.writeStringFile(ofs);
}

