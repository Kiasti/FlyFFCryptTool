#pragma once
#include "resourceManager.h"

namespace res::find
{
	struct FindFile
	{
		mutable std::string name;
		explicit FindFile(std::string n) : name(std::move(n)) {}
		bool operator()(const vp_Flyff& df) const;
		bool operator()(const vp_Insignia& df) const;
		bool operator()(const vp_AesGow& df) const;
		bool operator()(const vp_AesAzure& df) const;
		bool operator()(const vp_Moon& df) const;
		bool operator()(const vp_Equinox& df) const;
		bool operator()(const vp_Forsaken& df) const;
		bool operator()(const vp_NewFeiFei& df) const;
		template<typename b> bool operator()(const b&) const { return false; }
	};
}

namespace rFind = res::find;