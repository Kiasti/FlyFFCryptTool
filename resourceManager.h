#pragma once
#include <string>
#include "resource.h"
#include <map>
#include <ranges>
#include <variant>
#include "fileSys.h"
#include <shared_mutex>
#include <iostream>

namespace res
{
	using vp_Flyff		=		ResourceFile<rfile::flyff::Hdr,				rfile::flyff::Res>;
	using vp_Insignia	=		ResourceFile<rFileOther::HdrInsignia,		rFileOther::ResInsignia>;
	using vp_AesGow		=		ResourceFile<rFileOther::HdrAesGOW,			rfile::flyff::Res>;
	using vp_AesAzure	=		ResourceFile<rFileOther::HdrAesAzure,		rFileOther::ResAesAzure>;
	using vp_Moon		=		ResourceFile<rFileOther::HdrMoon,			rFileOther::ResMoon>;
	using vp_Equinox	=		ResourceFile<rFileOther::HdrEquinox,		rFileOther::ResEquinox>;
	using vp_Forsaken	=		ResourceFile<rFileOther::HdrForasken,		rFileOther::ResForsaken>;
	using vp_OF			=		ResourceFile<rfile::OF::Hdr,				rfile::OF::Res>;
	using vp_NewFeiFei	=		ResourceFile<rfile::feifei::Hdr,			rfile::feifei::Res>;
	using vp_Cloud		=		ResourceFile<rFileOther::HdrCloud,			rfile::flyff::Res>;
	using vp_Insanity	=		ResourceFile<rFileOther::HdrInsanity,		rFileOther::ResInsanity>;
	using vp_Lykan =			ResourceFile<rFileOther::HdrLykan,			rFileOther::ResLykan>;

	using fs_Variant	=	std::variant<vp_Flyff, vp_Insignia, vp_AesGow, vp_AesAzure, vp_Moon, vp_Equinox, vp_Forsaken, vp_OF, vp_NewFeiFei, vp_Cloud, vp_Insanity, vp_Lykan>;

	namespace props
	{
		class propData;
	}

	class manager
	{
		using TypesMap = std::map<rfile::Type, std::string>;
		using ResourceMap = std::map<std::string, fs_Variant>;
		using ResourceMapIter = std::map<std::string, fs_Variant>::iterator;

		TypesMap resourceTypes;
		ResourceMap resourceList;
		std::shared_mutex muxResList;

		props::propData* propInfo;

		public:
			template <typename Q, typename T> std::vector<T> loadResource(Q& header)
			{				
				std::vector<T> resList;
				resList.reserve(header.fileNumber);
				std::streampos startPos = header.defaultStartPos();
				for (unsigned long i = 0; i < static_cast<unsigned long>(header.fileNumber); ++i)
					resList.emplace_back(T(header, startPos));
				return resList;
			}
			
			template<typename T> void loadPackedFile(const std::string& fileName)
			{
				static_assert(rfile::hasStartPos<typename T::first_type>::value, "Missing member function 'hasStartPos'");
				static_assert(rfile::hasFileNum<typename T::first_type>::value, "Missing member variable 'fileNumber'");
				static_assert(rfile::hasRType<typename T::first_type>::value, "Missing member functiion 'getResourceType'");
				
				if (std::ifstream ifs(fileName, std::ios::in | std::ios::binary); ifs.good())
				{
					if (const auto it = resourceList.find(fileName); it == resourceList.end())
					{
						auto first = T::first_type(ifs);

						decltype(typename T::second_type()) second{};						
						if constexpr(rfile::hasDoInHdr<typename T::first_type>::value)
							second = first.doInHdr(ifs);
						else 
							second = loadResource<typename T::first_type, typename T::second_type::value_type>(first);
						//second = loadResource<T::first_type, T::second_type::value_type>(first);
						first.cleanup();

						std::scoped_lock lck(muxResList);
						resourceList.insert(std::make_pair(fileName, std::make_pair(std::move(first), std::move(second))));
						resourceTypes.insert(std::make_pair(T::first_type::getResourceType(), fileName));
					}
					ifs.close();
				}
			}

			template<typename T> void loadPackedClient(std::string folderName, std::string ext)
			{
				static_assert(rfile::hasStartPos<typename T::first_type>::value, "Missing member function 'hasStartPos'");
				static_assert(rfile::hasFileNum<typename T::first_type>::value, "Missing member variable 'fileNumber'");
				static_assert(rfile::hasRType<typename T::first_type>::value, "Missing member functiion 'getResourceType'");

				for (const auto list = fs::getPackedFiles(std::move(folderName), std::move(ext)); auto & val : list)
				{
					std::string const str = val.string();
					loadPackedFile<T>(str);
				}
			}

			manager();

			void loadPackedDefault(std::string&& filename);
			void loadPackedDefault(const std::string& filename);
			void loadPackedAesGow(std::string&& fileName);
			void loadPackedAesGow(const std::string& fileName);
			void loadPackedAesAzure(std::string&& fileName);
			void loadPackedAesAzure(const std::string& fileName);

			void loadPackedFlorist(const std::string& fileName);
			void loadPackedOF(const std::string& fileName);

			void extractSingleFile(std::string&& fileName);
			void extractSingleFile(const std::string& fileName);
			void extractSingleFile(const std::string& archiveName, std::string&& fileName);
			void extractSingleFile(const std::string& archiveName, const std::string& fileName);

			void extractAllFromArchive(const std::string& archiveName);
			void extractAllArchives();

			void loadProject();

			std::pair<ResourceMapIter, bool> findFile(std::string&& fileName);
			std::pair<ResourceMapIter, bool> findFile(const std::string& fileName);
			std::pair<ResourceMapIter, bool> testEntropia(const std::string& fileName);
	};
}