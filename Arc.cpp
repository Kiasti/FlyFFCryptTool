#include "Arc.h"
/*
#include <string>
#include <array>
#include <vector>
#include <future>
#include <functional>
#include <fstream>
#include <filesystem>
#include <algorithm> 
#include <cctype>
#include <locale>

#include "threadManager.h"
#include "Lexer.h"

#include "../../Libraries/Crypto++/Include/aes.h"
#include "../../Libraries/Crypto++/Include/cryptlib.h"
#include "../../Libraries/Crypto++/Include/files.h"
#include "../../Libraries/Crypto++/Include/modes.h"

#include "../../Libraries/zlib/include/zlib.h"
#pragma comment (lib, "../../libraries/Crypto++/Lib/cryptlib.lib") 
#pragma comment (lib, "../../libraries/zlib/Lib/zlibS.lib") 

#define CHUNK 256000 // 16384 // higher when more memory is good. Check users memory in future.

origin::Arc::Arc()
{
	pass = { 55, 68, 35, 91, 107, 35, 39, 106, 125, 112, 126, 69, 85, 103, 116, 37, 122, 51, 95, 117, 85, 99, 48, 63, 118, 108, 79, 57, 121, 53, 38, 121 };
	iv = { 71, 50, 98, 95, 42, 90, 57, 101, 39, 110, 71, 84, 91, 121, 109, 121 };
}
origin::Arc::~Arc() = default;

// Use CFB mode so we are not restricted to padding to block size. It is malleable but it would just crash the user's client.
// We could set up a resource verification using a better method but this will do for encrypting the file header and each file.
std::string origin::Arc::encryptFile(const std::string &fileName, const std::string &outFile)
{
	CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption e(pass.data(), pass.size(), iv.data(), iv.size());
	CryptoPP::FileSource(fileName.c_str(), true, new CryptoPP::StreamTransformationFilter(e, new CryptoPP::FileSink(outFile.c_str())));
	return outFile;
}

// It is going to be a packed file, so we would recreate the file in memory to apply the decryption on in memory. So we are
// going to be decrpyting string.
std::string origin::Arc::decryptString(const std::string &data)
{
	std::string output;
	CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption aes;
	aes.SetKeyWithIV(pass.data(), pass.size(), iv.data(), iv.size());
	CryptoPP::StringSource(data, true, new CryptoPP::StreamTransformationFilter(aes, new CryptoPP::StringSink(output)));
	return output;
}

void origin::Arc::compressFile(const std::string& fileName, const std::string& outFile)
{
	std::ifstream memes(fileName, std::ios::binary | std::ios::in);
	if (!memes.good())
	{
		std::cout << "Something went wrong with the file" << std::endl;
		return;
	}

	int flush;
	std::vector<unsigned char> buffer(CHUNK);
	std::vector<unsigned char> Out(CHUNK);

	z_stream defstream;
	defstream.zalloc = nullptr;
	defstream.zfree = nullptr;
	defstream.opaque = nullptr;
	if (deflateInit(&defstream, 9) != Z_OK)
		return;

	do {
		const std::streamsize extracted = memes.read(reinterpret_cast<char*>(&buffer[0]), CHUNK).gcount();
		defstream.avail_in = static_cast<unsigned int>(extracted);

		flush = memes.eof() ? Z_FINISH : Z_NO_FLUSH;
		defstream.next_in = buffer.data();

		defstream.avail_out = CHUNK;
		defstream.next_out = Out.data();

		deflate(&defstream, flush);

		std::ofstream oF(outFile, std::ios::binary | std::ios::app);
		oF.write(reinterpret_cast<char*>(&Out[0]), CHUNK - defstream.avail_out);
		oF.close();
	} while (flush != Z_FINISH);

	deflateEnd(&defstream);
	memes.close();
}

std::string origin::Arc::decompressString(std::string&& in) const
{
	std::string decompressed;
	int ret;
	std::vector<unsigned char> buffer(CHUNK);
	std::vector<unsigned char> Out(CHUNK);

	z_stream infstream;
	infstream.zalloc = nullptr;
	infstream.zfree = nullptr;
	infstream.opaque = nullptr;
	infstream.avail_in = 0;
	infstream.next_in = nullptr;

	if (inflateInit(&infstream) != Z_OK)
	{
		return "";
	}

	long long maxRead = in.length();
	do {
		std::streampos currentPos = maxRead >= CHUNK ? CHUNK : maxRead;
		maxRead -= currentPos;
		std::copy(in.begin(), in.begin() + currentPos, buffer.begin());
		infstream.avail_in = static_cast<unsigned int>(currentPos);
		if (infstream.avail_in == 0)
			break;

		infstream.next_in = buffer.data();
		do {
			infstream.avail_out = CHUNK;
			infstream.next_out = Out.data();

			ret = inflate(&infstream, Z_NO_FLUSH);
			switch (ret)
			{
				case Z_NEED_DICT:
				case Z_DATA_ERROR:
				case Z_MEM_ERROR:
					inflateEnd(&infstream);
					return "";
				default: break;
			}
			decompressed += std::string(reinterpret_cast<const char*>(&Out[0]), Out.size());
		} while (infstream.avail_out == 0);
	} while (ret != Z_STREAM_END);
	inflateEnd(&infstream);
	return decompressed;
}

std::string origin::Arc::decompressString(const std::string& in) const
{
	std::string decompressed;
	int ret;
	std::vector<unsigned char> buffer(CHUNK);
	std::vector<unsigned char> Out(CHUNK);

	z_stream infstream;
	infstream.zalloc = nullptr;
	infstream.zfree = nullptr;
	infstream.opaque = nullptr;
	infstream.avail_in = 0;
	infstream.next_in = nullptr;

	if (inflateInit(&infstream) != Z_OK)
	{
		return "";
	}

	long long maxRead = in.length();
	do {
		std::streampos currentPos = maxRead >= CHUNK ? CHUNK : maxRead;
		maxRead -= currentPos;
		std::copy(in.begin(), in.begin() + currentPos, buffer.begin());
		infstream.avail_in = static_cast<unsigned int>(currentPos);
		if (infstream.avail_in == 0)
			break;

		infstream.next_in = buffer.data();
		do {
			infstream.avail_out = CHUNK;
			infstream.next_out = Out.data();

			ret = inflate(&infstream, Z_NO_FLUSH);
			switch (ret)
			{
				case Z_NEED_DICT:
				case Z_DATA_ERROR:
				case Z_MEM_ERROR:
					inflateEnd(&infstream);
					return "";
				default: break;
			}
			decompressed.append(reinterpret_cast<char*>(&Out[0]), Out.size());
		} while (infstream.avail_out == 0);
	} while (ret != Z_STREAM_END);
	inflateEnd(&infstream);
	return decompressed;
}

bool origin::Arc::decompressFile(const std::string& fileName, std::string& outFile)
{
	std::ifstream memes(fileName, std::ios::binary);
	if (!memes.good())
	{
		std::cout << "Something went wrong with the file" << std::endl;
		return false;
	}
	std::string decompressed;

	int ret;
	std::vector<unsigned char> buffer(CHUNK);
	std::vector<unsigned char> Out(CHUNK);

	z_stream infstream;
	infstream.zalloc = nullptr;
	infstream.zfree = nullptr;
	infstream.opaque = nullptr;
	infstream.avail_in = 0;
	infstream.next_in = nullptr;

	if (inflateInit(&infstream) != Z_OK)
	{
		std::cout << "An Error has occurred" << std::endl;
		return false;
	}

	do {
		infstream.avail_in = static_cast<unsigned int>(memes.read(reinterpret_cast<char*>(&buffer[0]), CHUNK).gcount());
		if (infstream.avail_in == 0)
			break;

		infstream.next_in = buffer.data();

		do {
			infstream.avail_out = CHUNK;
			infstream.next_out = Out.data();

			ret = inflate(&infstream, Z_NO_FLUSH);
			switch (ret)
			{
				case Z_NEED_DICT:
				case Z_DATA_ERROR:
				case Z_MEM_ERROR:
					inflateEnd(&infstream);
					return false;
				default: break;
			}
			decompressed += std::string(reinterpret_cast<const char*>(&Out[0]), Out.size());
		} while (infstream.avail_out == 0);
	} while (ret != Z_STREAM_END);
	inflateEnd(&infstream);
	outFile = decompressed;
	return true;
}



origin::soloArc::soloArc() = default;
origin::soloArc::~soloArc() = default;

void origin::soloArc::packArc(std::string&& folder, std::string&& output)
{
	if (!std::filesystem::is_directory(folder))
	{
		std::cout << "No existing main Directory meaning empty directory or wrong directory" << std::endl;
		return;
	}

	if (!std::filesystem::is_directory(output))
	{
		if (!std::filesystem::create_directory(output))
			std::cout << "Failed to create dir: " << output << std::endl;
	}

	ThreadPool tp;
	tp.init();

	std::vector<std::future<bool>> results;
	for (auto &upper : std::filesystem::directory_iterator(folder))
	{
		if (const std::filesystem::path& parentPath = upper.path(); !std::filesystem::is_directory(parentPath))
		{
			results.emplace_back(tp.enqueue([=]() -> bool
			{
				const std::string nigStr = parentPath.filename().string();
				const std::string temp = output + nigStr;
				compressFile(nigStr, temp + "c");
				encryptFile(temp + "c", temp);
				std::filesystem::remove(temp + "c");
				return true;
			}));
		}
	}
	for (const auto& result : results)
		result.wait();

	results.clear();
	tp.end();


	unsigned long long pos = 0;
	for (auto &files : std::filesystem::directory_iterator(output))
	{
		if (const std::filesystem::path& item = files.path(); !std::filesystem::is_directory(item))
		{
			if (item.extension().compare(".arc") == 0 || item.extension().compare(".header") == 0)
				continue;

			std::ofstream main(output + "Data.arc", std::ios::binary | std::ios::app);
			std::ifstream itemFile(item, std::ios::binary);
			main << itemFile.rdbuf();
			itemFile.close();
			main.close();

			std::ofstream hdr(output+ "Data.header", std::ios::binary | std::ios::app);
			const std::string wew = item.filename().string();
			const uintmax_t size = std::filesystem::file_size(item);
			hdr << wew << '|' << pos << '|' << size << '|';
			pos += size;
			hdr.close();
		}
	}
}





origin::multiArc::multiArc() = default;
origin::multiArc::~multiArc() = default;

bool origin::multiArc::loadPrerequisiteFiles()
{
	std::string line;
	std::ifstream fileCheck("doFile.txt", std::ios::binary);
	if (!fileCheck.good())
	{
		std::cout << "You're missing doFile.txt, you fucking doFile." << std::endl;
		return false;
	}
	files.reserve(50);
	dir.reserve(20);
	ignoreFiles.reserve(20);

	while (std::getline(fileCheck, line))
	{
		std::size_t stringPos = line.find("doFile");
		if (stringPos != std::string::npos)
		{
			std::string newStr = line.substr(stringPos + 6);
			Lexer::trimString(newStr);
			files.emplace_back(newStr);
			continue;
		}
		stringPos = line.find("doDir");
		if (stringPos != std::string::npos)
		{
			std::string newStr = line.substr(stringPos + 5);
			Lexer::trimString(newStr);
			dir.emplace_back(newStr);
		}
		stringPos = line.find("doIgnore");
		if (stringPos != std::string::npos)
		{
			std::string newStr = line.substr(stringPos + 8);
			Lexer::trimString(newStr);
			ignoreFiles.emplace_back(newStr);
		}
	}
	fileCheck.close();
	return true;
}

bool origin::multiArc::getResClientFolder()
{
	// get current folder
	if (currentPath.has_parent_path())
	{
		//get parent dir with resclient
		std::filesystem::path parent = currentPath.parent_path();
		parent /= "ResClient";

		// if directory exists skip else create
		if (!std::filesystem::is_directory(parent))
		{
			if (!std::filesystem::create_directory(parent))
			{
				std::cout << "cannot create res client directory" << std::endl;
				return false;
			}
		}
		// set reclient path
		resClient = parent;
		std::cout << "found output folder: " << parent.string() << std::endl;
		return true;
	}
	return false;
}

void origin::multiArc::createFolderForResClient(std::string&& str)
{
	std::string resClientVer = resClient.string();
	resClientVer += str;
	if (!std::filesystem::is_directory(resClientVer))
	{
		if (const std::vector<std::string>::iterator it = std::find(std::begin(dir), std::end(dir), str); it != dir.end())
			if (!std::filesystem::create_directory(resClientVer))
				std::cout << "Failed to create dir: " << resClientVer << std::endl;
	}
}

void origin::multiArc::convertFileForResClient(const std::filesystem::path& fullFileName)
{
	// get ending location
	std::filesystem::path endingFile = resClient;
	endingFile /= fullFileName.filename();

	//starting file 
	const std::string tempStr = endingFile.string() + "c"; // compressed
	compressFile(fullFileName.string(), tempStr); // full file being old dir to resclient + c
	encryptFile(tempStr, endingFile.string()); 
	std::filesystem::remove(tempStr);
}


// Start at current_directory then stack down. It will build .header then .arc for each folder separately. 
void origin::multiArc::createResClient(const std::filesystem::path& startPath)
{
	// If a file is added, the position / size of header will increment
	long long position = 0;

	// We need to create the output path. We grab the folder difference from the current path (start) and
	// what we are expecting. Then append. Ie: main/world/wdMadrigal -> resclient/world/wdMadrigal
	// technically this will be nothing for first run. P = resclient
	auto currentPathLen = currentPath.string().length();
	std::string folders = startPath.string().substr(currentPathLen);
	std::filesystem::path p = resClient;
	p /= folders;

	// We iterate over the full directory to just find files. and handle each file. Make sure it is a file we dont ignore and then if its not 
	// main dir we wont need to see if it is a doFile either. Our outpath is created Ie: p + filename; File is compressed, then encrypted + removed.
	for (auto &filesInDir : std::filesystem::directory_iterator(startPath))
	{
		if (const std::filesystem::path& filePath = filesInDir.path(); !std::filesystem::is_directory(filePath))
		{
			std::string fileNameLocation = filePath.filename().string();
			if (std::vector<std::string>::const_iterator ignore = std::find(std::cbegin(ignoreFiles), std::cend(ignoreFiles), fileNameLocation); ignore == ignoreFiles.end())
			{
				if (currentPath == startPath)
				{
					if (std::vector<std::string>::const_iterator it = std::find(std::cbegin(files), std::cend(files), fileNameLocation); it == files.end())
						continue;
				}
				std::cout << "Compressing and encrypting file: " << fileNameLocation;
				std::filesystem::path outPath = p;
				outPath /= fileNameLocation;

				std::transform(fileNameLocation.begin(), fileNameLocation.end(), fileNameLocation.begin(), ::tolower);
				compressFile(filePath.string(), outPath.string() + ".compressed");
				if (fileNameLocation == "character-etc.txt.txt")
				{
					std::filesystem::copy(outPath.string() + ".compressed", outPath.string() + ".zlibfile");
				}
				encryptFile(outPath.string() + ".compressed", outPath.string());
				std::filesystem::remove(outPath.string() + ".compressed");


				uintmax_t size = std::filesystem::file_size(outPath.string());
				std::ofstream main(p.string() + ".HeaderU", std::ios::binary | std::ios::app);
				main << fileNameLocation << '|' << position << '|' << size  << '|';
				main.close();
				position += size;

				// out path item gets added to q because we want to add in exact order to .arc
				arcQueue.push(outPath);
			}
		}
	}

	if (position != 0 && !arcQueue.empty())
	{
		// compress + encrypt header info
		compressFile(p.string() + ".HeaderU", p.string() + ".Header.compressed");
		encryptFile(p.string() + ".Header.compressed", p.string() + ".Header");

		std::cout << "Made header files" << std::endl;

		uintmax_t headerLen = std::filesystem::file_size(p.string() + ".Header");

		std::ifstream openHeader(p.string() + ".Header", std::ios::binary | std::ios::in);
		std::ofstream outArc(p.string() + ".Arc", std::ios::binary | std::ios::app);
		outArc.write(reinterpret_cast<const char*>(&headerLen), sizeof(long long));
		outArc << openHeader.rdbuf();
		openHeader.close();

		while (!arcQueue.empty())
		{
			if (std::ifstream fileInQ(arcQueue.front(), std::ios::binary | std::ios::in); fileInQ.good())
			{
				outArc << fileInQ.rdbuf();
				fileInQ.close();
			}
			else
			{
				std::cout << "incorrect file in q " << arcQueue.front().string() << std::endl;
			}
			arcQueue.pop();
		}
		outArc.close();
	}

	// now we check for directories to run the same code!
	for (auto& directories : std::filesystem::directory_iterator(startPath))
	{
		if (std::filesystem::is_directory(directories)) 
		{
			std::string doWeDoThisFolder = directories.path().string();
			if (std::vector<std::string>::const_iterator it = std::find(std::cbegin(dir), std::cend(dir), doWeDoThisFolder); it != dir.end())
			{
				std::cout << "Create this folder: " << doWeDoThisFolder << std::endl;
				if (!std::filesystem::create_directory(p))
					std::cout << "Failed to create dir: " << p.string() << std::endl;
				else
					createResClient(directories.path()); // next dir :)
			}
		}
	}
}



// We will need to store the header after the file is loaded through AddResource. This will store the header inside of the game so we
// wont need to get the information constantly to find a file. We can create a map where our key value is the file name. The client
// creates the file names in relative path from the main folder before acquiring the file, so this is valid for us.
//std::map<std::string, OriginFile::blockInfo> originList;

bool origin::multiArc::doClient()
{
	currentPath = std::filesystem::current_path();
	//load merge files
	if (!loadPrerequisiteFiles()) { return false; }
	//obtain resclient folder / output dir
	if (!getResClientFolder())
	{
		std::cout << "Cannot get resclient dir and cant create dir" << std::endl;
		return false;
	}
	//create files
	createResClient(currentPath);
	return true;
}

void origin::multiArc::createArc()
{
	if (doClient())
		std::cout << "success!" << std::endl;
}


#include <windows.h>

void origin::multiArc::testFile()
{
	currentPath = std::filesystem::current_path();
	std::ifstream fileTest(".Arc", std::ios::binary | std::ios::in);
	long long headerSize = 0;
	fileTest.read(reinterpret_cast<char*>(&headerSize), sizeof(long long));
	std::cout << "H len: " << headerSize << std::endl;
	
	std::string headerInMem;
	headerInMem.resize(headerSize);
	fileTest.read(&headerInMem[0], headerSize);
	headerInMem = decompressString(decryptString(headerInMem));

	auto headerLen = headerInMem.length();
	std::string fileName;
	long long loopCnt = 0;
	unsigned long long stringFind = headerInMem.find('|');
	unsigned long long stringStart = 0;
	OriginFile of{};
	std::map<std::string, OriginFile> listOfHeaders;
	while (stringFind != std::string::npos)
	{
		switch (loopCnt % 3)
		{
			case 0:
				fileName = headerInMem.substr(stringStart, stringFind - stringStart);
				std::transform(fileName.begin(), fileName.end(), fileName.begin(), ::tolower);
				break;
			case 1:
				of.start = std::stoll(headerInMem.substr(stringStart, stringFind - stringStart)) + 8 + headerSize;
				break;
			case 2:
				of.end = std::stoll(headerInMem.substr(stringStart, stringFind - stringStart));
				listOfHeaders.insert(std::make_pair(fileName, of));
			default:
				break;
		}
		loopCnt++;

		stringStart = stringFind;
		if (stringStart++ >= headerLen)
			break;
		stringFind = headerInMem.find('|', stringStart);
	}

	if (auto iter = listOfHeaders.find("character-etc.txt.txt"); iter != listOfHeaders.end())
	{
		fileTest.seekg(iter->second.start);
		std::string tester;
		tester.resize(iter->second.end);

		fileTest.read(&tester[0], iter->second.end);
		tester = decryptString(tester);
		std::ofstream outThis("characterdecrpyt.test", std::ios::binary | std::ios::app);
		outThis << tester;
		outThis.close();
		tester = decompressString(tester);
		//std::cin.get();
	
		unsigned int nSize = static_cast<unsigned int>(tester.length() + 2);
		if (char * m_pProg = &tester[0]; static_cast<unsigned char>(*(m_pProg + 0)) == 0xff && static_cast<unsigned char>(*(m_pProg + 1)) == 0xfe) // is unicode ?
		{
			char* lpMultiByte = new char[nSize];
			if (const int nResult = WideCharToMultiByte(1252, 0, reinterpret_cast<LPCWSTR>(m_pProg + 2), -1, lpMultiByte, static_cast<int>(nSize), nullptr, nullptr); nResult > 0)
			{
				lpMultiByte[nResult - 1] = 0;
				memcpy(m_pProg, lpMultiByte, static_cast<size_t>(nResult));
			}
			delete[](lpMultiByte);
		}
	}
	else
	{
		std::cout << "cancer" << std::endl;
	}
	std::cout << "done" << std::endl;
	
}
*/


/* Just incase we want to decrpyt File in the future */
/*std::string Arc::decrpytFile(const std::string &fileName, const std::string &outFile)
{
	std::string output;
	CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption aes;
	aes.SetKeyWithIV(pass.data(), pass.size(), iv.data(), iv.size());
	CryptoPP::FileSource(fileName.c_str(), true, new CryptoPP::StreamTransformationFilter(aes, new CryptoPP::FileSink(outFile.c_str())));
	return output;
}*/

/* Just in Case We want to decompress file in the future */
/*void Arc::decompressFile(const std::string& fileName, const std::string& outFile)
{
	std::ifstream memes(fileName, std::ios::binary);
	if (!memes.good())
	{
		std::cout << "Something went wrong with the file" << std::endl;
		return;
	}

	// create local variables
	int ret;
	std::vector<unsigned char> buffer(CHUNK);
	std::vector<unsigned char> Out(CHUNK);

	// Initialize zlib start
	z_stream infstream;
	infstream.zalloc = nullptr;
	infstream.zfree = nullptr;
	infstream.opaque = nullptr;
	infstream.avail_in = 0;
	infstream.next_in = nullptr;

	if (inflateInit(&infstream) != Z_OK)
	{
		std::cout << "An Error has occured" << std::endl;
		return;
	}

	do {
		infstream.avail_in = static_cast<unsigned int>(memes.read(reinterpret_cast<char*>(&buffer[0]), CHUNK).gcount());
		if (infstream.avail_in == 0)
			break;
		infstream.next_in = buffer.data();

		do 
		{
			infstream.avail_out = CHUNK;
			infstream.next_out = Out.data();

			ret = inflate(&infstream, Z_NO_FLUSH);
			switch (ret)
			{
				case Z_NEED_DICT:
				case Z_DATA_ERROR:
				case Z_MEM_ERROR:
					inflateEnd(&infstream);
					return;
				default: break;
			}

			std::ofstream oF(outFile, std::ios::binary | std::ios::app);
			oF.write(reinterpret_cast<char*>(&Out[0]), CHUNK - infstream.avail_out);
			oF.close();
		} while (infstream.avail_out == 0);
	} while (ret != Z_STREAM_END);

	inflateEnd(&infstream);
	memes.close();
}*/