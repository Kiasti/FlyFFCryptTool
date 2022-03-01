/**
*	@file	resource.h
*	@brief	Contains list of file structures used in the project. File structures are separated with 
*			a Header and a list of information in relation to the inner contents.
*
*	@author Kia (https://github.com/Kiasti) 
*/
#pragma once
#include <fstream>
#include <string>
#include <typeinfo>
#include <future>
#include <type_traits>
#include <array>
#include "fileSys.h"

constexpr unsigned char ENC_KEY[]{ 124, 108, 129, 11, 220, 152, 221, 110, 12, 10, 10, 7, 16, 21, 23, 24, 176, 101, 117, 110, 215, 231, 127, 200 }; // gow
constexpr char ENC_EXT[]{ "*.enc" };
constexpr unsigned long UNENCRYPTED_BYTES = sizeof(short) + sizeof(short) + sizeof(bool) + sizeof(int);
constexpr unsigned long tempkey[]{ 0x153A1761, 0x39441E5B, 0x6942486F, 0x7949306c, 0x634C3137, 0x6E684337 };

/** @brief Contains the File Structures used for the pack files. */
namespace res
{
	namespace file
	{
		/** @brief Type enum to use to store statically to differentiate between structures. */
		enum class Type : unsigned int { Default, AesGow, AesAzure, Insignia, Moon, Equinox, Forsaken, NewFeiFei, Cloud, Custom };


		/** @brief Template member deduction for function 'hasStartPos'
		*
		*	@see res::file::flyff::Hdr
		*	@see res::file::other::HdrAes1
		*/
		template <typename T, typename U = int> struct hasStartPos : std::false_type { };
		template <typename T> struct hasStartPos<T, decltype((void)T::defaultStartPos(), 0)> : std::true_type { };

		template <typename T, typename U = int> struct hasFileNum : std::false_type { };
		template <typename T> struct hasFileNum<T, decltype((void)T::fileNumber, 0)> : std::true_type { };

		template <typename T, typename U = int> struct hasRType : std::false_type { };
		template <typename T> struct hasRType<T, decltype((void)T::getResourceType(), 0)> : std::true_type { };

		// todo: fix hasCleanup
		template <typename T, typename U = int> struct hasCleanup : std::false_type { };
		template <typename T> struct hasCleanup<T, decltype((void)T::cleanup(), 0)> : std::true_type { };


		namespace other { struct HdrAesGOW; }

		/** @brief Main types for the game "FlyFF" */
		namespace flyff
		{
			struct Hdr
			{
				unsigned char g_byEncryptionKey;
				bool bEncryption;
				int nMergeFileHeaderSize;

				char version[8]{};
				short fileNumber;
				bool compressed = false;
				bool onebyte = false;
				std::string str;

				Hdr() : g_byEncryptionKey(0), bEncryption(true), nMergeFileHeaderSize(0), fileNumber(0) { }
				explicit Hdr(std::ifstream& ifs);
				void cleanup() { str.resize(0); }
				void mainRead(std::ifstream& ifs);

				[[nodiscard]] static long long defaultStartPos() { return 7 + sizeof(short); }
				[[nodiscard]] unsigned char decryption(unsigned char byData) const;
				[[nodiscard]] static Type getResourceType() { return Type::Default; }
			};

			struct Res
			{
				short nFileLength;
				std::string fileName;
				int fileSize;

				unsigned long time_;
				unsigned long filePos;

				explicit Res(const Hdr& hdr, std::streampos& currentPos);
				explicit Res(const other::HdrAesGOW& hdr, std::streampos& currentPos);
			};
		}


		/** @brief Contains released formats and those that are reverse engineered out of a client. */
		namespace other
		{
			/** @brief Keys reversed engineered from the "insignia" client. Other clients that use this enc may use different sets of keys. */
			static unsigned char encInsignia[2048] = { 245, 56, 252, 103, 108, 3, 50, 54, 66, 224, 246, 178, 42, 171, 214, 226, 3, 135, 177, 91, 216, 130,
				134, 68, 120, 30, 110, 47, 73, 250, 104, 132, 253, 37, 176, 129, 150, 93, 48, 246, 91, 117, 188, 9, 246, 173, 86, 183, 191, 141, 90, 236,
				95, 64, 216, 116, 176, 184, 238, 79, 147, 4, 236, 71, 121, 115, 180, 5, 249, 86, 76, 165, 87, 185, 254, 100, 197, 126, 76, 201, 198, 181,
				111, 186, 160, 122, 37, 190, 76, 60, 208, 221, 109, 223, 99, 88, 135, 117,  142, 132, 247, 82, 188, 105, 65, 180, 149, 164, 64, 117, 174,
				51, 108, 37, 136, 220, 147, 50, 165, 253, 79, 245, 184, 189, 132, 110, 56, 118, 86, 167, 219, 80, 238, 199, 209, 117, 248, 6, 122, 235,
				228, 127, 126, 214, 141, 157, 113, 73, 88, 134, 195, 85, 207, 144, 54, 3, 152, 253, 228, 210, 124, 31, 79, 152, 152, 94, 67, 164, 83, 19,
				227, 141, 183, 184, 25, 206, 119, 224, 49, 230, 251, 238, 31, 207, 226, 135, 128, 94, 146, 206, 219, 249, 33, 166, 112, 197, 49, 229, 62,
				48, 91, 204, 244, 69, 161, 54, 156, 23, 119, 102, 139, 182, 205, 253, 213, 140, 1, 126, 180, 18, 221, 228, 19, 132, 55, 233, 109, 18, 73,
				83, 158, 145, 163, 1, 140, 48, 249, 216, 47, 172, 233, 105, 230, 44, 68, 37, 44, 35, 207, 226, 196, 177, 49, 45, 81, 117, 186, 112, 15,
				182, 200, 139, 204, 37, 199, 248, 20, 204, 113, 166, 146, 39, 80, 9, 39, 116, 252, 229, 65, 18, 190, 74, 65, 102, 142, 235, 97, 118, 175,
				23, 116, 137, 202, 119, 226, 94, 156, 194, 100, 74, 17, 94, 176, 42, 80, 157, 98, 40, 6, 220, 120, 212, 48, 251, 247, 126, 75, 103, 149,
				218, 152, 156, 30, 91, 50, 10, 92, 216, 37, 199, 215, 59, 129, 245, 1, 96, 153, 23, 22, 239, 137, 57, 112, 210, 158, 159, 89, 205, 126,
				101, 20, 53, 45, 136, 177, 116, 65, 85, 246, 71, 66, 137, 4, 62, 5, 61, 83, 58, 250, 152, 228, 52, 82, 165, 81, 96, 45, 75, 188, 146, 175,
				141, 90, 191, 180, 162, 15, 247, 69, 140, 32, 68, 32, 205, 173, 1, 54, 51, 205, 97, 77, 31, 210, 229, 127, 222, 226, 134, 55, 82, 71, 244,
				151, 101, 151, 180, 47, 68, 136, 97, 46, 90, 244, 111, 120, 161, 20, 131, 246, 81, 42, 248, 124, 92, 69, 144, 186, 116, 64, 212, 191, 136,
				192, 242, 69, 42, 171, 186, 55, 248, 208, 241, 169, 108, 52, 241, 42, 99, 109, 220, 203, 95, 142, 229, 40, 78, 121, 60, 32, 7, 248, 186,
				4, 179, 161, 165, 152, 2, 42, 78, 245, 89, 77, 252, 128, 56, 79, 16, 40, 82, 103, 87, 7, 99, 10, 96, 248, 218, 203, 246, 176, 111, 154, 49,
				1, 22, 237, 245, 202, 141, 90, 40, 58, 31, 230, 114, 19, 145, 101, 238, 253, 105, 21, 205, 158, 162, 179, 160, 169, 66, 171, 189, 54, 50, 157,
				134, 189, 28, 252, 106, 74, 89, 69, 153, 247, 63, 29, 231, 123, 150, 77, 104, 118, 220, 247, 72, 91, 204, 145, 93, 97, 78, 76, 136, 225, 156,
				240, 8, 129, 97, 179, 245, 147, 249, 165, 165, 36, 122, 212, 96, 133, 203, 37, 20, 243, 68, 72, 89, 57, 86, 44, 240, 138, 159, 10, 250, 14,
				187, 183, 92, 77, 21, 103, 27, 226, 86, 211, 123, 55, 83, 36, 223, 246, 50, 214, 149, 41, 57, 148, 157, 85, 152, 205, 43, 53, 194, 108, 242,
				240, 169, 152, 104, 193, 121, 19, 113, 101, 11, 49, 157, 111, 203, 34, 229, 51, 75, 248, 88, 200, 115, 209, 211, 40, 27, 8, 180, 70, 60, 228,
				32, 233, 224, 138, 141, 186, 187, 31, 126, 32, 102, 49, 31, 9, 245, 46, 136, 118, 231, 247, 232, 200, 21, 43, 125, 215, 195, 64, 167, 114, 93,
				43, 41, 38, 17, 179, 142, 66, 148, 43, 119, 45, 123, 57, 98, 67, 178, 203, 49, 235, 126, 63, 93, 68, 250, 86, 212, 182, 13, 180, 246, 123, 119,
				12, 194, 60, 8, 248, 246, 67, 159, 71, 224, 189, 129, 188, 78, 219, 199, 59, 37, 246, 16, 186, 242, 30, 204, 228, 42, 185, 221, 214, 53, 223,
				51, 250, 180, 111, 117, 39, 60, 185, 212, 237, 76, 55, 93, 159, 194, 62, 9, 248, 238, 208, 143, 254, 107, 187, 25, 164, 110, 34, 206, 62, 100,
				200, 18, 2, 138, 133, 8, 221, 102, 160, 254, 107, 35, 34, 111, 187, 192, 46, 35, 234, 176, 227, 231, 100, 80, 51, 144, 248, 87, 208, 184, 26,
				234, 150, 137, 131, 38, 92, 21, 61, 54, 43, 147, 78, 209, 37, 220, 74, 77, 92, 227, 65, 254, 226, 140, 127, 223, 173, 176, 87, 244, 15, 134,
				148, 231, 228, 128, 139, 151, 234, 150, 100, 26, 228, 180, 134, 81, 153, 145, 44, 86, 71, 172, 250, 236, 120, 27, 8, 11, 11, 163, 119, 170, 188,
				72, 40, 163, 254, 102, 235, 164, 81, 177, 97, 197, 127, 40, 26, 47, 7, 47, 70, 29, 133, 2, 174, 98, 139, 167, 111, 202, 121, 129, 140, 72, 31,
				185, 92, 157, 141, 225, 81, 52, 164, 134, 6, 148, 75, 53, 109, 81, 2, 216, 4, 219, 93, 91, 37, 187, 153, 197, 25, 87, 245, 34, 44, 207, 38, 25,
				166, 197, 202, 123, 160, 37, 220, 19, 119, 253, 251, 26, 23, 86, 58, 31, 200, 159, 125, 176, 234, 195, 148, 160, 5, 198, 201, 248, 61, 130, 91,
				53, 111, 51, 142, 167, 133, 133, 56, 222, 254, 75, 226, 38, 191, 127, 175, 193, 30, 215, 89, 139, 82, 116, 46, 226, 49, 43, 119, 217, 91, 226,
				212, 244, 163, 198, 170, 226, 199, 146, 152, 68, 104, 158, 177, 159, 223, 111, 224, 132, 24, 8, 219, 101, 42, 207, 226, 249, 5, 171, 119, 91,
				14, 94, 132, 28, 118, 237, 130, 247, 211, 203, 249, 139, 63, 21, 35, 178, 194, 142, 4, 192, 73, 242, 194, 83, 7, 112, 202, 211, 226, 176, 222,
				28, 27, 62, 54, 11, 27, 149, 42, 240, 235, 231, 176, 75, 14, 81, 39, 21, 172, 48, 55, 118, 168, 181, 18, 27, 154, 49, 21, 6, 36, 174, 22, 35,
				236, 221, 112, 1, 222, 25, 152, 46, 100, 18, 211, 97, 66, 119, 69, 181, 74, 181, 39, 178, 122, 105, 115, 100, 44, 27, 111, 232, 128, 36, 204,
				62, 210, 141, 94, 48, 224, 38, 54, 148, 150, 10, 18, 150, 224, 176, 215, 36, 235, 95, 187, 109, 135, 98, 168, 131, 106, 12, 212, 195, 168, 175,
				172, 65, 68, 150, 142, 180, 25, 22, 211, 37, 128, 145, 29, 112, 88, 220, 131, 122, 66, 76, 251, 33, 104, 96, 181, 112, 227, 217, 196, 212, 80,
				113, 235, 229, 92, 208, 82, 75, 35, 35, 11, 213, 138, 151, 236, 245, 70, 57, 44, 141, 136, 90, 130, 93, 179, 139, 216, 227, 223, 30, 208, 63,
				81, 254, 211, 15, 165, 54, 20, 135, 138, 56, 88, 31, 65, 39, 149, 207, 114, 191, 43, 170, 174, 55, 77, 245, 238, 143, 51, 208, 246, 109, 106,
				6, 82, 203, 133, 139, 195, 221, 80, 17, 37, 164, 4, 80, 3, 99, 28, 140, 244, 9, 114, 33, 242, 39, 144, 26, 177, 220, 32, 23, 3, 67, 143, 29,
				204, 83, 204, 117, 160, 11, 13, 147, 112, 26, 133, 10, 52, 251, 243, 139, 115, 165, 218, 238, 250, 74, 18, 44, 60, 108, 216, 229, 189, 57, 15,
				225, 22, 229, 130, 174, 150, 166, 235, 234, 250, 221, 176, 216, 160, 134, 229, 181, 232, 148, 162, 92, 104, 159, 34, 131, 65, 161, 58, 53, 49,
				39, 212, 55, 12, 163, 242, 77, 121, 69, 244, 204, 185, 238, 243, 29, 152, 56, 139, 180, 133, 36, 252, 42, 149, 177, 121, 118, 196, 254, 110,
				243, 108, 183, 186, 212, 9, 122, 205, 251, 147, 152, 61, 163, 158, 213, 161, 121, 171, 5, 233, 9, 23, 132, 174, 13, 41, 171, 160, 89, 42, 125,
				136, 122, 214, 1, 96, 3, 52, 31, 20, 31, 106, 142, 34, 236, 230, 89, 138, 191, 16, 1, 201, 191, 21, 101, 221, 230, 9, 173, 171, 93, 216, 116, 209,
				22, 17, 193, 203, 158, 117, 159, 69, 140, 239, 203, 158, 135, 99, 193, 38, 206, 33, 223, 73, 42, 178, 200, 102, 114, 34,  120, 248, 133, 114, 182,
				69, 244, 188, 12, 219, 37, 141, 235, 74, 112, 187, 62, 206, 73, 197, 87, 132, 70, 136, 220, 61, 248, 43, 168, 99, 71, 92, 90, 124, 251, 165, 190,
				27, 58, 134, 168, 202, 88, 228, 2, 66, 52, 149, 216, 110, 117, 248, 29, 171, 202, 243, 179, 252, 149, 122, 76, 201, 20, 167, 149, 199, 51, 112,
				77, 201, 251, 35, 148, 194, 30, 139, 44, 97, 134, 154, 9, 99, 82, 59, 237, 78, 63, 173, 176, 9, 241, 193, 132, 191, 58, 221, 182, 175, 80, 57, 15,
				195, 73, 15, 113, 82, 101, 90, 142, 64, 199, 83, 118, 250, 114, 232, 120, 216, 185, 71, 137, 133, 190, 69, 215, 59, 157, 212, 136, 63, 211, 124,
				17, 78, 139, 68, 27, 67, 200, 155, 231, 227, 31, 113, 64, 159, 75, 137, 78, 80, 102, 221, 244, 9, 174, 137, 6, 4, 55, 55, 179, 144, 208, 80, 107,
				76, 66, 41, 111, 46, 206, 53, 16, 31, 95, 164, 102, 78, 5, 56, 238, 21, 134, 57, 215, 245, 134, 8, 158, 2, 62, 23, 13, 146, 230, 88, 20, 206, 7,
				243, 93, 35, 26, 32, 128, 49, 206, 214, 169, 11, 36, 183, 253, 253, 71, 159, 10, 152, 119, 117, 196, 58, 137, 110, 238, 184, 41, 125, 55, 7, 145,
				151, 71, 17, 217, 204, 249, 102, 184, 167, 39, 175, 94, 148, 221, 134, 24, 14, 75, 72, 79, 123, 87, 145, 47, 36, 152, 171, 48, 199, 9, 82, 159,
				162, 121, 139, 112, 237, 36, 121, 165, 162, 223, 207, 78, 21, 219, 244, 143, 105, 160, 27, 50, 238, 183, 23, 64, 181, 224, 182, 53, 78, 55, 239,
				158, 212, 17, 186, 27, 82, 23, 42, 232, 208, 161, 171, 154, 135, 67, 26, 71, 163, 29, 198, 39, 237, 32, 175, 234, 99, 76, 185, 13, 109, 69, 88,
				108, 238, 50, 121, 5, 34, 217, 61, 83, 119, 49, 97, 77, 12, 123, 184, 115, 198, 112, 145, 50, 111, 201, 176, 180, 96, 206, 183, 7, 92, 29, 124,
				140, 74, 64, 171, 86, 200, 22, 38, 200, 222, 116, 174, 188, 190, 99, 222, 231, 146, 71, 69, 252, 178, 38, 252, 37, 128, 196, 164, 82, 35, 55,
				245, 90, 180, 211, 3, 203, 206, 99, 169, 177, 74, 199, 82, 249, 27, 242, 244, 252, 239, 207, 115, 21, 215, 127, 216, 36, 66, 134, 212, 5, 113,
				211, 165, 168, 125, 84, 8, 233, 96, 19, 25, 48, 156, 146, 178, 234, 211, 3, 10, 141, 206, 166, 97, 83, 194, 225, 130, 54, 250, 222, 137, 72, 142,
				165, 65, 117, 34, 169, 125, 197, 234, 11, 157, 220, 182, 68, 120, 69, 203, 237, 80, 64, 124, 72, 54, 201, 114, 83, 228, 155, 209, 209, 238, 128,
				137, 207, 161, 246, 82, 62, 200, 42, 132, 68, 161, 195, 10, 132, 155, 127, 105, 62, 105, 13, 142, 73, 202, 70, 112, 119, 3, 206, 196, 224, 243,
				26, 28, 29, 84, 230, 161, 128, 242, 142, 38, 38, 187, 57, 166, 113, 191, 237, 183, 101, 50, 147, 105, 225, 241, 64, 5, 91, 135, 217, 59, 248,
				101, 162, 79, 135, 168, 236, 48, 179, 220, 232, 75, 218, 230, 105, 141, 145, 159, 253, 183, 21, 23, 95, 175, 96, 176, 57, 43, 21, 225, 71, 247,
				25, 84, 44, 192, 25
			};

			struct HdrInsignia
			{
				//header starts at -60 from end. then whatever to end kk.
				int hdrSize;
				unsigned char fileType;

				unsigned char byEncryptionKey;
				bool bEncryption = true;
				std::string headerStr;
				short fileNumber;
				mutable long decryptPos = 0;

				HdrInsignia() : hdrSize(0), fileType(11), byEncryptionKey(0xC4), fileNumber(0) { }
				explicit HdrInsignia(std::ifstream& ifs);
				void cleanup() { headerStr.resize(0); }
				[[nodiscard]] static Type getResourceType() { return Type::Insignia; }
				[[nodiscard]] static long long defaultStartPos() { return sizeof(short); }
				[[nodiscard]] unsigned char decryptionHdr(unsigned char byData) const;
				[[nodiscard]] unsigned char decryptionFile(unsigned char byData) const;
			};

			struct ResInsignia
			{
				short nFileLength;
				std::string fileName;
				int fileSize;

				unsigned long time_;
				unsigned long filePos;

				explicit ResInsignia(const HdrInsignia& hdr, std::streampos& currentPos);
			};

			struct HdrAesGOW
			{
				bool bEncryption;
				int nFileHeaderSize;
				short fileNumber;

				std::string hdrString;
				bool compressed = false;
				explicit HdrAesGOW(std::ifstream& ifs);

				[[nodiscard]] static long long defaultStartPos() { return sizeof(short); }
				void cleanup() { hdrString.resize(0); }
				[[nodiscard]] static Type getResourceType() { return Type::AesGow; }
			};

			struct HdrAesAzure
			{
				unsigned short firstCrc;
				short fileVersion;
				bool bEncryption;
				int headerSize;

				short fileNumber;

				std::string headerStr;
				explicit HdrAesAzure(std::ifstream& ifs);
				static void rotr(unsigned char* data, size_t size);
				static void rotl(unsigned char* data, size_t size);

				void cleanup() { headerStr.resize(0); }
				[[nodiscard]] static long long defaultStartPos() { return sizeof(short); }
				[[nodiscard]] static Type getResourceType() { return Type::AesAzure; }
			};

			struct ResAesAzure
			{
				unsigned long long crc;
				int size;
				int offset;
				long long time_;
				explicit ResAesAzure(const HdrAesAzure& hdr, std::streampos& startPos);
			};

			struct HdrMoon
			{
				unsigned char key;
				bool isEncrpytion;
				unsigned long headerLen;

				std::string str;

				char ver[5]{};
				short fileNumber;

				HdrMoon() : key(0), isEncrpytion(false), headerLen(0), fileNumber(0) { }
				explicit HdrMoon(std::ifstream& ifs);

				[[nodiscard]] static long long defaultStartPos() { return sizeof(short) + (sizeof(char) * 5); }
				[[nodiscard]] static Type getResourceType() { return Type::Moon; }
				[[nodiscard]] unsigned char decryption(unsigned char byData) const;
				void cleanup() { str.resize(0); }
			};

			struct ResMoon
			{
				int offset;
				int fileSize;
				short fileNameLen;
				int time;

				std::string fileName;

				ResMoon() : offset(0), fileSize(0), fileNameLen(0), time(0) { }
				explicit ResMoon(const HdrMoon& hdr, std::streampos& currentPos);

			};

			/** @brief Keys handled by the 'Equinox' style of encryption taken from the release. */
			inline static unsigned char eTable[256] = { 255, 239, 223, 207, 191, 175, 159, 143, 127, 111, 95, 79, 63, 47, 31, 15, 254, 238, 222, 206, 190, 174, 158, 142, 126, 110,
				94, 78, 62, 46, 30, 14, 253, 237, 221, 205, 189, 173, 157, 141, 125, 109, 93, 77, 61, 45, 29, 13, 252, 236, 220, 204, 188, 172, 156, 140, 124, 108, 92, 76, 60,
				44, 28, 12, 251, 235, 219, 203, 187, 171, 155, 139, 123, 107, 91, 75, 59, 43, 27, 11, 250, 234, 218, 202, 186, 170, 154, 138, 122, 106, 90, 74, 58, 42, 26, 10,
				249, 233, 217, 201, 185, 169, 153, 137, 121, 105, 89, 73, 57, 41, 25, 9, 248, 232, 216, 200, 184, 168, 152, 136, 120, 104, 88, 72, 56, 40, 24, 8, 247, 231, 215,
				199, 183, 167, 151, 135, 119, 103, 87, 71, 55, 39, 23, 7, 246, 230, 214, 198, 182, 166, 150, 134, 118, 102, 86, 70, 54, 38, 22, 6, 245, 229, 213, 197, 181, 165,
				149, 133, 117, 101, 85, 69, 53, 37, 21, 5, 244, 228, 212, 196, 180, 164, 148, 132, 116, 100, 84, 68, 52, 36, 20, 4, 243, 227, 211, 195, 179, 163, 147, 131, 115,
				99, 83, 67, 51, 35, 19, 3, 242, 226, 210, 194, 178, 162, 146, 130, 114, 98, 82, 66, 50, 34, 18, 2, 241, 225, 209, 193, 177, 161, 145, 129, 113, 97, 81, 65, 49,
				33, 17, 1, 240, 224, 208, 192, 176, 160, 144, 128, 112, 96, 80, 64, 48, 32, 16, 0
			};

			struct HdrEquinox
			{
				long long headerLen;
				unsigned char xorKey;

				std::string hdr;
				unsigned char xorKey2;

				short ver;
				bool bEncrpytion;
				short fileNumber;

				HdrEquinox();
				explicit HdrEquinox(std::ifstream& ifs);

				[[nodiscard]] static long long defaultStartPos() { return sizeof(short) * 2 + sizeof(bool); }
				[[nodiscard]] static Type getResourceType() { return Type::Equinox; }
				[[nodiscard]] static unsigned char decrpytion(unsigned char byData, unsigned char key);
				void cleanup() { hdr.resize(0); }
			};

			struct ResEquinox
			{
				unsigned char encKey;
				int fileSize;
				unsigned long time_;
				int offset;
				short fileNameLen;

				std::string fileName;

				ResEquinox();
				ResEquinox(const HdrEquinox& hdr, std::streampos& startPos);
			};

			struct HdrForasken
			{
				unsigned long fileNumber;

				std::string hdr;
				HdrForasken() : fileNumber(0) { }
				explicit HdrForasken(std::ifstream& ifs);

				[[nodiscard]] static long long defaultStartPos() { return 0; }
				[[nodiscard]] static Type getResourceType() { return Type::Forsaken; }
				void cleanup() { hdr.resize(0); }

			};
			struct ResForsaken
			{
				unsigned long stringSize;
				std::string fileName;
				long offset;
				long fileSize;

				ResForsaken(const HdrForasken& hdr, std::streampos& startPos);

			};

			struct HdrCloud : flyff::Hdr
			{
				static constexpr unsigned char ENCR_KEY = 0x79;
				static constexpr unsigned char ENCR_MARKER1 = 0x98;
				static constexpr unsigned char ENCR_MARKER2 = 0x99;

				HdrCloud() = default;
				explicit HdrCloud(std::ifstream& ifs);
				void cleanup() { str.resize(0); }

				[[nodiscard]] static long long defaultStartPos() { return 7 + sizeof(short); }
				[[nodiscard]] static Type getResourceType() { return Type::Cloud; }
			};
		}

		/** @brief Chinese version of the game */
		namespace feifei
		{
			// todo: methods to brute force hash.
			inline unsigned long string_id(const char* str)
			{
				static unsigned int m[70]{};
				strncpy(reinterpret_cast<char*>(m), str, 256);

				unsigned int i = 0;
				for (; i < 256 / 4 && m[i]; ++i)
				{
				}

				m[i++] = 0x9BE74448;
				m[i++] = 0x66F42C48;

				unsigned int v = 0xF4FA8928;
				unsigned int edi = 0x7758B42B;
				unsigned int esi = 0x37A8470E;
				for (unsigned int ecx = 0; ecx < i; ++ecx)
				{
					unsigned int ebx = 0x267B0B11;
					v = (v << 1) | (v >> 0x1F);
					ebx ^= v;

					unsigned int eax = m[ecx];
					esi ^= eax;
					edi ^= eax;

					unsigned int edx = ebx;
					edx += edi;
					edx |= 0x02040801;
					edx &= 0xBFEF7FDF;

					long long num = edx;
					num *= esi;
					eax = static_cast<unsigned>(num);
					edx = static_cast<unsigned>(num >> 0x20);
					if (edx != 0)
						eax++;

					num = eax;
					num += edx;
					eax = static_cast<unsigned>(num);
					if (static_cast<unsigned>(num >> 0x20) != 0)
						eax++;

					edx = ebx;
					edx += esi;
					edx |= 0x00804021;
					edx &= 0x7DFEFBFF;

					esi = eax;
					num = edi;
					num *= edx;
					eax = static_cast<unsigned>(num);
					edx = static_cast<unsigned>(num >> 0x20);

					num = edx;
					num += edx;
					edx = static_cast<unsigned>(num);
					if (static_cast<unsigned>(num >> 0x20) != 0)
						eax++;
					num = eax;
					num += edx;

					eax = static_cast<unsigned>(num);
					if (static_cast<unsigned>(num >> 0x20) != 0)
						eax += 2;
					edi = eax;
				}

				esi ^= edi;
				v = esi;
				return v;
			}

			inline char* string_adjust(char* p)
			{
				for (int i = 0; p[i]; i++)
				{
					if (p[i] >= 'A' && p[i] <= 'Z')
						p[i] += 'a' - 'A';
					else if (p[i] == '/')
						p[i] = '\\';
				}
				return p;
			}


			constexpr unsigned long long hdrSize = 12;
			struct Hdr
			{
				unsigned long id = 1464092240;	//'WDFP';
				int fileNumber;					// file count 
				unsigned int offset;			// res start

				std::string str;				// pre load header in to memory to iterate through
				explicit Hdr(std::ifstream& ifs);
				void cleanup() { str.resize(0); }

				[[nodiscard]] static long long defaultStartPos() { return 0; }
				[[nodiscard]] static Type getResourceType() { return Type::NewFeiFei; }

			};

			struct Res
			{
				unsigned long uid;
				unsigned long offset;
				int size;
				unsigned long space;

				Res(const Hdr& header, std::streampos& pos);
			};
		}

		namespace OF
		{
			inline static constexpr unsigned long table[32] = { 1987242877, 989, 9833322, 298452, 629278, 8278275, 6727234, 91786, 828872582, 829288, 1019222, 40200022,
				10, 104141, 202222, 2942425, 294824922, 8675309, 1119833322, 0xFF42FA00, 0xFFF35322, 0x0042420, 0x69694200, 91786, 9036768, 0xCBE107A1, 0x9f9f812b, 
				0x7574bf78, 0xb610fb0d, 0xc4754133, 0x10192220, 1
			};

			inline static std::array<unsigned char, 32> key{ 55, 68, 35, 91, 107, 35, 39, 106, 125, 112, 126, 69, 85, 103, 116, 37, 122, 51, 95, 117, 85, 99, 48, 63, 118, 108, 79, 57, 121, 53, 38, 121 };
			inline static std::array<unsigned char, 16> iv{ 0x55, 0x42, 0x09, 0xF7, 0xE4, 0x06, 0x53, 0xAF, 0xAA, 0xFE, 0x19, 0x87, 0x41, 98, 59, 201 };

			constexpr size_t hdrSize = sizeof(unsigned long long) + sizeof(unsigned char) + sizeof(unsigned short) + sizeof(unsigned long);
			struct Hdr
			{
				unsigned long fileNumber;
				unsigned long key;

				bool compress = true;
				bool isXor = false;

				std::string hdrString;

				Hdr() : fileNumber(0), key(0) {}
				explicit Hdr(std::ifstream& ifs);
				void cleanup() { hdrString.resize(0); }
				[[nodiscard]] static long long defaultStartPos() { return 0; }
				[[nodiscard]] static Type getResourceType() { return Type::Custom; }
				static void xorDecryption(char* start, const size_t len, unsigned char&& k)
				{
					for (size_t i = 0; i < len; )
					{
						start[i] = static_cast<char>(~start[i] ^ k);
						start[i] = static_cast<char>(start[i] << 4 | start[i] >> 4);

						++i;
						i = i << 1;
					}
				}

				static void xorEncryption(char* start, const size_t len, unsigned char k)
				{
					for (size_t i = 0; i < len; )
					{
						start[i] = static_cast<char>(start[i] << 4 | start[i] >> 4);
						start[i] = static_cast<char>(~start[i] ^ k);

						++i;
						i = i << 1;
					}
				}
			};

			constexpr size_t resOriginSize = sizeof(unsigned long long) + sizeof(long) + sizeof(unsigned long) + (sizeof(unsigned char) * 64);
			struct Res
			{
				unsigned long long offset;
				long long size;
				unsigned char hash[64]{ 0, };
				unsigned long keyArr4; 

				Res() : offset(0), size(0), keyArr4(0) {}
				explicit Res(const Hdr& hdr, std::streampos& startPos);
			};
		}
	}

	template <typename res> using Resources = std::vector<res>;
	template <typename hdr, typename res> using ResourceFile = std::pair<hdr, Resources<res>>;
}

namespace rfile = res::file;
namespace rFileOther = res::file::other;
