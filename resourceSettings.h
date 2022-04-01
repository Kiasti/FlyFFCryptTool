#pragma once
#include <string>
#include <vector>
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
	
	/**	@brief	Main Setting that holds the header settings and the main info regarding 
				the setting.															*/
	class Setting final : rwIO
	{
		MainInfo mInfo;
		ResGlobalInfo* gInfo{};
		ResPackList resPackList;

		public:
			Setting();
			explicit Setting(std::ifstream& ifs, rwIOType type = rwIOType::bin);
			~Setting() override;
		
			Setting(Setting const&) = default;
			Setting& operator =(Setting const&) = default;
			Setting(Setting&&) = default;
			Setting& operator=(Setting&&) = default;

			void write(std::ofstream& ofs, rwIOType type = rwIOType::bin) const override;
			void read(std::ifstream& ifs, rwIOType type = rwIOType::bin) override;

			MainInfo& getMainInfo() { return mInfo; }
			void setMainInfo(const MainInfo& mf) { mInfo = mf; }
			[[nodiscard]] file::Type getType() const { return mInfo.type; }
			[[nodiscard]] std::string getSettingName() const { return mInfo.serverName; }
			[[nodiscard]] std::string getExtension() const { return mInfo.extension; }
			[[nodiscard]] int getId() const { return mInfo.id; }

			[[nodiscard]] ResGlobalInfo* getGlobalInfo() const { return gInfo; }
			void setGInfo(ResGlobalInfo* gf) { gInfo = gf; }


	};
	
	/**	@brief	List of Settings for different servers.
		@see	Setting																				*/
	using SettingsList = std::vector<Setting>;


	/** @brief	Handles information related to user settings in regards to extracting, packing,
	*			and general application use.														*/
	class Settings final : public ApplicationSettings, public rwIO
	{
		SettingsList settings;

		protected:
			void read(std::ifstream& ifs, rwIOType type = rwIOType::bin) override;
			void write(std::ofstream& ofs, rwIOType type = rwIOType::bin) const override;

			// Todo: Implement a default dir.
			[[nodiscard]] static bool setClientDirectory(const std::string&) { return true; }
		public:
			Settings() = default;
			~Settings() override = default;

			Settings(Settings const&) = default;
			Settings& operator =(Settings const&) = default;
			Settings(Settings&&) = default;
			Settings& operator=(Settings&&) = default;

			bool loadSettings(std::string_view settingsFile = "settings.bin");
			bool loadSettingsStr(std::string_view settingsFile = "settings.txt");
			static bool loadSettingsParser(std::string_view settingsFile = "settings.txt");

			[[nodiscard]] bool saveSettings(std::string_view settingsFile = "settings.bin") const;
			[[nodiscard]] bool saveSettingsStr(std::string_view settingsFile = "settings.txt") const;

			void addSetting(Setting& ptr);
	};	
}

namespace rSetting = res::settings;


