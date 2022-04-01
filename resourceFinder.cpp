#include "resourceFinder.h"

#include "CRC64.h"
#include "cryptEngine.h"

bool res::find::FindFile::operator()(const vp_Flyff& df) const
{
	return std::ranges::any_of(df.second.begin(), df.second.end(), [this](const file::flyff::Res& rhs) -> bool {
			return name == rhs.fileName;
		});
}

bool res::find::FindFile::operator()(const vp_Cloud& df) const
{
	return std::ranges::any_of(df.second.begin(), df.second.end(), [this](const file::flyff::Res& rhs) -> bool {
		return name == rhs.fileName;
		});
}

bool res::find::FindFile::operator()(const vp_Insignia& df) const
{
	return std::ranges::any_of(df.second.begin(), df.second.end(), [this](const file::other::ResInsignia& rhs) -> bool {
			return name == rhs.fileName;
		});
}

bool res::find::FindFile::operator()(const vp_AesGow& df) const
{
	return std::ranges::any_of(df.second.begin(), df.second.end(), [this](const file::flyff::Res& rhs) -> bool {
			return name == rhs.fileName;
		});
}

bool res::find::FindFile::operator()(const vp_AesAzure& df) const
{
	auto crc = cryptEngine::wbqt::CRC64::Compute(reinterpret_cast<const unsigned char*>(&name[0]), name.length());
	return std::ranges::any_of(df.second.begin(), df.second.end(), [crc](const file::other::ResAesAzure& rhs) -> bool {
			return crc == rhs.crc;
		});
}

bool res::find::FindFile::operator()(const vp_Moon& df) const
{
	return std::ranges::any_of(df.second.begin(), df.second.end(), [this](const file::other::ResMoon& rhs) -> bool {
			return name == rhs.fileName;
		});

}

bool res::find::FindFile::operator()(const vp_Equinox& df) const
{
	return std::ranges::any_of(df.second.begin(), df.second.end(), [this](const file::other::ResEquinox& rhs) -> bool {
			return name == rhs.fileName;
		});

}

bool res::find::FindFile::operator()(const vp_Forsaken& df) const
{
	std::ranges::transform(name, name.begin(), [](const unsigned char c) { return std::tolower(c); });
	name = cryptEngine::getSha256(name);
	return std::ranges::any_of(df.second.begin(), df.second.end(), [this](const file::other::ResForsaken& rhs) -> bool {
			return name == rhs.fileName;
		});
}

bool res::find::FindFile::operator()(const vp_NewFeiFei& df) const
{
	unsigned int hashId = file::feifei::string_id(name.c_str());
	return std::ranges::any_of(df.second.begin(), df.second.end(), [hashId](const file::feifei::Res& rhs) -> bool {
			return hashId == rhs.uid;
		});
}



bool res::find::findEntropia::operator()(const vp_Forsaken& df) const
{
	return std::ranges::any_of(df.second.begin(), df.second.end(), [this](const file::other::ResForsaken& rhs) -> bool {
		return name == rhs.fileName;
		});

}