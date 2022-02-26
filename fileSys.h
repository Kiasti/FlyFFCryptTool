#pragma once
#include <filesystem>
#include <map>
#include <ranges>

namespace res::fs
{
	using resNameToFiles = std::pair<std::string, std::vector<std::filesystem::path>>;
	using folderToResMap = std::map<std::filesystem::path, resNameToFiles>;
	
	// using class to generate based off class settings?
	// size / all in one / name / etc
	struct splitSetting
	{
		unsigned long maxSplitSize;
		bool splitName;
		bool splitSize;
		bool allInOne;
		bool allowSubFolder;
		splitSetting() : maxSplitSize(10000), splitName(false), splitSize(true), allInOne(false), allowSubFolder(true) { }
	};

	class gen
	{
			splitSetting ss;
			folderToResMap ftrm;
		public:
			gen() = default;
			explicit gen(splitSetting&& s) : ss(s) { }
			explicit gen(const splitSetting& s) : ss(s) { }

			folderToResMap makeFTRM(const std::filesystem::path& folder)
			{
				ftrm.clear();
				if (ss.allInOne)
					allinOneFTRM(folder);
				else
				{
					for (auto& path : std::filesystem::directory_iterator(folder))
					{
						if (path.is_directory())
						{
							if (ss.allowSubFolder)
							{
								auto paths2 = makeFTRM(path.path().string());
								if (ss.allInOne)
								{
									if (ftrm.contains(folder))
										for (const auto& [fst, snd] : paths2 | std::views::values)
											ftrm[folder].second.insert(ftrm[folder].second.end(), snd.begin(), snd.end());
									else
									{
										std::vector<std::filesystem::path> wew;
										wew.emplace_back(path.path());
										ftrm.insert(std::make_pair(folder, std::make_pair(folder.string(), wew)));
									}

								}
								ftrm.insert(paths2.begin(), paths2.end());
							}
							continue;
						}

						if (ftrm.contains(path.path()))
							ftrm[folder].second.emplace_back(path.path());
						else
						{
							std::vector<std::filesystem::path> wew;
							wew.emplace_back(path.path());
							ftrm.insert(std::make_pair(folder, std::make_pair(folder.string(), wew)));
						}
					}
					return ftrm;
				}
			}
			folderToResMap sizeFTRM(const std::filesystem::path& folder)
			{			
				for (auto& path : std::filesystem::directory_iterator(folder))
				{
					if (path.is_directory())
					{
						if (ss.allowSubFolder)
						{
							auto paths2 = sizeFTRM(path.path().string());
							if (ss.allInOne)
							{
								if (ftrm.contains(folder))
									for (const auto& [fst, snd] : paths2 | std::views::values)
										ftrm[folder].second.insert(ftrm[folder].second.end(), snd.begin(), snd.end());
								else
								{
									std::vector<std::filesystem::path> wew;
									wew.emplace_back(path.path());
									ftrm.insert(std::make_pair(folder, std::make_pair(folder.string(), wew)));
								}

							}
							ftrm.insert(paths2.begin(), paths2.end());
						}
						continue;
					}

					if (ftrm.contains(path.path()))
						ftrm[folder].second.emplace_back(path.path());
					else
					{
						std::vector<std::filesystem::path> wew;
						wew.emplace_back(path.path());
						ftrm.insert(std::make_pair(folder, std::make_pair(folder.string(), wew)));
					}
				}
				return ftrm;
			}
		

			void allinOneFTRM(const std::filesystem::path& folder)
			{
				for (auto& path : std::filesystem::recursive_directory_iterator(folder))
				{
					if (path.is_directory())
						continue;

					if (ftrm.contains(folder))
						ftrm[folder].second.emplace_back(path.path());
					else
					{
						std::vector<std::filesystem::path> wew;
						wew.emplace_back(path.path());
						ftrm.insert(std::make_pair(folder, std::make_pair(folder.string(), wew)));
					}
				}
			}

		//void subFolder(const std::filesystem::path& folder)
			//{
				
			//}

			folderToResMap makeFTRMAllInOne(const std::filesystem::path& folder)
			{
				ftrm.clear();
				for (auto& path : std::filesystem::directory_iterator(folder))
				{
					if (path.is_directory())
					{
						if (ss.allowSubFolder)
						{
							auto paths2 = makeFTRMAllInOne(path.path().string());
							if (ftrm.contains(folder))
								for (const auto& [fst, snd] : paths2 | std::views::values)
									ftrm[folder].second.insert(ftrm[folder].second.end(), snd.begin(), snd.end());
							else
							{
								std::vector<std::filesystem::path> wew;
								wew.emplace_back(path.path());
								ftrm.insert(std::make_pair(folder, std::make_pair(folder.string(), wew)));
							}
						}
						continue;
					}

					if (ftrm.contains(path.path()))
						ftrm[folder].second.emplace_back(path.path());
					else
					{
						std::vector<std::filesystem::path> wew;
						wew.emplace_back(path.path());
						ftrm.insert(std::make_pair(folder, std::make_pair(folder.string(), wew)));
					}
				}
				return ftrm;
			}

		
	};


	inline std::vector<std::filesystem::path> getPackedFiles(std::string&& folder, std::string&& ext)
	{
		std::vector<std::filesystem::path> tmp;
		for (auto& path : std::filesystem::recursive_directory_iterator(folder))
		{
			if (path.is_directory())
				continue;
			if (path.path().extension().string() == ext)
				tmp.emplace_back(path.path());
		}
		return tmp;
	}
	
	inline void createDirs(const std::string& fileName, const std::string& out)
	{
		if (const std::filesystem::path p = out.empty() ? fileName : out + fileName; p.has_parent_path())
			std::filesystem::create_directories(p.parent_path());
	}

	// returns a tree of
	// path : elements (folder to files) from parent folder
	// path/subpath : elements
	// path/subpath2 : elements
	// path/sub/sub2 : elements	
	
	
}
