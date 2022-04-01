#include "resourceManager.h"
#include <iostream>
#include "cryptEngine.h"
#include "resourceExtractor.h"
#include "resourceMemLoad.h"
#include "ffPropManager.h"
#include "resourceFinder.h"

res::manager::manager() : propInfo(nullptr)
{
}

void res::manager::extractAllFromArchive(const std::string& archiveName)
{
	//std::scoped_lock lck(muxResList);	
	if (const auto it = resourceList.find(archiveName); it != resourceList.end())
	{
		const extract::extractSettings temp{ "" };
		fs_Variant& ref = it->second;
		std::visit(extract::all{ archiveName, temp }, ref);
	}
}

void res::manager::extractSingleFile(std::string&& fileName)
{
	extractSingleFile(fileName);
}

void res::manager::extractSingleFile(const std::string& fileName)
{
	if (const auto [fst, snd] = findFile(fileName); snd)
		extractSingleFile(fst->first, fileName);
}

void res::manager::extractSingleFile(const std::string& archiveName, std::string&& fileName)
{
	extractSingleFile(archiveName, fileName);
}

void res::manager::extractSingleFile(const std::string& archiveName, const std::string& fileName)
{
	//std::scoped_lock lck(muxResList);
	if (const auto it = resourceList.find(archiveName); it != resourceList.end())
	{
		const extract::extractSettings temp{ "" };
		fs_Variant& ref = it->second;
		std::visit(extract::all{ archiveName, fileName, temp }, ref);
	}
}


void res::manager::loadPackedDefault(std::string&& filename)		{	loadPackedFile<vp_Flyff>(filename);		}
void res::manager::loadPackedDefault(const std::string& filename)	{	loadPackedFile<vp_Flyff>(filename);		}
void res::manager::loadPackedAesGow(std::string&& fileName)			{	loadPackedFile<vp_AesGow>(fileName);	}
void res::manager::loadPackedAesGow(const std::string& fileName)	{	loadPackedFile<vp_AesGow>(fileName);	}
void res::manager::loadPackedAesAzure(std::string&& fileName)		{	loadPackedFile<vp_AesAzure>(fileName);	}
void res::manager::loadPackedAesAzure(const std::string& fileName)	{	loadPackedFile<vp_AesAzure>(fileName);	}
void res::manager::loadPackedOF(const std::string& fileName)		{	loadPackedFile<vp_OF>(fileName);		}
void res::manager::loadPackedFlorist(const std::string& fileName)	{	loadPackedFile<vp_Insignia>(fileName);	}

void res::manager::loadProject()
{
	if (!propInfo)
		propInfo = new props::propData(this);

	propInfo->loadDefines();
	propInfo->loadStrings();
	propInfo->loadMainFiles();

	for (const std::set<std::string> fileList = propInfo->generateFileList(); const auto& iter : fileList)
	{
		if (const auto [fst, snd] = findFile(iter); snd)
			extractSingleFile(fst->first, iter);
	}
	std::cout << "completed kekw" << std::endl;
}



std::pair<res::manager::ResourceMapIter, bool> res::manager::findFile(std::string&& fileName)
{
	for (auto iter = resourceList.begin(); iter != resourceList.end(); ++iter)
	{
		if (auto res = std::visit(find::FindFile(fileName), iter->second); res)
			return std::make_pair(iter, res);
	}
	return std::make_pair(resourceList.end(), false);
}

std::pair<res::manager::ResourceMapIter, bool> res::manager::findFile(const std::string& fileName)
{
	for (auto iter = resourceList.begin(); iter != resourceList.end(); ++iter)
	{
		if (auto res = std::visit(find::FindFile(fileName), iter->second); res)
			return std::make_pair(iter, res);
	}
	return std::make_pair(resourceList.end(), false);
}

void res::manager::extractAllArchives()
{
	for (const auto& key : resourceList | std::views::keys)
	{
		auto fut = std::async(std::launch::async, &manager::extractAllFromArchive, this, key);
	}
}



std::pair<res::manager::ResourceMapIter, bool> res::manager::testEntropia(const std::string& fileName)
{
	for (auto iter = resourceList.begin(); iter != resourceList.end(); ++iter)
	{
		if (auto res = std::visit(find::findEntropia(fileName), iter->second); res)
			return std::make_pair(iter, res);
	}
	return std::make_pair(resourceList.end(), false);
}