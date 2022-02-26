#include "compressionEngine.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>

#include "zlib/include/zlib.h"

void compressionEngine::zlibIO::compressString(std::string& file)
{
	z_stream defstream;
	defstream.zalloc = nullptr;
	defstream.zfree = nullptr;
	defstream.opaque = nullptr;
	if (deflateInit(&defstream, 9) != Z_OK)
		return;

	int flush;
	size_t readChunk = 0;
	std::string decompressed;
	decompressed.reserve(file.size() * 4);
	const std::string Out(chunk, 0);
	do {
		const bool isEof = chunk + readChunk > file.length() || chunk > file.length();
		const unsigned long long extracted = isEof ? file.length() - readChunk : chunk;
		defstream.avail_in = static_cast<unsigned int>(extracted);

		flush = isEof ? Z_FINISH : Z_NO_FLUSH;
		defstream.next_in = reinterpret_cast<unsigned char*>(&file[readChunk]);
		defstream.avail_out = chunk;
		defstream.next_out = reinterpret_cast<unsigned char*>(Out[0]);

		deflate(&defstream, flush);
		readChunk += extracted;
		decompressed += Out;
	} while (flush != Z_FINISH);
	deflateEnd(&defstream);

	decompressed.shrink_to_fit();
	file = std::move(decompressed);
}

bool compressionEngine::zlibIO::decompressString(std::string& in)
{
	z_stream infstream;
	infstream.zalloc = nullptr;
	infstream.zfree = nullptr;
	infstream.opaque = nullptr;
	infstream.avail_in = 0;
	infstream.next_in = nullptr;

	if (inflateInit(&infstream) != Z_OK)
		return false;

	int ret;
	const std::string outputString(chunk, 0);
	std::string decompressed(in.length() << 1, 0);

	size_t readChunk = 0;
	do
	{
		const unsigned long long extracted = chunk + readChunk > in.length() || chunk > in.length() ? in.length() - readChunk : chunk;
		infstream.avail_in = static_cast<unsigned int>(extracted);
		if (infstream.avail_in == 0)
			break;

		infstream.next_in = reinterpret_cast<unsigned char*>(in[readChunk]);
		do
		{
			infstream.avail_out = chunk;
			infstream.next_out = reinterpret_cast<unsigned char*>(outputString[0]);

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
			decompressed += outputString;
		} while (infstream.avail_out == 0);
		readChunk += extracted;
	} while (ret != Z_STREAM_END);
	inflateEnd(&infstream);

	in = std::move(decompressed);
	return true;
}

void compressionEngine::zlibIO::compressFile(const std::string& fileName, const std::string& output)
{
	std::ifstream memes(fileName, std::ios::binary | std::ios::in);
	if (!memes.good())
	{
		std::cout << "Something went wrong with the file" << std::endl;
		return;
	}

	int flush;
	std::vector<unsigned char> buffer(chunk);
	std::vector<unsigned char> Out(chunk);

	z_stream defstream;
	defstream.zalloc = nullptr;
	defstream.zfree = nullptr;
	defstream.opaque = nullptr;
	if (deflateInit(&defstream, 9) != Z_OK)
		return;

	do
	{
		const std::streamsize extracted = memes.read(reinterpret_cast<char*>(&buffer[0]), chunk).gcount();
		defstream.avail_in = static_cast<unsigned int>(extracted);

		flush = memes.eof() ? Z_FINISH : Z_NO_FLUSH;
		defstream.next_in = buffer.data();

		defstream.avail_out = chunk;
		defstream.next_out = Out.data();

		deflate(&defstream, flush);

		std::ofstream oF(output, std::ios::binary | std::ios::app);
		oF.write(reinterpret_cast<char*>(&Out[0]), chunk - defstream.avail_out);
		oF.close();
	} while (flush != Z_FINISH);

	deflateEnd(&defstream);
	memes.close();
}
