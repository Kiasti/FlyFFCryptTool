#pragma once
/*
//#include "Lexer.h"
#include <array>
#include "Arc.h"
#include <string>
#include <array>
#include <vector>
#include <future>
#include <functional>
#include <atomic>
#include <fstream>
#include <filesystem>
#include <cctype>
#include <queue>

namespace origin
{
#pragma pack(1)
	struct OriginFile
	{
		char fileName[128];
		unsigned long long start;
		unsigned long long end;
	};
#pragma pack()


	class Arc
	{
		std::array<unsigned char, 32> pass;
		std::array<unsigned char, 16> iv;

	public:
		Arc();
		std::string encryptFile(const std::string &fileName, const std::string &outFile);
		static void compressFile(const std::string& fileName, const std::string& outFile);

		std::string decryptString(const std::string &data);
		std::string decompressString(std::string&& in) const;
		std::string decompressString(const std::string& in) const;
		static bool decompressString(const std::string& in, std::string& out);
		static bool decompressFile(const std::string& fileName, std::string& outFile);

		//std::string decrpytFile(const std::string &fileName, const std::string &outFile);
		//static void decompressFile(const std::string& fileName, const std::string& outFile);
		std::array<unsigned char, 32> &key() { return pass; }
		std::array<unsigned char, 16> &iV() { return iv; }
		~Arc();
	};

	class soloArc : public Arc
	{

	public:
		soloArc();
		void packArc(std::string&& folder, std::string&& output);
		~soloArc();
	};


	class multiArc : public Arc
	{
		std::vector<std::string> files;
		std::vector<std::string> dir;
		std::vector<std::string> ignoreFiles;
		std::vector<std::string> vecItems;
		std::filesystem::path resClient;
		std::filesystem::path currentPath;
		std::queue<std::filesystem::path> arcQueue;
		std::string resourceDir;

		std::vector<OriginFile> headerList;

		long fileCount = 0;


	public:
		multiArc();
		bool loadPrerequisiteFiles();
		bool getResClientFolder();
		void createResClient(const std::filesystem::path& path);
		void createFolderForResClient(std::string&& str);
		void convertFileForResClient(const std::filesystem::path& fullFileName);

		void testFile();
		void packArc();
		bool doClient();
		void createArc();
		void createSubHeaders(const std::filesystem::path& path);
		void createSubArcs(const std::filesystem::path& path);
		~multiArc();
	};
}


*/