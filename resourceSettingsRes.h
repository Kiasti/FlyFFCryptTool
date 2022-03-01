#pragma once
/**
*	@file	resourceSettingsRes.h
* 
*	@brief	Settings are split multiple ways. This file contains the specific settings for
*			extracting each style of resource. The user should be able to store tables and
*			keys that are used globally or hardcoded in the flyff source, in settings and 
*			alter them for other servers.
* 
*	@author	Kia (https://github.com/Kiasti)
*/

#include "cryptEngine.h"
#include "resource.h"
#include "resourcePackSettings.h"
#include "rwIO.h"

namespace res::settings
{
	//
	// Global Info Settings
	//
	struct ResGlobalInfo : rwIO
	{
		ResGlobalInfo() = default;
		explicit ResGlobalInfo(std::ifstream&, bool) { }
		~ResGlobalInfo() override = default;

		ResGlobalInfo(ResGlobalInfo const&) = default;
		ResGlobalInfo& operator =(ResGlobalInfo const&) = default;
		ResGlobalInfo(ResGlobalInfo&&) = default;
		ResGlobalInfo& operator=(ResGlobalInfo&&) = default;

		void write(std::ofstream&) const override {}
		void read(std::ifstream&) override {}
		void writeStringFile(std::ofstream&) const override {}
		void readStringFile(std::ifstream&) override {}
	};

	struct ForsakenGlobalInfo final : ResGlobalInfo
	{
		char sha_hash[128]{ "m3ntu5d3rHur3ns00hn\0" };
		char secondExt[16]{ ".flyt\0" };
		cryptEngine::saltPos saltPos{ cryptEngine::saltPos::beg };
	
		ForsakenGlobalInfo() = default;
		~ForsakenGlobalInfo() override = default;
		explicit ForsakenGlobalInfo(std::ifstream& ifs, bool bin = true);

		ForsakenGlobalInfo(ForsakenGlobalInfo const&) = default;
		ForsakenGlobalInfo& operator =(ForsakenGlobalInfo const&) = default;
		ForsakenGlobalInfo(ForsakenGlobalInfo&&) = default;
		ForsakenGlobalInfo& operator=(ForsakenGlobalInfo&&) = default;
	
		void write(std::ofstream& ofs) const override;
		void read(std::ifstream& ifs) override;
		void writeStringFile(std::ofstream& ofs) const override;
		void readStringFile(std::ifstream& ifs) override;
	};

	struct EquinoxGlobalInfo final : ResGlobalInfo
	{
		char compareStr[20]{};
		std::array<unsigned char, 256> keyArr{};

		EquinoxGlobalInfo() = default;
		explicit EquinoxGlobalInfo(std::ifstream& ifs, bool bin = true);
		~EquinoxGlobalInfo() override = default;

		EquinoxGlobalInfo(EquinoxGlobalInfo const&) = default;
		EquinoxGlobalInfo& operator =(EquinoxGlobalInfo const&) = default;
		EquinoxGlobalInfo(EquinoxGlobalInfo&&) = default;
		EquinoxGlobalInfo& operator=(EquinoxGlobalInfo&&) = default;
	
		void write(std::ofstream& ofs) const override;
		void read(std::ifstream& ifs) override;
		void writeStringFile(std::ofstream& ofs) const override;
		void readStringFile(std::ifstream& ifs) override;
	};

	struct InsigniaGlobalInfo final : ResGlobalInfo
	{
		unsigned long hdrKeyLen{};
		std::vector<unsigned char> hdrKeys;
		std::array<std::pair<unsigned char, unsigned char>, 10> fileTypeToKey;
		unsigned short offsetPos{}; // not defaultly in
	
		InsigniaGlobalInfo() = default;
		explicit InsigniaGlobalInfo(std::ifstream& ifs, bool bin = true);

		~InsigniaGlobalInfo() override = default;

		InsigniaGlobalInfo(InsigniaGlobalInfo const&) = default;
		InsigniaGlobalInfo& operator =(InsigniaGlobalInfo const&) = default;
		InsigniaGlobalInfo(InsigniaGlobalInfo&&) = default;
		InsigniaGlobalInfo& operator=(InsigniaGlobalInfo&&) = default;
	
		void write(std::ofstream&) const override;
		void read(std::ifstream&) override;
		void writeStringFile(std::ofstream&) const override;
		void readStringFile(std::ifstream&) override;
	};

	struct CloudGlobalInfo final : ResGlobalInfo
	{
		std::array<unsigned char, 3> keyTable{};
	
		CloudGlobalInfo() = default;
		explicit CloudGlobalInfo(std::ifstream& ifs, bool bin = true);
		~CloudGlobalInfo() override = default;
		CloudGlobalInfo(CloudGlobalInfo const&) = default;
		CloudGlobalInfo& operator =(CloudGlobalInfo const&) = default;
		CloudGlobalInfo(CloudGlobalInfo&&) = default;
		CloudGlobalInfo& operator=(CloudGlobalInfo&&) = default;
	
		void write(std::ofstream& ofs) const override;
		void read(std::ifstream& ifs) override;
		void writeStringFile(std::ofstream& ofs) const override;
		void readStringFile(std::ifstream& ifs) override;
	};

	struct MoonGlobalInfo final : ResGlobalInfo
	{
		unsigned char hardcodedKey{};
	
		MoonGlobalInfo() = default;
		explicit MoonGlobalInfo(std::ifstream& ifs, bool bin = true);
		~MoonGlobalInfo() override = default;
		MoonGlobalInfo(MoonGlobalInfo const&) = default;
		MoonGlobalInfo& operator =(MoonGlobalInfo const&) = default;
		MoonGlobalInfo(MoonGlobalInfo&&) = default;
		MoonGlobalInfo& operator=(MoonGlobalInfo&&) = default;
	
		void write(std::ofstream& ofs) const override;
		void read(std::ifstream& ifs) override;
		void writeStringFile(std::ofstream& ofs) const override;
		void readStringFile(std::ifstream& ifs) override;
	};

	struct AesGlobalInfo final : ResGlobalInfo
	{
		std::array<unsigned char, 24> keyArr{};
	
		AesGlobalInfo() = default;
		explicit AesGlobalInfo(std::ifstream& ifs, bool bin = true);
		~AesGlobalInfo() override = default;

		AesGlobalInfo(AesGlobalInfo const&) = default;
		AesGlobalInfo& operator =(AesGlobalInfo const&) = default;
		AesGlobalInfo(AesGlobalInfo&&) = default;
		AesGlobalInfo& operator=(AesGlobalInfo&&) = default;
	
		void write(std::ofstream& ofs) const override;
		void read(std::ifstream& ifs) override;
		void writeStringFile(std::ofstream& ofs) const override;
		void readStringFile(std::ifstream& ifs) override;
	};


	struct ResInnerInfo : rwIO
	{
		ResInnerInfo() = default;
		~ResInnerInfo() override = default;

		ResInnerInfo(ResInnerInfo const&) = default;
		ResInnerInfo& operator =(ResInnerInfo const&) = default;
		ResInnerInfo(ResInnerInfo&&) = default;
		ResInnerInfo& operator=(ResInnerInfo&&) = default;
	};

	// resource info
	struct DefaultInnerInfo final : ResInnerInfo
	{
		bool bEncrpytion{};
		unsigned char xorKey{};
		char ver[8]{};
	
		DefaultInnerInfo() = default;
		~DefaultInnerInfo() override = default;

		DefaultInnerInfo(DefaultInnerInfo const&) = default;
		DefaultInnerInfo& operator =(DefaultInnerInfo const&) = default;
		DefaultInnerInfo(DefaultInnerInfo&&) = default;
		DefaultInnerInfo& operator=(DefaultInnerInfo&&) = default;
	
		void write(std::ofstream& ofs) const override;
		void read(std::ifstream& ifs) override;
		void writeStringFile(std::ofstream& ofs) const override;
		void readStringFile(std::ifstream& ifs) override;
	};


	struct InsigniaInnerInfo final : ResInnerInfo
	{
		unsigned char fileType{};

		InsigniaInnerInfo() = default;
		~InsigniaInnerInfo() override = default;

		InsigniaInnerInfo(InsigniaInnerInfo const&) = default;
		InsigniaInnerInfo& operator =(InsigniaInnerInfo const&) = default;
		InsigniaInnerInfo(InsigniaInnerInfo&&) = default;
		InsigniaInnerInfo& operator=(InsigniaInnerInfo&&) = default;

		void write(std::ofstream& ofs) const override;
		void read(std::ifstream& ifs) override;
		void writeStringFile(std::ofstream& ofs) const override;
		void readStringFile(std::ifstream& ifs) override;
	};


	struct AesInnerInfo final : ResInnerInfo
	{
		bool bEncrpyt{};
	
		AesInnerInfo() = default;
		~AesInnerInfo() override = default;
		AesInnerInfo(AesInnerInfo const&) = default;
		AesInnerInfo& operator =(AesInnerInfo const&) = default;
		AesInnerInfo(AesInnerInfo&&) = default;
		AesInnerInfo& operator=(AesInnerInfo&&) = default;
	
		void write(std::ofstream& ofs) const override;
		void read(std::ifstream& ifs) override;
		void writeStringFile(std::ofstream& ofs) const override;
		void readStringFile(std::ifstream& ifs) override;
	};

	struct EquinoxInnerInfo final : ResInnerInfo
	{
		unsigned char hdrLenKey{};
		unsigned char innerKey{}; //hdr itself
		unsigned short version{};
		bool bEncrpyt{};
		unsigned char fileKey{};
	
		EquinoxInnerInfo() = default;
		~EquinoxInnerInfo() override = default;

		EquinoxInnerInfo(EquinoxInnerInfo const&) = default;
		EquinoxInnerInfo& operator =(EquinoxInnerInfo const&) = default;
		EquinoxInnerInfo(EquinoxInnerInfo&&) = default;
		EquinoxInnerInfo& operator=(EquinoxInnerInfo&&) = default;
	
		void write(std::ofstream& ofs) const override;
		void read(std::ifstream& ifs) override;
		void writeStringFile(std::ofstream& ofs) const override;
		void readStringFile(std::ifstream& ifs) override;
	};

	struct MoonInnerInfo final : ResInnerInfo
	{
		unsigned char key{};
		bool bEncrpyt{};
		char ver[5]{};
	
		MoonInnerInfo() = default;
		~MoonInnerInfo() override = default;
		MoonInnerInfo(MoonInnerInfo const&) = default;
		MoonInnerInfo& operator =(MoonInnerInfo const&) = default;
		MoonInnerInfo(MoonInnerInfo&&) = default;
		MoonInnerInfo& operator=(MoonInnerInfo&&) = default;
	
		void write(std::ofstream& ofs) const override;
		void read(std::ifstream& ifs) override;
		void writeStringFile(std::ofstream& ofs) const override;
		void readStringFile(std::ifstream& ifs) override;
	};

	inline ResInnerInfo* createInnerInfo(const file::Type ft)
	{
		switch (ft)
		{
			case file::Type::Default:
				return new DefaultInnerInfo;
			case file::Type::AesGow:
			case file::Type::AesAzure:
				return new AesInnerInfo;
			case file::Type::Insignia:
				return new InsigniaInnerInfo;
			case file::Type::Moon:
				return new MoonInnerInfo;
			case file::Type::Equinox:
				return new EquinoxInnerInfo;
			case file::Type::NewFeiFei:
			case file::Type::Forsaken:
				return new ResInnerInfo;
			case file::Type::Custom:
				return nullptr;
		}
		return nullptr;
	}


	inline ResGlobalInfo* createGlobalInfo(const file::Type ft)
	{
		switch (ft)
		{
			case file::Type::Default:
				return new ResGlobalInfo;
			case file::Type::AesGow:
			case file::Type::AesAzure:
				return new AesGlobalInfo;
			case file::Type::Insignia:
				return new InsigniaGlobalInfo;
			case file::Type::Moon:
				return new MoonGlobalInfo;
			case file::Type::Equinox:
				return new EquinoxGlobalInfo;
			case file::Type::NewFeiFei:
				return new ResGlobalInfo;
			case file::Type::Forsaken:
				return new ForsakenGlobalInfo;
			case file::Type::Custom:
				return nullptr;
		}
		return nullptr;
	}

	inline ResGlobalInfo* createGlobalInfo(const file::Type ft, std::ifstream& ifs, bool bin = true)
	{
		switch (ft)
		{
			case file::Type::Default:
				return new ResGlobalInfo(ifs, bin);
			case file::Type::AesGow:
			case file::Type::AesAzure:
				return new AesGlobalInfo(ifs, bin);
			case file::Type::Insignia:
				return new InsigniaGlobalInfo(ifs, bin);
			case file::Type::Moon:
				return new MoonGlobalInfo(ifs, bin);
			case file::Type::Equinox:
				return new EquinoxGlobalInfo(ifs, bin);
			case file::Type::NewFeiFei:
				return new ResGlobalInfo(ifs, bin);
			case file::Type::Forsaken:
				return new ForsakenGlobalInfo(ifs, bin);
			case file::Type::Custom:
				return nullptr;
		}
		return nullptr;
	}

	struct MainInfo final : rwIO
	{
		file::Type type = file::Type::Default; // as int
		int id = 0;
		char serverName[256]{ "Flyff - Official\0" };
		char extension[16]{ ".res\0" };

		MainInfo() = default;
		~MainInfo() override = default;
		MainInfo(MainInfo const&) = default;
		MainInfo& operator =(MainInfo const&) = default;
		MainInfo(MainInfo&&) = default;
		MainInfo& operator=(MainInfo&&) = default;

		void write(std::ofstream& ofs) const override;
		void read(std::ifstream& ifs) override;
		void writeStringFile(std::ofstream& ofs) const override;
		void readStringFile(std::ifstream& ifs) override;
	};
}
