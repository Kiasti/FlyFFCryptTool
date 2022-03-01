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
		explicit FileSelectionList(std::ifstream & ifs, bool bin = true);
		~FileSelectionList() override = default;

		FileSelectionList(FileSelectionList const&) = default;
		FileSelectionList& operator =(FileSelectionList const&) = default;
		FileSelectionList(FileSelectionList&&) = default;
		FileSelectionList& operator=(FileSelectionList&&) = default;

		void read(std::ifstream & ifs) override;
		void write(std::ofstream & ofs) const override;
		void readStringFile(std::ifstream & ifs) override;
		void writeStringFile(std::ofstream & ofs) const override;
	};

	struct ResInnerInfo;
	struct ResourceSetting final : rwIO
	{
		ResInnerInfo* ptr{ nullptr };
		FileSelectionList fs;

		ResourceSetting() = default;
		explicit ResourceSetting(std::ifstream& ifs, bool bin = true);
		~ResourceSetting() override = default;

		ResourceSetting(ResourceSetting const&) = default;
		ResourceSetting& operator =(ResourceSetting const&) = default;
		ResourceSetting(ResourceSetting&&) = default;
		ResourceSetting& operator=(ResourceSetting&&) = default;

		void read(std::ifstream&) override;
		void write(std::ofstream&) const override;
		void readStringFile(std::ifstream&) override;
		void writeStringFile(std::ofstream&) const override;
	};

	struct ResPack final : rwIO
	{
		char name[128]{};
		ResourceSetting rs;

		ResPack() = default;
		explicit ResPack(std::ifstream& ifs, bool bin = true);
		~ResPack() override = default;

		ResPack(ResPack const&) = default;
		ResPack& operator =(ResPack const&) = default;
		ResPack(ResPack&&) = default;
		ResPack& operator=(ResPack&&) = default;

		void read(std::ifstream&) override;
		void write(std::ofstream&) const override;
		void readStringFile(std::ifstream&) override;
		void writeStringFile(std::ofstream&) const override;
	};

	struct ResPackList final : std::vector<ResPack>, rwIO
	{
		PackLimit pck{};
		long long maxSize{};

		ResPackList() = default;
		explicit ResPackList(std::ifstream& ifs, bool bin = true);
		~ResPackList() override = default;

		ResPackList(ResPackList const&) = default;
		ResPackList& operator =(ResPackList const&) = default;
		ResPackList(ResPackList&&) = default;
		ResPackList& operator=(ResPackList&&) = default;

		void read(std::ifstream&) override;
		void write(std::ofstream&) const override;
		void readStringFile(std::ifstream&) override;
		void writeStringFile(std::ofstream&) const override;
	};

}
