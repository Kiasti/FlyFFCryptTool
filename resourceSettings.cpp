#include "resourceSettings.h"
#include "fileSys.h"
#include "Util.h"

void rSetting::Settings::write(std::ofstream& ofs) const
{
	unsigned char serverAmount = settings.size() >= UCHAR_MAX ? UCHAR_MAX : static_cast<unsigned char>(settings.size());
	ofs.write(reinterpret_cast<char*>(&serverAmount), sizeof(unsigned char));

	for (const auto& setting : settings)
		setting.write(ofs);
}



void rSetting::Settings::read(std::ifstream& ifs)
{

}



void rSetting::Settings::readStringFile(std::ifstream& ifs)
{
	unsigned char servers = 0;
	ifs >> servers;

}

void rSetting::Settings::writeStringFile(std::ofstream& ofs) const
{

}



bool rSetting::Settings::loadSettings(const std::string_view settingsFile)
{
	if (std::ifstream readingFile(settingsFile.data(), std::ios::in | std::ios::binary); readingFile.good())
	{
		read(readingFile);
		readingFile.close();
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



bool rSetting::Settings::loadSettingsStr(const std::string_view settingsFile)
{
	if (std::ifstream ifs(settingsFile.data(), std::ios::in | std::ios::binary); ifs.good())
	{
		readStringFile(ifs);
		ifs.close();
		return true;
	}
	return false;
}


bool rSetting::Settings::saveSettingsStr(const std::string_view settingsFile) const
{
	if (std::ofstream ofs(settingsFile.data(), std::ios::out | std::ios::binary); ofs.good())
	{
		writeStringFile(ofs);
		ofs.close();
		return true;
	}
	return false;
}


