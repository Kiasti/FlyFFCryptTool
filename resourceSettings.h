#pragma once
#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <map>
#include <filesystem>
#include <variant>
#include "resource.h"


namespace res::s
{
	struct customSetting
	{
		
	};
	struct subSetting
	{
		bool oneByte = false;
		bool compression = false;
		bool largeSupport = false;
	};

	struct aesSetting
	{
		
	};
	struct floristSetting
	{
		
	};

	//using settingVariant = std::variant<
	struct serverSetting
	{
		file::Type type = file::Type::def;
		std::string extension{ ".res" };
		unsigned char encrpytionXor = 0x54;
		std::array<unsigned char, 25> enc_key{ 0 };
		subSetting sub;

		void write(const std::string& serverName, std::ofstream& ofs) const;
		std::string read(std::ifstream& ifs);

		void writeStringFile(const std::string& serverName, std::ofstream& ofs) const;
		std::string readStringFile(std::ifstream& ifs);
	};

	class setting
	{
		std::vector<std::string> ignoreList;
	
		protected:
			mutable std::filesystem::path clientDirectory;

			virtual void readBin() = 0;
			virtual void saveBin() const = 0;
			virtual void readStringFileBin() = 0;
			virtual void saveStringFileBin() const = 0;
			virtual void convertStringFileToBin()
			{
				readStringFileBin();
				saveBin();
			}
			[[nodiscard]] bool setClientDirectory(const std::string& dir) const;
		public:
			setting() = default;
			virtual ~setting() = default;
			void loadIgnoreList();
		
	};
	
	namespace ext
	{
		class ext final : public setting
		{
			std::map<std::string, serverSetting> list // hardcoded entries for now to dump them in a bin file.
			{
				std::pair<std::string, serverSetting>("default",
					{file::Type::def, ".res", 0x57, {}, {}}),
				std::pair<std::string, serverSetting>("spirit",
					{file::Type::aes1, ".spir", 0, {12, 51, 22, 32, 123, 11, 85, 124, 23, 13, 21, 26, 105, 64, 76, 121, 55, 49, 76, 99, 55, 67, 104, 110}, {}}),
				std::pair<std::string, serverSetting>("ember",
					{file::Type::aes1, ".pak", 0, {133, 207, 168, 16, 239, 251, 221, 129, 12, 29, 11, 6, 18, 25, 26, 24, 170, 103, 136, 141, 248, 232, 220, 153}, {}}),
				std::pair<std::string, serverSetting>("gow",
					{file::Type::aes1, ".gow", 0, {124, 108, 129, 11, 220, 152, 221, 110, 12, 10, 10, 7, 16, 21, 23, 24, 176, 101, 117, 110, 215, 231, 127, 200}, {}})
			};
			std::string folderOutput;
		
			protected:
				void readBin() override;
				void saveBin() const override;
				void readStringFileBin() override;
				void saveStringFileBin() const override;
		};

	}
	

	namespace pack
	{
		class pack final : public setting
		{
			enum packLimit
			{
				size, letter, bulk, none
			};
			
			std::vector<std::string> ignoreList;
			std::pair<std::string, serverSetting> ss;
			std::map<std::string, std::vector<std::filesystem::path>> resourceToPath;
		
			protected:
				void readBin() override;
				void saveBin() const override;
				void readStringFileBin() override;
				void saveStringFileBin() const override;
				void loadListToPack();
		};
	}
}

namespace extS = res::s::ext;
namespace packS = res::s::pack;
namespace normalS = res::s;

