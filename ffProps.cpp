#include "ffProps.h"
#include <ranges>
#include "ffParser.h"


ffProps::MODELELEM* ffProps::modelMng::GetModelElem(const unsigned long dwType, const unsigned long dwIndex)
{
	unsigned long idx = dwIndex;
	if (const std::vector<MODELELEM>::iterator iter = std::ranges::find_if(m_aaModelElem[dwType], [idx](const MODELELEM& ma) -> bool {
		return ma.m_dwIndex == idx;
		}); iter != m_aaModelElem[dwType].end())
		return &*iter;
	return nullptr;
}

std::string ffProps::modelMng::MakeBoneName(const unsigned long dwType, const unsigned long dwIndex)
{
	std::string wew;
	if (const MODELELEM* lpModelElem = GetModelElem(dwType, dwIndex))
	{
		wew = g_szRoot[dwType];
		wew += "_";
		wew += lpModelElem->m_szName;
		wew += ".chr";
	}
	return wew;
}

void ffProps::modelMng::LoadScript(ffTextParser::CScript&& script)
{
	unsigned int iMotion;
	script.GetToken();
	while (script.tok != ffTextParser::FINISHED)
	{
		const unsigned int iType = script.GetNumber();
		if (iType >= MAX_OBJTYPE)
			return;

		script.GetToken();
		script.GetToken();

		int nBrace = 1;
		while (nBrace)
		{
			char szObject[48];
			MODELELEM modelElem{};
			if (*script.token == '}')
			{
				nBrace--;
				if (nBrace > 0)
				{
					script.GetToken(); // object name or }
					continue;
				}
				if (nBrace == 0)
					continue;
			}
			strcpy_s(szObject, script.token);

			script.SetMark();
			script.GetToken(); // {
			if (*script.token == '{')
			{
				nBrace++;
				script.GetToken(); // object name or }
				strcpy_s(szObject, script.token); // folder ¶Ç´Â object name
				continue;
			}
			script.GoMark();

			const unsigned int iObject = script.GetNumber();
			if (iObject == 0) {}

			modelElem.m_dwType = iType;
			modelElem.m_dwIndex = iObject;

			modelElem.m_szName = szObject;
			modelElem.m_dwModelType = script.GetNumber();
			script.GetToken();

			modelElem.m_szPart = script.Token;
			modelElem.m_bFly = static_cast<unsigned char>(script.GetNumber());
			modelElem.m_dwDistant = static_cast<unsigned char>(script.GetNumber());
			modelElem.m_bPick = static_cast<unsigned char>(script.GetNumber());
			modelElem.m_fScale = script.GetFloat();
			modelElem.m_bTrans = static_cast<unsigned char>(script.GetNumber());
			modelElem.m_bShadow = static_cast<unsigned char>(script.GetNumber());
			modelElem.m_bReserved = 0;
			modelElem.m_nTextureEx = script.GetNumber();
			modelElem.m_bRenderFlag = static_cast<unsigned char>(script.GetNumber());

			script.GetToken(); // object name or { or }
			if (*script.token == '{')
			{
				char szMotion[48]{};
				script.SetMark();
				script.GetToken(); // motion name or }

				int nMax = 0;
				while (*script.token != '}')
				{
					strcpy_s(szMotion, script.token);
					iMotion = script.GetNumber();
					if (static_cast<int>(iMotion) > nMax)
						nMax = static_cast<int>(iMotion);
					script.GetToken(); // motion name or }
				}
				nMax++;
				script.GoMark();

				script.GetToken(); // motion name or }
				//modelElem.m_apszMotion = new char[static_cast<unsigned long long>(nMax * 32)];
				modelElem.m_nMax = nMax;
				//memset(modelElem.m_apszMotion, 0, sizeof(char) * nMax * 32);

				while (*script.token != '}')
				{
					strcpy_s(szMotion, script.token);

					// ReSharper disable once CppAssignedValueIsNeverUsed
					iMotion = script.GetNumber();
					modelElem.m_apszMotion.emplace_back(szMotion);
					script.GetToken(); // motion name or }
				}
				script.GetToken(); // object name or }
			}
			m_aaModelElem[iType].push_back(modelElem);
		}
		script.GetToken();
	}
}