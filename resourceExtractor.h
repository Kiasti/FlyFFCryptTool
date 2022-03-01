#pragma once
#include <string>
#include "resource.h"
#include "resourceManager.h"

namespace res::extract
{
	/**		@brief Possible struct to be used for a more settings approach when extracting contents
	* 
	*		@var outputFolder 
	*/
	struct extractSettings
	{
		std::string outputFolder{};
		bool allFiles{};
	};

	inline std::mutex outputFile;
	inline std::string folderHehe;

	struct all
	{
		extractSettings ext;
		mutable std::string archiveName;
		mutable std::string fileName;

		explicit all(std::string strRef, extractSettings ref) : ext(std::move(ref)), archiveName(std::move(strRef)) { }
		explicit all(std::string&& strRef, extractSettings ref) : ext(std::move(ref)), archiveName(strRef) { }

		explicit all(std::string strRef, std::string file, extractSettings ref) : ext(std::move(ref)), archiveName(std::move(strRef)), fileName(std::move(file)) { }
		explicit all(std::string&& strRef, std::string file, extractSettings ref) : ext(std::move(ref)), archiveName(strRef), fileName(std::move(file)) { }

		void operator()(const vp_Flyff& df) const;
		void operator()(const vp_Insignia& df) const;
		void operator()(const vp_AesGow& df) const;
		void operator()(const vp_AesAzure& df) const;
		void operator()(const vp_Moon& df) const;
		void operator()(const vp_Equinox& df) const;
		void operator()(const vp_Forsaken& df) const;
		void operator()(const vp_NewFeiFei& df) const;
		void operator()(const vp_Cloud& df) const;
		template<typename b> void operator()(const b&) const { }
	};
}
