#include "resourcePackSettings.h"
#include "resourceSettingsRes.h"

//
//	File selection
//
res::settings::FileSelectionList::FileSelectionList(std::ifstream& ifs, const rwIOType type)
{
	read(ifs, type);
}

void res::settings::FileSelectionList::read(std::ifstream& ifs, const rwIOType type)
{
	switch (type)
	{
		case rwIOType::bin: 
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
			break;
		case rwIOType::string: 
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
			break;
		}
		case rwIOType::ffParser:
			break;
	}
}

void res::settings::FileSelectionList::write(std::ofstream& ofs, const rwIOType type) const
{
	switch (type)
	{
		case rwIOType::bin: 
			for (auto& elem : _Elems)
			{
				unsigned long amountOfEntries = static_cast<unsigned long>(elem.size());
				ofs.write(reinterpret_cast<const char*>(&amountOfEntries), sizeof(unsigned long));
				for (const auto& str : elem)
					ofs.write(&str[0], 128);

			}
			break;
		case rwIOType::string: 
			for (auto& elem : _Elems)
			{
				const unsigned long amountOfEntries = static_cast<unsigned long>(elem.size());
				ofs << std::to_string(amountOfEntries) << '\n';
				for (const auto& str : elem)
					ofs << str << '\n';
			}
			break;
		case rwIOType::ffParser: 
			break;
	}
}






//
// ResourceSetting
//

//
// ResPack
//
res::settings::ResPack::ResPack(std::ifstream& ifs, const rwIOType type)
{
	ResPack::read(ifs, type);
}

void res::settings::ResPack::read(std::ifstream& ifs, const rwIOType type)
{
	ifs.read(name, static_cast<std::streamsize>(std::size(name)));
	ptr->read(ifs, type);
	fs.read(ifs, type);

}

void res::settings::ResPack::write(std::ofstream& ofs, const rwIOType type) const
{
	ofs.write(name, static_cast<std::streamsize>(std::size(name)));
	ptr->write(ofs, type);
	fs.write(ofs, type);

}



//
// ResPackList
//
res::settings::ResPackList::ResPackList(std::ifstream& ifs, const rwIOType type)
{
	read(ifs, type);
}


void res::settings::ResPackList::read(std::ifstream& ifs, const rwIOType type)
{
	unsigned long count = 0;
	switch (type)
	{
		case rwIOType::bin: 
		{
			ifs.read(reinterpret_cast<char*>(&count), sizeof(unsigned long));
			break;
		}
		case rwIOType::string:
		{
			std::string tmp;
			ifs >> tmp;
			count = std::stoul(tmp);
			break;
		}
		case rwIOType::ffParser: 
			break;	
	}
	reserve(count);
	for (unsigned long i = 0; i < count; ++i)
		emplace_back(ResPack(ifs, type));

}

void res::settings::ResPackList::write(std::ofstream& ofs, const rwIOType type) const
{
	switch (type)
	{
		case rwIOType::bin:
		{
			const unsigned long count = static_cast<unsigned long>(size());
			ofs.write(reinterpret_cast<const char*>(&count), sizeof(unsigned long));
			break;
		}
		case rwIOType::string:
		{
			const unsigned long count = static_cast<unsigned long>(size());
			ofs << std::to_string(count) << '\n';
			break;
		}
		case rwIOType::ffParser:
			break;
	}
	for (const auto& ref : *this)
		ref.write(ofs, type);
}

