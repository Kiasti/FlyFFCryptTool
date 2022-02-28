#pragma once
#include <string>
#include <vector>
#include <map>
#include <filesystem>
#include "resource.h"
#include "resourceSettingsRes.h"


namespace res::settings
{
	/**	@brief	Currently none, but there may be a reason to implement default settings
				unrelated to the extract and pack functionality.						*/


	class ApplicationSettings
	{
		[[maybe_unused]] bool isCool = false;
	};


	class FreeSettings
	{
		// list of settings that are free
	};

	/** @brief	Handles information related to user settings in regards to extracting, packing,
	*			and general application use.														*/
	class Settings final : public ApplicationSettings, public rwIO
	{

		SettingsList settings;

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

			bool loadSettings(std::string_view settingsFile = "settings.bin");
			bool loadSettingsStr(std::string_view settingsFile = "settings.txt");

			bool saveSettings(std::string_view settingsFile = "settings.bin") const;
			bool saveSettingsStr(std::string_view settingsFile = "settings.txt") const;

			void addSetting(Setting& ptr);
	};	
}

namespace rSetting = res::settings;


