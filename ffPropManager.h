#pragma once
#include "resourceManager.h"
#include "ffProps.h"

namespace res
{
	class manager;
	namespace props
	{
		class propData
		{
			bool isPackedInOne = true;

			std::map<unsigned long, ItemProp> itemProp;
			std::map<unsigned long, ItemProp> skillProp;
			std::map<unsigned long, MoverProp> moverProp;

			std::map<std::string, CHARACTER> m_mapCharacter;
			std::map<std::string, TERRAIN> terrain;

			std::map<unsigned long, WNDAPPLET> appletList;
			std::map<std::string, baseWorldStruct> worldList;

			modelMng mdlMng;
			manager* rmm;
			public:
				propData();
				explicit propData(manager* mngr);

				void loadDefines() const;
				void loadStrings() const;
				void spec_toBinary(int wew = 0);
				void loadMainFiles();

				void loadPropItem(ffTextParser::CScript&& scanner, propVer&& ipv = propVer::ver19);
				void loadPropMover(ffTextParser::CScript&& scanner, propVer&& ipv = propVer::ver19);
				void loadCharacterProp(ffTextParser::CScript&& script);
				void loadTerrain(ffParser::text::CScanner&& scanner);
				void loadApplet(ffParser::text::CScript&& scanner, propVer&& ipv = propVer::ver19);

				void loadPropSkill(ffParser::text::CScript&& scanner, propVer&& ipv = propVer::ver19);

				void loadWorldInsides();
				void loadWorldIncBase(ffParser::text::CScript&& scanner);
				bool loadWorldWldFile(const std::string& fileName, baseWorldStruct& baseWrld) const;

				[[nodiscard]] std::set<std::string> genModelFileNames() const;
				[[nodiscard]] std::set<std::string> genMdlTexFileNames(const std::set<std::string>& modelFiles) const;
				std::set<std::string> genWorldFileNames();

				std::set<std::string> generateFileList(); 
		};

		
	}
}
