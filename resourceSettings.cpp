/**
*		@file	resourceSettings.cpp
*		@todo	Fix string loading and writing formats. Try out FlyFF's parser vs using ifstream / ofstream with new lines.
*/


#include "resourceSettings.h"
#include "fileSys.h"
#include "Util.h"


// commented out functions that was in main to try and make base settings bin. Later will swap to purely .txt and .bin settings.
/*
void createBaseSettings()
{
	rSetting::Settings sm;

	rSetting::Setting setting1;
	setting1.getMainInfo().id = 1;
	setting1.getMainInfo().type = rfile::Type::Default;
	strcpy(setting1.getMainInfo().extension, ".res\0");
	strcpy(setting1.getMainInfo().serverName, "Official\0");
	setting1.setGInfo(rSetting::createGlobalInfo(rfile::Type::Default));
	sm.addSetting(setting1);

	rSetting::Setting setting2;
	setting2.getMainInfo().id = 2;
	setting2.getMainInfo().type = rfile::Type::AesGow;
	strcpy(setting2.getMainInfo().extension, ".enc\0");
	strcpy(setting2.getMainInfo().serverName, "GoW\0");
	setting2.setGInfo(rSetting::createGlobalInfo(rfile::Type::AesGow));
	dynamic_cast<rSetting::AesGlobalInfo*>(setting2.getGlobalInfo())->keyArr = { 124, 108, 129, 11, 220, 152, 221, 110, 12, 10, 10, 7, 16, 21, 23, 24, 176, 101, 117, 110, 215, 231, 127, 20 };
	sm.addSetting(setting2);

	rSetting::Setting setting3;
	setting3.getMainInfo().id = 3;
	strcpy(setting3.getMainInfo().extension, ".fast\0");
	setting3.getMainInfo().type = rfile::Type::Forsaken;
	strcpy(setting3.getMainInfo().serverName, "Forskan\0");
	setting3.setGInfo(rSetting::createGlobalInfo(rfile::Type::Forsaken));
	dynamic_cast<rSetting::ForsakenGlobalInfo*>(setting3.getGlobalInfo())->saltPos = cryptEngine::saltPos::beg;
	strcpy(dynamic_cast<rSetting::ForsakenGlobalInfo*>(setting3.getGlobalInfo())->sha_hash, "m3ntu5d3rHur3ns00hn\0");
	strcpy(dynamic_cast<rSetting::ForsakenGlobalInfo*>(setting3.getGlobalInfo())->secondExt, ".fly\0");
	sm.addSetting(setting3);

	rSetting::Setting setting4;
	setting4.getMainInfo().id = 4;
	setting4.getMainInfo().type = rfile::Type::Moon;
	strcpy(setting4.getMainInfo().extension, ".lune\0");
	strcpy(setting4.getMainInfo().serverName, "Moon\0");
	setting4.setGInfo(rSetting::createGlobalInfo(rfile::Type::Moon));
	dynamic_cast<rSetting::MoonGlobalInfo*>(setting4.getGlobalInfo())->hardcodedKey = 0x7B;
	sm.addSetting(setting4);

	rSetting::Setting setting5;
	setting5.getMainInfo().id = 5;
	setting5.getMainInfo().type = rfile::Type::Equinox;
	strcpy(setting5.getMainInfo().extension, ".res\0");
	strcpy(setting5.getMainInfo().serverName, "Equinox\0");
	setting5.setGInfo(rSetting::createGlobalInfo(rfile::Type::Equinox));
	strcpy(dynamic_cast<rSetting::EquinoxGlobalInfo*>(setting5.getGlobalInfo())->compareStr, "\"RoC Resource File\"");
	memcpy(&dynamic_cast<rSetting::EquinoxGlobalInfo*>(setting5.getGlobalInfo())->keyArr[0], rfile::other::eTable, std::size(rfile::other::eTable));
	sm.addSetting(setting5);

	bool b = sm.saveSettings();
	b = sm.saveSettingsStr();
}


void checkSettingsStr()
{
	rSetting::Settings sm;
	sm.loadSettingsStr();

	std::cout << "check" << std::endl;
}


void checkSettingsBin()
{
	rSetting::Settings sm;
	sm.loadSettings();

	std::cout << "check" << std::endl;

}
*/

// 
// Single Setting
//

rSetting::Setting::Setting() = default;
rSetting::Setting::Setting(std::ifstream& ifs, const rwIOType type)
{
	read(ifs, type);
}

rSetting::Setting::~Setting() = default;
void rSetting::Setting::read(std::ifstream& ifs, const rwIOType type)
{
	mInfo.read(ifs, type);

	delete gInfo;
	gInfo = createGlobalInfo(mInfo.type, ifs, type);

	resPackList.read(ifs, type);
}

void rSetting::Setting::write(std::ofstream& ofs, const rwIOType type) const
{
	mInfo.write(ofs, type);

	if (gInfo)
		gInfo->write(ofs, type);
}



//
//	Settings Class
//

void rSetting::Settings::read(std::ifstream& ifs, const rwIOType type)
{
	unsigned char serverAmount = 0;
	switch (type)
	{
		case rwIOType::bin:
			ifs.read(reinterpret_cast<char*>(&serverAmount), sizeof(unsigned char));
			break;
		case rwIOType::string:
		{
			std::string tmp;
			ifs >> tmp;

			serverAmount = static_cast<unsigned char>(std::stoul(tmp));
			break;
		}
		case rwIOType::ffParser:
			break;
	}

	for (int i = 0; i < serverAmount; ++i)
		settings.emplace_back(Setting(ifs, type));
}

void rSetting::Settings::write(std::ofstream& ofs, const rwIOType type) const
{
	switch (type)
	{
		case rwIOType::bin: 
		{
			unsigned char servers = settings.size() >= UCHAR_MAX ? UCHAR_MAX : static_cast<unsigned char>(settings.size());
			ofs.write(reinterpret_cast<char*>(&servers), sizeof(unsigned char));
			break;
		}
		case rwIOType::string: 
			ofs << std::to_string(settings.size() >= UCHAR_MAX ? UCHAR_MAX : settings.size()) << "\n\t"; 
			break;
		case rwIOType::ffParser: break;
	}
	for (const auto& setting : settings)
		setting.write(ofs, type);
}

bool rSetting::Settings::loadSettings(const std::string_view settingsFile)
{
	if (std::ifstream ifs(settingsFile.data(), std::ios::in | std::ios::binary); ifs.good())
	{
		read(ifs);
		ifs.close();
		return true;
	}
	return false;
}


bool rSetting::Settings::loadSettingsStr(const std::string_view settingsFile)
{
	if (std::ifstream ifs(settingsFile.data(), std::ios::in | std::ios::binary); ifs.good())
	{
		read(ifs, rwIOType::string);
		ifs.close();
		return true;
	}
	return false;
}

bool rSetting::Settings::loadSettingsParser(const std::string_view settingsFile)
{
	if (std::ifstream ifs(settingsFile.data(), std::ios::in | std::ios::binary); ifs.good())
	{
		// Todo: ffParser implementation
		ifs.close();
		return true;
	}
	return false;
}



bool rSetting::Settings::saveSettings(const std::string_view settingsFile) const
{
	if (std::ofstream ofs(settingsFile.data(), std::ios::out | std::ios::binary); ofs.good())
	{
		write(ofs);
		ofs.close();
		return true;
	}
	return false;
}


bool rSetting::Settings::saveSettingsStr(const std::string_view settingsFile) const
{
	if (std::ofstream ofs(settingsFile.data(), std::ios::out | std::ios::binary); ofs.good())
	{
		write(ofs, rwIOType::string);
		ofs.close();
		return true;
	}
	return false;
}

void rSetting::Settings::addSetting(Setting& ptr)
{
	settings.emplace_back(ptr);
}
