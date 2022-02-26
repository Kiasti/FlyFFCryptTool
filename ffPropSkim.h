#pragma once
#include <fstream>
#include "ffProps.h"
#include "paths.hpp"


namespace res::props
{
	static std::set<std::string> skimGmObjectForTextures(std::istringstream& ifs, const unsigned int type, const int folderType = 0)
	{
		std::set<std::string> textureList;

		ifs.seekg(sizeof(types::vec3d<float>) * 2, std::ios::cur);
		ifs.seekg(sizeof(int) * 3, std::ios::cur);
		ifs.seekg(28, std::ios::cur);

		int maxVertList = 0;
		ifs.read(reinterpret_cast<char*>(&maxVertList), sizeof(int));
		int maxVB = 0;
		ifs.read(reinterpret_cast<char*>(&maxVB), sizeof(int));
		int maxFace = 0;
		ifs.read(reinterpret_cast<char*>(&maxFace), sizeof(int));
		int maxIb = 0;
		ifs.read(reinterpret_cast<char*>(&maxIb), sizeof(int));

		ifs.seekg(static_cast<long long>(sizeof(types::vec3d<float>) * maxVertList), std::ios::cur);
		if (type == 1)
			ifs.seekg(static_cast<long long>(skinvertSize * maxVB), std::ios::cur);
		else
			ifs.seekg(static_cast<long long>(normalVertSize * maxVB), std::ios::cur);


		ifs.seekg(static_cast<long long>(sizeof(unsigned short) * maxIb), std::ios::cur);
		ifs.seekg(static_cast<long long>(sizeof(unsigned short) * maxVB), std::ios::cur);


		unsigned int dwTemp = 0; // phys
		ifs.read(reinterpret_cast<char*>(&dwTemp), sizeof(unsigned long));
		if (dwTemp)
			ifs.seekg(static_cast<long long>(sizeof(int) * maxVertList), std::ios::cur);



		int bIsMaterial = 0;
		ifs.read(reinterpret_cast<char*>(&bIsMaterial), sizeof(int));
		if (bIsMaterial)
		{
			// Material count
			ifs.read(reinterpret_cast<char*>(&bIsMaterial), sizeof(int));

			if (bIsMaterial == 0)
				bIsMaterial = 1;

			for (int i = 0; i < bIsMaterial; i++)
			{
				char szBitmap[256];
				ifs.seekg(sizeof(types::d3d9Mat), std::ios::cur);

				int lenlen = 0;
				ifs.read(reinterpret_cast<char*>(&lenlen), sizeof(int));
				ifs.read(szBitmap, static_cast<long long>(lenlen * sizeof(char)));

				if (folderType >= 1)
				{
					textureList.insert(std::string(paths::model_Texture_Hi) + szBitmap);
					textureList.insert(std::string(paths::model_Texture_Mid) + szBitmap);
					textureList.insert(std::string(paths::model_Texture_Low) + szBitmap);
				}
				else
					textureList.insert(szBitmap);
			}
		}

		int matBlockList = 0;
		ifs.read(reinterpret_cast<char*>(&matBlockList), sizeof(int));
		if (matBlockList > 0)
			ifs.seekg(static_cast<long long>(matBlockSize) * matBlockList, std::ios::cur);

		return textureList;
	}

	static std::set<std::string> seekTextureObj(std::istringstream& ifs, const int folderType = 0)
	{
		std::set<std::string> strList{};
		if (!ifs.good())
			return strList;

		char cLen;
		int nVer = 0;
		ifs.read(&cLen, sizeof(char));
		ifs.seekg(cLen, std::ios::cur);
		ifs.read(reinterpret_cast<char*>(&nVer), sizeof(int));

		if (nVer < 20)
			return strList;

		unsigned long seekPos = sizeof(int) + (sizeof(types::vec3d<float>) * 2);
		if (nVer >= 22) // __VER >= 9
			seekPos += sizeof(types::vec3d<float>) * 2;
		seekPos += (sizeof(float) * 2) + 16;
		seekPos += (sizeof(types::vec3d<float>) * 2) + sizeof(float);
		ifs.seekg(seekPos, std::ios::cur);

		int maxFrame = 0;
		ifs.read(reinterpret_cast<char*>(&maxFrame), sizeof(int));


		int temp = 0;
		int temp2 = 0;
		// m_nMaxEvent
		ifs.read(reinterpret_cast<char*>(&temp), sizeof(int));
		if (temp > 0) 
			ifs.seekg(static_cast<long long>(sizeof(types::vec3d<float>) * temp), std::ios::cur);


		// Collision ntemp
		ifs.read(reinterpret_cast<char*>(&temp), sizeof(int));
		if (temp)
		{
			std::set<std::string> tempList = skimGmObjectForTextures(ifs, 0, folderType);
			strList.insert(tempList.begin(), tempList.end());
		}



		ifs.read(reinterpret_cast<char*>(&temp2), sizeof(int)); // lod
		ifs.read(reinterpret_cast<char*>(&temp), sizeof(int)); //bone
		if (temp > 0)
		{
			ifs.seekg(static_cast<long long>((sizeof(types::matrix<float>) * 2) * temp), std::ios::cur);
			if (maxFrame > 0)
			{
				for (int i = 0; i < temp; i++)
				{
					int	nLen = 0;
					ifs.read(reinterpret_cast<char*>(&nLen), sizeof(int));
					ifs.seekg(static_cast<long long>((sizeof(char) * nLen) + (sizeof(types::matrix<float>) * 2) + sizeof(int)), std::ios::cur);
				}
				ifs.seekg(sizeof(int), std::ios::cur); //ani count



				for (int i = 0; i < temp; ++i)
				{
					int nFrame = 0;
					ifs.read(reinterpret_cast<char*>(&nFrame), sizeof(int));
					if (nFrame == 1)
						ifs.seekg(static_cast<long long>(tmAniSize * maxFrame), std::ios::cur);
					else
						ifs.seekg(sizeof(types::matrix<float>), std::ios::cur);
				}
			}
			ifs.seekg(sizeof(int), std::ios::cur);//resFp.Read(&m_bSendVS, 4, 1);
		}

		const int nMaxGroup = (temp2 != 0) ? 3 : 1;
		ifs.seekg(sizeof(int), std::ios::cur); //nPoolSize
		for (int i = 0; i < nMaxGroup; i++)
		{
			ifs.read(reinterpret_cast<char*>(&temp), sizeof(int));

			for (int j = 0; j < temp; j++)
			{
				int type = 0;
				ifs.read(reinterpret_cast<char*>(&type), sizeof(int));
				type = type & 0xffff;

				int maxBone = 0;
				ifs.read(reinterpret_cast<char*>(&maxBone), sizeof(int));
				if (maxBone > 0)
					ifs.seekg(static_cast<long long>(sizeof(int) * maxBone), std::ios::cur);

				ifs.seekg(sizeof(int), std::ios::cur); //objmnid

				int parent = 0;
				ifs.read(reinterpret_cast<char*>(&parent), sizeof(int));
				if (parent != -1)
					ifs.seekg(sizeof(int), std::ios::cur);
				ifs.seekg(sizeof(types::matrix<float>), std::ios::cur);

				std::set<std::string> tempList = skimGmObjectForTextures(ifs, type, folderType);
				strList.insert(tempList.begin(), tempList.end());

				if (type == 0 && maxFrame > 0)
				{
					int nFrame;
					ifs.read(reinterpret_cast<char*>(&nFrame), sizeof(int));
					if (nFrame)
						ifs.seekg(static_cast<long long>(tmAniSize * maxFrame), std::ios::cur);
				}

			}
		}
		return strList;
	}

	// todo: if it is packed in one, add the folder information beforehand. (isPackedInOne)
	static std::set<std::string> scanPartSfx(std::istringstream& ifs, const SFXPARTTYPE sfxpt, const SFXLOADVER ver = SFXLOADVER::load1, const bool isPackedInOne = false)
	{
		std::set<std::string> textureNames{};
		std::string strTemp;
		strTemp.resize(255);

		int nStrLen = 0;
		ifs.read(reinterpret_cast<char*>(&nStrLen), sizeof(int));
		ifs.read(&strTemp[0], static_cast<long long>(nStrLen * sizeof(char)));
		strTemp.shrink_to_fit();

		if (ver >= SFXLOADVER::load2)
		{
			ifs.read(reinterpret_cast<char*>(&nStrLen), sizeof(int));

			strTemp.clear();
			strTemp.resize(255, 0);
			ifs.read(&strTemp[0], static_cast<long long>(nStrLen * sizeof(char)));
			strTemp.shrink_to_fit();
		}

		unsigned short texFrame = 1;
		if (ver >= SFXLOADVER::load1)
		{
			ifs.read(reinterpret_cast<char*>(&texFrame), sizeof(unsigned short));
			ifs.seekg(sizeof(unsigned short), std::ios::cur); //texloop
			if (ver >= SFXLOADVER::load2)
				ifs.seekg(sizeof(int), std::ios::cur); // bvisible
		}
		ifs.seekg(sizeof(SFXPARTBILLTYPE) + sizeof(SFXPARTALPHATYPE), std::ios::cur);


		if (!strTemp.empty())
		{
			if (texFrame > 1) // ?
			{
				for (int i = 0; i < texFrame; ++i)
				{
					const auto pos = strTemp.find_last_of('.');
					const std::string ext = strTemp.substr(pos, strTemp.length());
					std::string substr = strTemp.substr(0, pos);
					if (i < 10) //replace with std::format?
						substr += "0" + std::to_string(i);
					else
						substr += std::to_string(i);

					substr += ext;
					textureNames.insert(substr);
				}
			}
			else
				textureNames.insert(strTemp);
		}


		if (sfxpt == SFXPARTTYPE_PARTICLE)
		{
			switch (ver)
			{
				case SFXLOADVER::oldLoad:
				case SFXLOADVER::load1:
					ifs.seekg(sizeof(unsigned short) * 5 + sizeof(float) * 6 + sizeof(types::vec3d<float>) * 3, std::ios::cur);
					break;
				case SFXLOADVER::load2:
					ifs.seekg(sizeof(unsigned short) * 5 + sizeof(float) * 12 + sizeof(types::vec3d<float>) * 8 + sizeof(int), std::ios::cur);
					break;
				case SFXLOADVER::load3:
					ifs.seekg(sizeof(unsigned short) * 5 + sizeof(float) * 12 + sizeof(types::vec3d<float>) * 8 + sizeof(int) * 2, std::ios::cur);
					break;
			}
		}



		int nKey = 0;
		ifs.read(reinterpret_cast<char*>(&nKey), sizeof(int));
		for (int i = 0; i < nKey; i++)
			ifs.seekg(sizeof(unsigned short) + (sizeof(types::vec3d<float>) * 4) + sizeof(int), std::ios::cur);

		if (sfxpt == SFXPARTTYPE_CUSTOMMESH)
			ifs.seekg(sizeof(int), std::ios::cur);

		return textureNames;
	}


	static std::set<std::string> scanSfxFiles(std::istringstream& ifs, bool isPackedInOne = false)
	{
		if (!ifs.good())
			return {};

		std::string szTemp;
		szTemp.resize(8);
		ifs.read(&szTemp[0], 8 * sizeof(char));

		SFXLOADVER lVer = SFXLOADVER::oldLoad;
		if (szTemp == "SFX0.1  ")
			lVer = SFXLOADVER::load1;
		else if (szTemp == "SFX0.2  ")
			lVer = SFXLOADVER::load2;
		else if (szTemp == "SFX0.3  ")
			lVer = SFXLOADVER::load3;
		else
			ifs.seekg(0, std::ios::beg);

		std::set<std::string> list;
		int nPart = 0;
		ifs.read(reinterpret_cast<char*>(&nPart), sizeof(int));
		for (int i = 0; i < nPart; i++)
		{
			SFXPARTTYPE nType = SFXPARTNONE;
			ifs.read(reinterpret_cast<char*>(&nType), sizeof(SFXPARTTYPE));

			auto results = scanPartSfx(ifs, nType, lVer, isPackedInOne);
			list.insert(results.begin(), results.end());
		}
		return list;
	}



}
