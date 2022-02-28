#pragma once
#include "cryptEngine.h"
#include "resource.h"

struct rwIO
{
	rwIO() = default;
	~rwIO() = default;

	rwIO(rwIO const&) = default;
	rwIO& operator =(rwIO const&) = default;
	rwIO(rwIO&&) = default;
	rwIO& operator=(rwIO&&) = default;
	
	virtual void write(std::ofstream&) const { }
	virtual void read(std::ifstream&) { }
	virtual void writeStringFile(std::ofstream&) const { }
	virtual void readStringFile(std::ifstream&) { }
};


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



	struct ResInnerInfo : rwIO 
	{
		protected:
			~ResInnerInfo() = default;
		public:
			ResInnerInfo() = default;
			ResInnerInfo(ResInnerInfo const&) = default;
			ResInnerInfo& operator =(ResInnerInfo const&) = default;
			ResInnerInfo(ResInnerInfo&&) = default;
			ResInnerInfo& operator=(ResInnerInfo&&) = default;

	};

	struct FileSelectionList final : std::array<std::vector<std::string>, MAX>, rwIO
	{
		FileSelectionList() = default;
		~FileSelectionList() = default;

		FileSelectionList(FileSelectionList const&) = default;
		FileSelectionList& operator =(FileSelectionList const&) = default;
		FileSelectionList(FileSelectionList&&) = default;
		FileSelectionList& operator=(FileSelectionList&&) = default;

		void read(std::ifstream& ifs) override;
		void write(std::ofstream& ofs) const override;
		void readStringFile(std::ifstream& ifs) override;
		void writeStringFile(std::ofstream& ofs) const override;
	};

	struct ResourceSetting final : rwIO
	{
		ResInnerInfo* ptr{nullptr};
		FileSelectionList fs;

		ResourceSetting() = default;
		~ResourceSetting() = default;

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
		~ResPack() = default;

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
		ResPackList() = default;
		virtual ~ResPackList() = default;

		ResPackList(ResPackList const&) = default;
		ResPackList& operator =(ResPackList const&) = default;
		ResPackList(ResPackList&&) = default;
		ResPackList& operator=(ResPackList&&) = default;

		void read(std::ifstream&) override;
		void write(std::ofstream&) const override;
		void readStringFile(std::ifstream&) override;
		void writeStringFile(std::ofstream&) const override;
	};


	// main info that isnt resource
	struct ResGlobalInfo : rwIO
	{
		PackLimit pck{};
		long long maxSize{};

		bool bDoFile{};
		ResPack doFiles;
		FileSelectionList ignoreList;

		ResGlobalInfo() = default;
		virtual ~ResGlobalInfo() = default;

		ResGlobalInfo(ResGlobalInfo const&) = default;
		ResGlobalInfo& operator =(ResGlobalInfo const&) = default;
		ResGlobalInfo(ResGlobalInfo&&) = default;
		ResGlobalInfo& operator=(ResGlobalInfo&&) = default;

		void write(std::ofstream&) const override;
		void read(std::ifstream&) override;
		void writeStringFile(std::ofstream&) const override;
		void readStringFile(std::ifstream&) override;
	};

	struct ForsakenGlobalInfo final : ResGlobalInfo
	{
		char sha_hash[128]{ "m3ntu5d3rHur3ns00hn\0" };
		char firstExt[16]{ ".fast\0" };
		char secondExt[16]{ ".flyt\0" };
		cryptEngine::saltPos saltPos{ cryptEngine::saltPos::beg };
	
		ForsakenGlobalInfo() = default;
		~ForsakenGlobalInfo() override = default;
		ForsakenGlobalInfo(ForsakenGlobalInfo const&) = default;
		ForsakenGlobalInfo& operator =(ForsakenGlobalInfo const&) = default;
		ForsakenGlobalInfo(ForsakenGlobalInfo&&) = default;
		ForsakenGlobalInfo& operator=(ForsakenGlobalInfo&&) = default;
	
		void write(std::ofstream&) const override { }
		void read(std::ifstream&) override { }
		void writeStringFile(std::ofstream&) const override { }
		void readStringFile(std::ifstream&) override { }
	};

	struct EquinoxGlobalInfo final : ResGlobalInfo
	{
		protected:
			~EquinoxGlobalInfo() override = default;
		public:
			char compareStr[20]{};
			std::array<unsigned char, 256> keyArr{};

			EquinoxGlobalInfo() = default;
			EquinoxGlobalInfo(EquinoxGlobalInfo const&) = default;
			EquinoxGlobalInfo& operator =(EquinoxGlobalInfo const&) = default;
			EquinoxGlobalInfo(EquinoxGlobalInfo&&) = default;
			EquinoxGlobalInfo& operator=(EquinoxGlobalInfo&&) = default;
	
			void write(std::ofstream&) const override { }
			void read(std::ifstream&) override { }
			void writeStringFile(std::ofstream&) const override { }
			void readStringFile(std::ifstream&) override { }
	};

	struct InsigniaGlobalInfo final : ResGlobalInfo
	{
		protected:
			~InsigniaGlobalInfo() override = default;

		public:
			unsigned long hdrKeyLen{};
			std::vector<unsigned char> hdrKeys;
			std::array<std::pair<unsigned char, unsigned char>, 10> fileTypeToKey;
			unsigned short offsetPos{}; // not defaultly in
	
			InsigniaGlobalInfo() = default;
			InsigniaGlobalInfo(InsigniaGlobalInfo const&) = default;
			InsigniaGlobalInfo& operator =(InsigniaGlobalInfo const&) = default;
			InsigniaGlobalInfo(InsigniaGlobalInfo&&) = default;
			InsigniaGlobalInfo& operator=(InsigniaGlobalInfo&&) = default;
	
			void write(std::ofstream&) const override { }
			void read(std::ifstream&) override { }
			void writeStringFile(std::ofstream&) const override { }
			void readStringFile(std::ifstream&) override { }
	
	};

	struct CloudGlobalInfo final : ResGlobalInfo
	{
		protected:
			~CloudGlobalInfo() override = default;
		public:
			std::array<unsigned char, 3> keyTable;
	
			CloudGlobalInfo() = default;
			CloudGlobalInfo(CloudGlobalInfo const&) = default;
			CloudGlobalInfo& operator =(CloudGlobalInfo const&) = default;
			CloudGlobalInfo(CloudGlobalInfo&&) = default;
			CloudGlobalInfo& operator=(CloudGlobalInfo&&) = default;
	
			void write(std::ofstream&) const override { }
			void read(std::ifstream&) override { }
			void writeStringFile(std::ofstream&) const override { }
			void readStringFile(std::ifstream&) override { }
	};

	struct MoonGlobalInfo final : ResGlobalInfo
	{
		protected:
			~MoonGlobalInfo() override = default;
		public:
			unsigned char hardcodedKey{};
	
			MoonGlobalInfo() = default;
			MoonGlobalInfo(MoonGlobalInfo const&) = default;
			MoonGlobalInfo& operator =(MoonGlobalInfo const&) = default;
			MoonGlobalInfo(MoonGlobalInfo&&) = default;
			MoonGlobalInfo& operator=(MoonGlobalInfo&&) = default;
	
			void write(std::ofstream&) const override { }
			void read(std::ifstream&) override { }
			void writeStringFile(std::ofstream&) const override { }
			void readStringFile(std::ifstream&) override { }
	};

	struct AesGlobalInfo : ResGlobalInfo
	{
		protected:
			~AesGlobalInfo() override = default;
		public:
			std::array<unsigned char, 24> keyArr{};
	
			AesGlobalInfo() = default;
			AesGlobalInfo(AesGlobalInfo const&) = default;
			AesGlobalInfo& operator =(AesGlobalInfo const&) = default;
			AesGlobalInfo(AesGlobalInfo&&) = default;
			AesGlobalInfo& operator=(AesGlobalInfo&&) = default;
	
			void write(std::ofstream&) const override { }
			void read(std::ifstream&) override { }
			void writeStringFile(std::ofstream&) const override { }
			void readStringFile(std::ifstream&) override { }
	};

	struct AesAzureInfo final : AesGlobalInfo
	{
		protected:
			~AesAzureInfo() override = default;

		public:
			unsigned long long crc64Polynomial{};

			AesAzureInfo() = default;
			AesAzureInfo(AesAzureInfo const&) = default;
			AesAzureInfo& operator =(AesAzureInfo const&) = default;
			AesAzureInfo(AesAzureInfo&&) = default;
			AesAzureInfo& operator=(AesAzureInfo&&) = default;

			void write(std::ofstream&) const override { }
			void read(std::ifstream&) override { }
			void writeStringFile(std::ofstream&) const override { }
			void readStringFile(std::ifstream&) override { }
	};

	// resource info

	struct DefaultInnerInfo final : ResInnerInfo
	{
		protected:
			~DefaultInnerInfo() = default;
		public:
			bool bEncrpytion{};
			unsigned char xorKey{};
			char ver[8]{};
	
			DefaultInnerInfo() = default;
			DefaultInnerInfo(DefaultInnerInfo const&) = default;
			DefaultInnerInfo& operator =(DefaultInnerInfo const&) = default;
			DefaultInnerInfo(DefaultInnerInfo&&) = default;
			DefaultInnerInfo& operator=(DefaultInnerInfo&&) = default;
	
			void write(std::ofstream&) const override { }
			void read(std::ifstream&) override { }
			void writeStringFile(std::ofstream&) const override { }
			void readStringFile(std::ifstream&) override { }
	};


	struct InsigniaInnerInfo final : ResInnerInfo
	{
		protected:
			~InsigniaInnerInfo() = default;
		public:
			unsigned char fileType{};

			InsigniaInnerInfo() = default;
			InsigniaInnerInfo(InsigniaInnerInfo const&) = default;
			InsigniaInnerInfo& operator =(InsigniaInnerInfo const&) = default;
			InsigniaInnerInfo(InsigniaInnerInfo&&) = default;
			InsigniaInnerInfo& operator=(InsigniaInnerInfo&&) = default;

			void write(std::ofstream&) const override { }
			void read(std::ifstream&) override { }
			void writeStringFile(std::ofstream&) const override { }
			void readStringFile(std::ifstream&) override { }
	};


	struct AesInnerInfo final : ResInnerInfo
	{
		protected:
			~AesInnerInfo() = default;
		public:
			bool bEncrpyt{};
	
			AesInnerInfo() = default;
			AesInnerInfo(AesInnerInfo const&) = default;
			AesInnerInfo& operator =(AesInnerInfo const&) = default;
			AesInnerInfo(AesInnerInfo&&) = default;
			AesInnerInfo& operator=(AesInnerInfo&&) = default;
	
			void write(std::ofstream&) const override { }
			void read(std::ifstream&) override { }
			void writeStringFile(std::ofstream&) const override { }
			void readStringFile(std::ifstream&) override { }
	};

	struct EquinoxInnerInfo final : ResInnerInfo
	{
		protected:
			~EquinoxInnerInfo() = default;
		public:
			unsigned char hdrLenKey{};
			unsigned char innerKey{}; //hdr itself
			unsigned short version{};
			bool bEncrpyt{};
			unsigned char fileKey{};
	
			EquinoxInnerInfo() = default;
			EquinoxInnerInfo(EquinoxInnerInfo const&) = default;
			EquinoxInnerInfo& operator =(EquinoxInnerInfo const&) = default;
			EquinoxInnerInfo(EquinoxInnerInfo&&) = default;
			EquinoxInnerInfo& operator=(EquinoxInnerInfo&&) = default;
	
			void write(std::ofstream&) const override { }
			void read(std::ifstream&) override { }
			void writeStringFile(std::ofstream&) const override { }
			void readStringFile(std::ifstream&) override { }
	};

	struct MoonInnerInfo final : ResInnerInfo
	{
		protected:
			~MoonInnerInfo() = default;
		public:
			unsigned char key{};
			bool bEncrpyt{};
			char ver[5]{};
	
			MoonInnerInfo() = default;
			MoonInnerInfo(MoonInnerInfo const&) = default;
			MoonInnerInfo& operator =(MoonInnerInfo const&) = default;
			MoonInnerInfo(MoonInnerInfo&&) = default;
			MoonInnerInfo& operator=(MoonInnerInfo&&) = default;
	
			void write(std::ofstream&) const override { }
			void read(std::ifstream&) override { }
			void writeStringFile(std::ofstream&) const override { }
			void readStringFile(std::ifstream&) override { }
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


	/** @brief	Small struct that contains write/read functions that will be passed in a chain	*/


	/** @brief	Base Structure that stores the base required data for each server. Servers may have
				different settings and idealy this will be used to pack and extract different clients
				by utilizing polymorphism rather repeating the use of variant, so a type will be
				stored. Otherwise, an extention and a servername will be required.					*/
	struct MainInfo final : rwIO
	{
		file::Type type = file::Type::Default; // as int
		int id = 0;
		char serverName[256]{ "Flyff - Official\0" };
		char extension[16]{ ".res\0" };

		MainInfo() = default;
		~MainInfo() = default;
		MainInfo(MainInfo const&) = default;
		MainInfo& operator =(MainInfo const&) = default;
		MainInfo(MainInfo&&) = default;
		MainInfo& operator=(MainInfo&&) = default;

		void write(std::ofstream& ofs) const override;
		void read(std::ifstream& ifs) override;
		void writeStringFile(std::ofstream& ofs) const override;
		void readStringFile(std::ifstream& ifs) override;
	};
	
	// a Single setting
	class Setting 
	{
		MainInfo mInfo;
		ResGlobalInfo* gInfo{};
		ResPackList rpl;
		public:

			Setting();
			~Setting();
			Setting(Setting const&) = default;
			Setting& operator =(Setting const&) = default;
			Setting(Setting&&) = default;
			Setting& operator=(Setting&&) = default;


			void write(std::ofstream& ofs) const;
			void read(std::ifstream& ifs);
			void writeStringFile(std::ofstream& ofs) const;
			void readStringFile(std::ifstream& ifs);


	};
	using SettingsList = std::vector<Setting>;

	/** @brief	Settings for the official server of the game and servers that use the same setting.
				This mainly being just one byte key that is used with xor encryption and is stored
				in the file itself.																	*/


	/** @brief	An extra Xor key is stored on top of the one from officialSetting. The keys are
				xor'd together.																		*/


	/** @brief	Both the GoW and Azure resource types uses an array of 25 unsigned chars for an
				AES implementation.																	*/




	/** @brief	With this type of file, filenames are hashed with SHA-256. They use a hardcoded
				salt that I extracted. However, other servers using the same style could use a
				different salt or apply the salt at the end of the string instead. They also
				separate header from file contents.													*/




	/**	@brief	Insignia will loop over the length of the header and then will xor it over a
				hardcoded array.																	*/





	/** @brief	Within the Equinox release, they swap out characters based on a hardcoded
				table for the xor encryption														*/




	/** @brief	Possible implementation if I want to port ideas over that relates to this.			*/



}
