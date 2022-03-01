#pragma once
#include "resourceManager.h"
#include <string>
#include <utility>

namespace res::mLoad
{
	struct load
	{
		mutable std::string archiveName;
		mutable std::string fileName;
		
		explicit load(std::string arch, std::string file);

		std::string operator()(const vp_Flyff& df) const;
		std::string operator()(const vp_Insignia& df) const;
		std::string operator()(const vp_AesGow& df) const;
		std::string operator()(const vp_AesAzure& df) const;
		std::string operator()(const vp_Moon& df) const;
		std::string operator()(const vp_Equinox& df) const;
		std::string operator()(const vp_Forsaken& df) const;
		std::string operator()(const vp_NewFeiFei& df) const;
		std::string operator()(const vp_Cloud& df) const;
		template<typename b> std::string operator()(const b&) const { return ""; }

	};
}
