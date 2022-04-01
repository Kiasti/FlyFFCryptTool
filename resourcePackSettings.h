#pragma once
#include <vector>
#include "rwIO.h"
#include "resourceSettingsRes.h"

namespace res::settings
{
	enum FileSelection : int 
	{
		FILE,
		FOLDER,
		WILDCARD,
		MAX
	};
	enum PackLimit : int
	{
		none = 0,
		size = 1 << 0,
		letter = 1 << 1,
		bulk = 1 << 2,
		MaxPack
	};

	struct FileSelectionList final : std::array<std::vector<std::string>, MAX>, rwIO
	{
		FileSelectionList() = default;
		explicit FileSelectionList(std::ifstream & ifs, rwIOType type = rwIOType::bin);
		~FileSelectionList() override = default;

		FileSelectionList(FileSelectionList const&) = default;
		FileSelectionList& operator =(FileSelectionList const&) = default;
		FileSelectionList(FileSelectionList&&) = default;
		FileSelectionList& operator=(FileSelectionList&&) = default;

		void read(std::ifstream & ifs, rwIOType type = rwIOType::bin) override;
		void write(std::ofstream & ofs, rwIOType type = rwIOType::bin) const override;
	};

	// Forward declaration.
	struct ResInnerInfo;


	/**	@brief	A single '.res' file for packing and also contains the name and a list of
				FileSeletionStatements. Also contains the settings for the 'Resource'.

		@see	ResInnerInfo, FileSelectionList, rfile									*/
	struct ResPack final : rwIO
	{
		char name[128]{};
		ResInnerInfo* ptr{ nullptr };
		FileSelectionList fs;

		ResPack() = default;
		explicit ResPack(std::ifstream& ifs, rwIOType type = rwIOType::bin);
		~ResPack() override = default;

		ResPack(ResPack const&) = default;
		ResPack& operator =(ResPack const&) = default;
		ResPack(ResPack&&) = default;
		ResPack& operator=(ResPack&&) = default;

		void read(std::ifstream&, rwIOType type = rwIOType::bin) override;
		void write(std::ofstream&, rwIOType type = rwIOType::bin) const override;
	};

	struct ResPackList final : std::vector<ResPack>, rwIO
	{
		PackLimit pck{};
		long long maxSize{};

		ResPackList() = default;
		explicit ResPackList(std::ifstream& ifs, rwIOType type = rwIOType::bin);
		~ResPackList() = default;

		ResPackList(ResPackList const&) = default;
		ResPackList& operator =(ResPackList const&) = default;
		ResPackList(ResPackList&&) = default;
		ResPackList& operator=(ResPackList&&) = default;

		void read(std::ifstream&, rwIOType type = rwIOType::bin) override;
		void write(std::ofstream&, rwIOType type = rwIOType::bin) const override;
	};

}
