#pragma once
#include <string>
#include <vector>
#include <map>
#include <filesystem>
#include "resource.h"
#include "resourcePackSettings.h"
#include "resourceSettingsRes.h"


namespace res::settings
{
	/**	@brief	Currently none, but there may be a reason to implement default settings
				unrelated to the extract and pack functionality.						*/
	class ApplicationSettings
	{
		[[maybe_unused]] bool isCool = false;
	};
	
	class Setting
	{
		MainInfo mInfo;
		ResGlobalInfo* gInfo{};

		public:
			Setting();
			explicit Setting(std::ifstream& ifs, unsigned char type = 1);
			~Setting();
			Setting(Setting const&) = default;
			Setting& operator =(Setting const&) = default;
			Setting(Setting&&) = default;
			Setting& operator=(Setting&&) = default;

			void write(std::ofstream& ofs) const;
			void read(std::ifstream& ifs);
			void writeStringFile(std::ofstream& ofs) const;
			void readStringFile(std::ifstream& ifs);

			MainInfo& getMainInfo() { return mInfo; }
			void setMainInfo(const MainInfo& mf) { mInfo = mf; }
			[[nodiscard]] file::Type getType() const { return mInfo.type; }
			[[nodiscard]] std::string getSettingName() const { return mInfo.serverName; }
			[[nodiscard]] std::string getExtension() const { return mInfo.extension; }
			[[nodiscard]] int getId() const { return mInfo.id; }

			[[nodiscard]] ResGlobalInfo* getGlobalInfo() const { return gInfo; }
			void setGInfo(ResGlobalInfo* gf) { gInfo = gf; }


	};
	using SettingsList = std::vector<Setting>;

	/** @brief	Handles information related to user settings in regards to extracting, packing,
	*			and general application use.														*/
	class Settings final : public ApplicationSettings, public rwIO
	{
		SettingsList settings;
		ResPackList* resPackList {nullptr};

		protected:
			void read(std::ifstream& ifs) override;
			void write(std::ofstream& ofs) const override;
			void readStringFile(std::ifstream&) override;
			void writeStringFile(std::ofstream&) const override;

			[[nodiscard]] bool setClientDirectory(const std::string&) const { return true; }
		public:
			Settings() = default;
			~Settings() = default;

			Settings(Settings const&) = default;
			Settings& operator =(Settings const&) = default;
			Settings(Settings&&) = default;
			Settings& operator=(Settings&&) = default;

			bool loadPackFile(std::string_view packFile = "resource.bin");
			bool loadPackFileStr(std::string_view packFile = "resource.txt");
			bool loadPackFileParser(std::string_view packFile = "resource.txt");

			bool loadSettings(std::string_view settingsFile = "settings.bin");
			bool loadSettingsStr(std::string_view settingsFile = "settings.txt");
			static bool loadSettingsParser(std::string_view settingsFile = "settings.txt");

			[[nodiscard]] bool saveSettings(std::string_view settingsFile = "settings.bin") const;
			[[nodiscard]] bool saveSettingsStr(std::string_view settingsFile = "settings.txt") const;

			void addSetting(Setting& ptr);
	};	
}

namespace rSetting = res::settings;


