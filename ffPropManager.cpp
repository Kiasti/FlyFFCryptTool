// ReSharper disable CppInitializedValueIsAlwaysRewritten
#include "ffPropManager.h"

#include <iostream>

#include "ffPropSkim.h"
#include "resourceMemLoad.h"
#include "paths.hpp"

res::props::propData::propData() : rmm(nullptr)
{
}
res::props::propData::propData(manager* mngr) : rmm(mngr)
{
}

void res::props::propData::loadDefines() const
{
	if (rmm == nullptr)
		return;

	for (const auto* file : defaultDefines)
	{
		if (const auto [fst, snd] = rmm->findFile(file); snd)
		{
			ffParser::text::CScript cs;
			cs.Load(std::visit(mLoad::load{ fst->first, file }, fst->second));
			cs.scanDefineFile();
		}
	}
}

void res::props::propData::loadStrings() const
{
	if (rmm == nullptr)
		return;

	for (const auto* file : defaultTxt)
	{
		if (const auto [fst, snd] = rmm->findFile(file); snd)
		{
			ffParser::text::CScript cs;
			cs.Load(std::visit(mLoad::load{ fst->first, file }, fst->second));
			cs.scanStringFiles();
		}
	}
}

void res::props::propData::spec_toBinary(const int wew)
{
	if (wew != 0)
	{
		if (const auto iter = itemProp.find(21); iter != itemProp.end())
		{
			std::ofstream ofs("peenoise.wew", std::ios::out | std::ios::binary);
			ofs.write(reinterpret_cast<char*>(&iter->second), sizeof(ItemProp));
			ofs.close();
		}
	}
}

void res::props::propData::loadMainFiles()
{
	if (rmm)
	{
		auto found = rmm->findFile("spec_item.txt");
		if (found.second)
		{
			ffParser::text::CScript cs;
			cs.Load(std::visit(mLoad::load{ found.first->first, "spec_item.txt" }, found.first->second));
			loadPropItem(std::move(cs));
		}

		found = rmm->findFile("mdlobj.inc");
		if (found.second)
		{
			ffParser::text::CScript cs;
			cs.Load(std::visit(mLoad::load{ found.first->first, "mdlobj.inc" }, found.first->second));
			mdlMng.LoadScript(std::move(cs));
		}

		found = rmm->findFile("mdldyna.inc");
		if (found.second)
		{
			ffParser::text::CScript cs;
			cs.Load(std::visit(mLoad::load{ found.first->first, "mdldyna.inc" }, found.first->second));
			mdlMng.LoadScript(std::move(cs));
		}

		//todo: fix .txt.txt files
		found = rmm->findFile("character.inc");
		if (found.second)
		{
			ffParser::text::CScript cs;
			cs.Load(std::visit(mLoad::load{ found.first->first, "character.inc" }, found.first->second));
			loadCharacterProp(std::move(cs));
		}

		found = rmm->findFile("terrain.inc");
		if (found.second)
		{
			ffParser::text::CScript cs;
			cs.Load(std::visit(mLoad::load{ found.first->first, "terrain.inc" }, found.first->second));
			loadTerrain(std::move(cs));
		}


		loadWorldInsides();
	}
}


void res::props::propData::loadPropItem(ffTextParser::CScript&& scanner, propVer&& ipv)
{
	ItemProp prop;
	int nVer = scanner.GetNumber();
	itemProp.clear();

	while (scanner.tok != ffTextParser::FINISHED)
	{
		unsigned long dwID = scanner.GetNumber();
		prop.nVer = nVer;
		prop.dwID = dwID;
		scanner.GetToken();

		strcpy(prop.szName, scanner.token);

		prop.dwNum = scanner.GetNumber();
		prop.dwPackMax = scanner.GetNumber();
		prop.dwItemKind1 = scanner.GetNumber();
		prop.dwItemKind2 = scanner.GetNumber();
		prop.dwItemKind3 = scanner.GetNumber();
		prop.dwItemJob = scanner.GetNumber();
		prop.bPermanence = scanner.GetNumber();
		prop.dwUseable = scanner.GetNumber();
		prop.dwItemSex = scanner.GetNumber();
		prop.dwCost = scanner.GetNumber();
		prop.dwEndurance = scanner.GetNumber();
		prop.nAbrasion = scanner.GetNumber();
		prop.nMaxRepair = scanner.GetNumber();
		prop.dwHanded = scanner.GetNumber();
		prop.dwFlag = scanner.GetNumber();
		prop.dwParts = scanner.GetNumber();
		prop.dwPartsub = scanner.GetNumber();
		prop.bPartsFile = scanner.GetNumber();
		prop.dwExclusive = scanner.GetNumber();
		prop.dwBasePartsIgnore = scanner.GetNumber();
		prop.dwItemLV = scanner.GetNumber();
		prop.dwItemRare = scanner.GetNumber();
		prop.dwShopAble = scanner.GetNumber();
		prop.nLog = scanner.GetNumber();
		prop.bCharged = scanner.GetNumber();
		prop.bCharged = (prop.bCharged != 1 ? 0 : 1);
		prop.dwLinkKindBullet = scanner.GetNumber();
		prop.dwLinkKind = scanner.GetNumber();
		prop.dwAbilityMin = scanner.GetNumber();

		prop.dwAbilityMax = scanner.GetNumber();
		prop.eItemType =  scanner.GetNumber();
		prop.wItemEatk = static_cast<short>(scanner.GetNumber());
		prop.dwParry = scanner.GetNumber();
		prop.dwblockRating = scanner.GetNumber();
		prop.nAddSkillMin = scanner.GetNumber();
		prop.nAddSkillMax = scanner.GetNumber();
		prop.dwAtkStyle = scanner.GetNumber();
		prop.dwWeaponType = scanner.GetNumber();
		prop.dwItemAtkOrder1 = scanner.GetNumber();
		prop.dwItemAtkOrder2 = scanner.GetNumber();
		prop.dwItemAtkOrder3 = scanner.GetNumber();
		prop.dwItemAtkOrder4 = scanner.GetNumber();
		prop.tmContinuousPain = scanner.GetNumber();
		prop.nShellQuantity = scanner.GetNumber();
		prop.dwRecoil = scanner.GetNumber();
		prop.dwLoadingTime = scanner.GetNumber();
		prop.nAdjHitRate = scanner.GetNumber();
		prop.fAttackSpeed = scanner.GetFloat();
		prop.dwDmgShift = scanner.GetNumber();
		prop.dwAttackRange = scanner.GetNumber();
		prop.nProbability = scanner.GetNumber();
		prop.dwDestParam[0] = scanner.GetNumber();
		prop.dwDestParam[1] = scanner.GetNumber();
		prop.dwDestParam[2] = scanner.GetNumber();
		//19
		if (ipv >= propVer::ver19)
		{
			prop.dwDestParam[3] = scanner.GetNumber();
			prop.dwDestParam[4] = scanner.GetNumber();
			prop.dwDestParam[5] = scanner.GetNumber();
		}

		prop.nAdjParamVal[0] = scanner.GetNumber();
		prop.nAdjParamVal[1] = scanner.GetNumber();
		prop.nAdjParamVal[2] = scanner.GetNumber();
		if (ipv >= propVer::ver19)
		{
			prop.nAdjParamVal[3] = scanner.GetNumber();
			prop.nAdjParamVal[4] = scanner.GetNumber();
			prop.nAdjParamVal[5] = scanner.GetNumber();
		}

		//19
		prop.dwChgParamVal[0] = scanner.GetNumber();
		prop.dwChgParamVal[1] = scanner.GetNumber();
		prop.dwChgParamVal[2] = scanner.GetNumber();
		if (ipv >= propVer::ver19)
		{
			prop.dwChgParamVal[3] = scanner.GetNumber();
			prop.dwChgParamVal[4] = scanner.GetNumber();
			prop.dwChgParamVal[5] = scanner.GetNumber();
		}

		//19
		prop.nDestData1[0] = scanner.GetNumber();
		prop.nDestData1[1] = scanner.GetNumber();
		prop.nDestData1[2] = scanner.GetNumber();
		//19

		prop.dwActiveSkill = scanner.GetNumber();
		prop.dwActiveSkillLv = scanner.GetNumber();
		prop.dwActiveSkillRate = scanner.GetNumber();
		prop.dwReqMp = scanner.GetNumber();
		prop.dwReqFp = scanner.GetNumber();
		prop.dwReqDisLV = scanner.GetNumber();
		prop.dwReSkill1 = scanner.GetNumber();
		prop.dwReSkillLevel1 = scanner.GetNumber();
		prop.dwReSkill2 = scanner.GetNumber();
		prop.dwReSkillLevel2 = scanner.GetNumber();
		prop.dwSkillReadyType = scanner.GetNumber();
		prop.dwSkillReady = scanner.GetNumber();
		prop._dwSkillRange = scanner.GetNumber();
		prop.dwSfxElemental = scanner.GetNumber();
		prop.dwSfxObj = scanner.GetNumber();
		prop.dwSfxObj2 = scanner.GetNumber();
		prop.dwSfxObj3 = scanner.GetNumber();
		prop.dwSfxObj4 = scanner.GetNumber();
		prop.dwSfxObj5 = scanner.GetNumber();
		prop.dwUseMotion = scanner.GetNumber();
		prop.dwCircleTime = scanner.GetNumber();
		prop.dwSkillTime = scanner.GetNumber();
		prop.dwExeTarget = scanner.GetNumber();
		prop.dwUseChance = scanner.GetNumber();
		prop.dwSpellRegion = scanner.GetNumber();
		prop.dwSpellType = scanner.GetNumber();
		prop.dwReferStat1 = scanner.GetNumber();
		prop.dwReferStat2 = scanner.GetNumber();
		prop.dwReferTarget1 = scanner.GetNumber();
		prop.dwReferTarget2 = scanner.GetNumber();
		prop.dwReferValue1 = scanner.GetNumber();
		prop.dwReferValue2 = scanner.GetNumber();
		prop.dwSkillType = scanner.GetNumber();
		prop.nItemResistElecricity = static_cast<int>(scanner.GetFloat() * 100.0f);
		prop.nItemResistFire = static_cast<int>(scanner.GetFloat() * 100.0f);
		prop.nItemResistWind = static_cast<int>(scanner.GetFloat() * 100.0f);
		prop.nItemResistWater = static_cast<int>(scanner.GetFloat() * 100.0f);
		prop.nItemResistEarth = static_cast<int>(scanner.GetFloat() * 100.0f);
		prop.nEvildoing = scanner.GetNumber();
		prop.dwExpertLV = scanner.GetNumber();
		prop.dwExpertMax = scanner.GetNumber();
		prop.dwSubDefine = scanner.GetNumber();
		prop.dwExp = scanner.GetNumber();
		prop.dwComboStyle = scanner.GetNumber();
		prop.fFlightSpeed = scanner.GetFloat();
		prop.fFlightLRAngle = scanner.GetFloat();
		prop.fFlightTBAngle = scanner.GetFloat();
		prop.dwFlightLimit = scanner.GetNumber();
		prop.dwFFuelReMax = scanner.GetNumber();
		prop.dwAFuelReMax = scanner.GetNumber();
		prop.dwFuelRe = scanner.GetNumber();
		prop.dwLimitLevel1 = scanner.GetNumber();
		prop.nReflect = scanner.GetNumber();
		prop.dwSndAttack1 = scanner.GetNumber();
		prop.dwSndAttack2 = scanner.GetNumber();


		scanner.GetToken();
		scanner.GetToken();
		strncpy(prop.szIcon, scanner.token, sizeof(prop.szIcon) - 1);
		prop.szCommand[sizeof(prop.szIcon) - 1] = 0;

		scanner.GetToken();

		prop.dwQuestId = scanner.GetNumber();

		scanner.GetToken();
		scanner.GetToken();
		strcpy(prop.szTextFileName, scanner.token);
		scanner.GetToken();

		scanner.GetToken();
		strncpy(prop.szCommand, scanner.token, sizeof(prop.szCommand) - 1);
		prop.szCommand[sizeof(prop.szCommand) - 1] = 0;

		//v16
		if (ipv >= propVer::ver16)
		{
			prop.nMinLimitLevel = scanner.GetNumber();	//min level requirement
			prop.nMaxLimitLevel = scanner.GetNumber();	//Maximum level limit
			prop.nItemGroup = scanner.GetNumber();	//Item group? 12 = ticket (All source based)
			prop.nUseLimitGroup = scanner.GetNumber();	//Maximum use group: Amp = 2, gear = 0, scrolls(pgrade) = 1, 13 = ticket?
			prop.nMaxDuplication = scanner.GetNumber();	//Maximum use at once Ie: amps = 5 (source based ^ < )
			prop.nEffectValue = scanner.GetNumber();	//Multiple things
			prop.nTargetMinEnchant = scanner.GetNumber();	//Min req for upgrade material
			prop.nTargetMaxEnchant = scanner.GetNumber();	//Max effective an upgrade material is
			prop.bResetBind = scanner.GetNumber();	//1 = Can be reset. 0 = cant
			prop.nBindCondition = scanner.GetNumber();	//how item is bound. 0 doesnt bind. 1 binds on get 2= on equip
			prop.nResetBindCondition = scanner.GetNumber();	//?
			prop.dwHitActiveSkillId = scanner.GetNumber();	//Baruna rune id on hit
			prop.dwHitActiveSkillLv = scanner.GetNumber();	//Baruna Rune skilllv
			prop.dwHitActiveSkillProb = scanner.GetNumber();	//Baruna rune prob
			prop.dwHitActiveSkillTarget = scanner.GetNumber();	//baruna rune target
			prop.dwDamageActiveSkillId = scanner.GetNumber();	//Ondamage?
			prop.dwDamageActiveSkillLv = scanner.GetNumber();
			prop.dwDamageActiveSkillProb = scanner.GetNumber();
			prop.dwDamageActiveSkillTarget = scanner.GetNumber();
			prop.dwEquipActiveSkillId = scanner.GetNumber();	//While equiped?
			prop.dwEquipActiveSkillLv = scanner.GetNumber();
			prop.dwSmelting = scanner.GetNumber();	//Cannot upgrade items that last only X long
			prop.dwAttsmelting = scanner.GetNumber();	//Cannot upgrade items that last only X long
			prop.dwGemsmelting = scanner.GetNumber();	//Cannot upgrade items that last only X long
			prop.dwPierce = scanner.GetNumber();	//Cannot upgrade items that last only X long
			prop.dwUprouse = scanner.GetNumber();	//Cannot upgrade items that last only X long
			prop.bAbsoluteTime = scanner.GetNumber();	//if 1 = Time counts down while offline.
		}
		itemProp.insert(std::make_pair(prop.dwID, prop));

		nVer = scanner.GetNumber();	// version;
	}
}


void res::props::propData::loadPropMover(ffTextParser::CScript&& scanner, propVer&& ipv)
{
	std::set<int> checker;

	char szLastName[256];
	int i = 0;
	memset(szLastName, 0, sizeof(szLastName));
	moverProp.clear();

	while (true)
	{
		i = scanner.GetNumber();
		if (scanner.tok == ffTextParser::FINISHED)
			break;

		if (i == 0)
			continue;

		if (!checker.insert(i).second)
			return;

		MoverProp pProperty;
		pProperty.dwID = i;

		scanner.GetToken();
		strcpy(szLastName, scanner.token);
		strcpy(pProperty.szName, scanner.token);


		pProperty.dwAI = scanner.GetNumber();
		pProperty.dwStr = scanner.GetNumber();
		pProperty.dwSta = scanner.GetNumber();
		pProperty.dwDex = scanner.GetNumber();
		pProperty.dwInt = scanner.GetNumber();
		pProperty.dwHR = scanner.GetNumber();
		pProperty.dwER = scanner.GetNumber();
		pProperty.dwRace = scanner.GetNumber();
		pProperty.dwBelligerence = scanner.GetNumber();
		pProperty.dwGender = scanner.GetNumber();
		pProperty.dwLevel = scanner.GetNumber();
		pProperty.dwFlightLevel = scanner.GetNumber();
		pProperty.dwSize = scanner.GetNumber();
		pProperty.dwClass = scanner.GetNumber();
		pProperty.bIfParts = scanner.GetNumber();

		pProperty.nChaotic = scanner.GetNumber();
		pProperty.dwUseable = scanner.GetNumber();
		pProperty.dwActionRadius = scanner.GetNumber();
		pProperty.dwAtkMin = scanner.GetNumber();
		pProperty.dwAtkMax = scanner.GetNumber();
		pProperty.dwAtk1 = scanner.GetNumber();
		pProperty.dwAtk2 = scanner.GetNumber();
		pProperty.dwAtk3 = scanner.GetNumber();
		pProperty.dwAtk4 = scanner.GetNumber();
		pProperty.fFrame = scanner.GetFloat();
		if (abs(-1.0F - pProperty.fFrame) < 0.000001F)
			pProperty.fFrame = 1.0F;

		pProperty.dwOrthograde = scanner.GetNumber();
		pProperty.dwThrustRate = scanner.GetNumber();

		pProperty.dwChestRate = scanner.GetNumber();
		pProperty.dwHeadRate = scanner.GetNumber();
		pProperty.dwArmRate = scanner.GetNumber();
		pProperty.dwLegRate = scanner.GetNumber();

		pProperty.dwAttackSpeed = scanner.GetNumber();
		pProperty.dwReAttackDelay = scanner.GetNumber();
		pProperty.dwAddHp = scanner.GetNumber();
		pProperty.dwAddMp = scanner.GetNumber();
		pProperty.dwNaturalArmor = scanner.GetNumber();
		pProperty.nAbrasion = scanner.GetNumber();
		pProperty.nHardness = scanner.GetNumber();
		pProperty.dwAdjAtkDelay = scanner.GetNumber();

		pProperty.eElementType = scanner.GetNumber();
		pProperty.wElementAtk = static_cast<short>(scanner.GetNumber());

		pProperty.dwHideLevel = scanner.GetNumber();
		pProperty.fSpeed = scanner.GetFloat();
		pProperty.dwShelter = scanner.GetNumber();
		pProperty.dwFlying = scanner.GetNumber();
		pProperty.dwJumpIng = scanner.GetNumber();
		pProperty.dwAirJump = scanner.GetNumber();
		pProperty.bTaming = scanner.GetNumber();
		pProperty.dwResisMgic = scanner.GetNumber();

		pProperty.nResistElecricity = static_cast<int>(scanner.GetFloat() * 100.0f);
		pProperty.nResistFire = static_cast<int>(scanner.GetFloat() * 100.0f);
		pProperty.nResistWind = static_cast<int>(scanner.GetFloat() * 100.0f);
		pProperty.nResistWater = static_cast<int>(scanner.GetFloat() * 100.0f);
		pProperty.nResistEarth = static_cast<int>(scanner.GetFloat() * 100.0f);

		pProperty.dwCash = scanner.GetNumber();
		pProperty.dwSourceMaterial = scanner.GetNumber();
		pProperty.dwMaterialAmount = scanner.GetNumber();
		pProperty.dwCohesion = scanner.GetNumber();
		pProperty.dwHoldingTime = scanner.GetNumber();
		pProperty.dwCorrectionValue = scanner.GetNumber();
		pProperty.nExpValue = scanner.GetExpInteger();
		pProperty.nFxpValue = scanner.GetNumber();
		pProperty.nBodyState = scanner.GetNumber();
		pProperty.dwAddAbility = scanner.GetNumber();
		pProperty.bKillable = scanner.GetNumber();

		pProperty.dwVirtItem[0] = scanner.GetNumber();
		pProperty.dwVirtItem[1] = scanner.GetNumber();
		pProperty.dwVirtItem[2] = scanner.GetNumber();
		pProperty.bVirtType[0] = scanner.GetNumber();
		pProperty.bVirtType[1] = scanner.GetNumber();
		pProperty.bVirtType[2] = scanner.GetNumber();

		pProperty.dwSndAtk1 = scanner.GetNumber();
		pProperty.dwSndAtk2 = scanner.GetNumber();

		pProperty.dwSndDie1 = scanner.GetNumber();
		pProperty.dwSndDie2 = scanner.GetNumber();

		pProperty.dwSndDmg1 = scanner.GetNumber();
		pProperty.dwSndDmg2 = scanner.GetNumber();
		pProperty.dwSndDmg3 = scanner.GetNumber();

		pProperty.dwSndIdle1 = scanner.GetNumber();
		pProperty.dwSndIdle2 = scanner.GetNumber();
		scanner.GetToken();

		if (ipv == propVer::ver19)
		{
			//todo: v19 params
		}


		pProperty.m_fHitPoint_Rate = 1.0f;
		pProperty.m_fAttackPower_Rate = 1.0f;
		pProperty.m_fDefence_Rate = 1.0f;
		pProperty.m_fExp_Rate = 1.0f;
		pProperty.m_fItemDrop_Rate = 1.0f;
		pProperty.m_fPenya_Rate = 1.0f;
		pProperty.m_bRate = false;

		moverProp.insert(std::make_pair(i, pProperty));
	}
}

void res::props::propData::loadTerrain(ffTextParser::CScanner&& scanner)
{
	int nBrace = 1;
	scanner.SetMark();
	terrain.clear();

	int i = scanner.GetNumber(); // folder or id
	int ImageCnt = 0;
	int IdCnt = 0;

	int FrameCnt = scanner.GetNumber();
	while (nBrace)
	{
		if (*scanner.token == '}' || scanner.tok == ffTextParser::FINISHED)
		{
			nBrace--;
			if (nBrace > 0)
			{
				scanner.SetMark();
				i = scanner.GetNumber();

				FrameCnt = scanner.GetNumber();
				IdCnt = 0;
				if (nBrace == 1 && FrameCnt)
					ImageCnt = 0;
				continue;
			}
		}
		if (nBrace == 0)
			continue;

		scanner.GetToken();

		if (*scanner.token == '{')
		{
			nBrace++;
			scanner.SetMark();
			i = scanner.GetNumber(); // id
			FrameCnt = scanner.GetNumber();	//	Count
			continue;
		}

		scanner.GoMark();
		i = scanner.GetNumber(); // id
		scanner.GetNumber();	//	Count

		TERRAIN lpTerrain{};
		lpTerrain.m_dwId = i;
		scanner.GetToken();
		strcpy(lpTerrain.m_szTextureFileName, scanner.token);
		lpTerrain.m_bBlock = scanner.GetNumber();				 // block
		scanner.GetToken();										// sound
		strcpy(lpTerrain.m_szSoundFileName, scanner.token);

		terrain.insert(std::make_pair(lpTerrain.m_szTextureFileName, lpTerrain));

		scanner.SetMark();
		i = scanner.GetNumber();  // texture fileName
	}
}


std::string GetLangScript(ffParser::text::CScript& script)
{
	script.GetToken();
	std::string str = script.Token;
	script.GetToken();	// )
	script.GetToken(); // ;
	if (str.empty())
		str = " ";
	return str;
}


void res::props::propData::loadCharacterProp(ffTextParser::CScript&& script)
{
	script.GetToken(); // Mover name

	std::string strName = script.Token;
	while( script.tok != ffTextParser::FINISHED)
	{
		CHARACTER lpCharacter;
		strcpy(lpCharacter.m_szKey, strName.c_str());
		lpCharacter.m_nVenderType = 0;
		lpCharacter.bOutput = true;

		script.GetToken(); // {

		int nBlock = 1;
		while( nBlock && script.tok != ffTextParser::FINISHED )
		{
			script.GetToken();
			if( script.Token == "{" )
				nBlock++;
			else if( script.Token == "}" )
				nBlock--;
			else if (script.Token == "randomItem")
			{
				script.GetToken(); // {
				script.GetToken();
				while (*script.token != '}')
				{
					if (script.Token == ";")
					{
						script.GetToken();
						continue;
					}
					script.GetToken();
				}
			}
			else if(script.Token == "SetEquip")
			{
				lpCharacter.m_nEquipNum = 0;
				script.GetToken(); // (
				while( *script.token != ')' )
				{
					const DWORD dwEquip = script.GetNumber();
					if( lpCharacter.m_nEquipNum < MAX_HUMAN_PARTS )
						lpCharacter.m_adwEquip[ lpCharacter.m_nEquipNum++ ] = dwEquip;
					script.GetToken(); // ,
				}
			}
			else if(script.Token == "m_szName")
			{
				script.GetToken(); // =
				script.GetToken(); // name
				lpCharacter.m_strName = script.Token;
			}
			else if(script.Token == "SetName")
			{
				script.GetToken(); // (
				lpCharacter.m_strName = GetLangScript( script );
			}
			else if( script.Token == "SetFigure" )
			{
				script.GetToken(); // (
				lpCharacter.m_dwMoverIdx = script.GetNumber();
				script.GetToken(); // ,
				lpCharacter.m_dwHairMesh = script.GetNumber();
				script.GetToken(); // ,
				lpCharacter.m_dwHairColor = script.GetNumber();
				script.GetToken(); // ,
				lpCharacter.m_dwHeadMesh = script.GetNumber();
			}
			else if( script.Token == "SetMusic" )
			{
				script.GetToken(); // (
				lpCharacter.m_dwMusicId = script.GetNumber();
			}
			else if(script.Token == "m_nStructure")
			{
				script.GetToken();
				lpCharacter.m_nStructure = script.GetNumber();
			}
			else if(script.Token == "m_szChar")
			{
				script.GetToken();
				std::string filename = GetLangScript( script );
				strcpy( lpCharacter.m_szChar, filename.data() );
			}
			else if(script.Token == "m_szDialog")
			{
				script.GetToken();
				script.GetToken();
				strcpy( lpCharacter.m_szDialog, script.Token.data() );
			}
			else if(script.Token == "m_szDlgQuest")
			{
				script.GetToken();
				script.GetToken();
				strcpy( lpCharacter.m_szDlgQuest, script.Token.data() );
			}
			else if(script.Token == "SetImage" )
			{
				script.GetToken();
				std::string filename = GetLangScript( script );
				strcpy( lpCharacter.m_szChar, filename.data() );
			}
			else if( script.Token == "AddMenuLang" )
			{
				script.GetToken(); // (
				script.GetNumber(); //nLang 
				script.GetToken(); // ,

				//int nSubLang	= script.GetNumber();
				//script.GetToken(); // ,

				script.GetNumber(); // mmi
				script.GetToken(); // )
			}

			else if(script.Token == "AddMenu2"  || script.Token == "AddMenuEx")
			{
				script.GetToken(); // (
				const int nMMI = script.GetNumber();
				script.GetToken(); //text name
				std::string menutext = script.Token;
				script.GetToken(); // )

				lpCharacter.m_abMoverMenu[ nMMI ] = true;
				lpCharacter.m_abMoverMenu2[ nMMI ] = true;
				lpCharacter.m_abMoverMenuText[ nMMI ] = menutext;
			}
			else if(script.Token == "AddMenu" )
			{
				script.GetToken(); // (
				int nMMI = script.GetNumber();
				script.GetToken(); // )
				lpCharacter.m_abMoverMenu[ nMMI ] = true;
			}
			else if(script.Token == "AddVenderSlot" ) // "AddVendorSlot"랑 같다 삭제 예정
			{
				script.GetToken(); // (
				script.GetNumber(); // nslot 
				script.GetToken(); //
				script.GetToken();
				//lpCharacter.m_venderSlot[ nSlot ] = script.token;
				script.GetToken(); //
			}
			else if(script.Token == "AddVendorSlot" )
			{
				script.GetToken(); // (
				script.GetNumber(); // slot
				script.GetToken(); // ,
				//lpCharacter.m_venderSlot[ nSlot ] = GetLangScript( script );
			}
			else if( script.Token == "AddVendorSlotLang" )
			{
				script.GetToken(); // (
				script.GetNumber();	// lang
				script.GetToken();	//,
				script.GetNumber(); // sublang
				script.GetToken(); // ,
				script.GetNumber(); // slot
				script.GetToken(); // ,
				std::string strSlot	= GetLangScript( script );
				//lpCharacter.m_venderSlot[ nSlot ] = strSlot;
			}
			else if( script.Token == "AddVendorItemLang" )
			{
				script.GetToken();							// (
				script.GetNumber();		script.GetToken();	// lang ,				
				script.GetNumber();		script.GetToken();  // sublang ,				
				script.GetNumber();		script.GetToken();	// slot , 
				script.GetNumber();		script.GetToken();	// ik3 ,
				script.GetNumber();		script.GetToken();	// job ,
				script.GetNumber();		script.GetToken();	// uniquemin ,
				script.GetNumber();		script.GetToken();	// unique max ,
				script.GetNumber();		script.GetToken();	// total num )

				//LPVENDOR_ITEM pVendorItem	= new VENDOR_ITEM;
				//pVendorItem->m_nItemkind3	= nItemKind3;
				//pVendorItem->m_nItemJob		= nItemJob;
				//pVendorItem->m_nUniqueMin	= nUniqueMin;
				//pVendorItem->m_nUniqueMax	= nUniqueMax;
				//pVendorItem->m_nTotalNum	= nTotalNum;
				//lpCharacter.m_venderItemAry[ nSlot ].Add( pVendorItem );
			}
			else if( script.Token == "AddVenderItem" || script.Token == "AddVendorItem")
			{
				script.GetToken();							// (
				script.GetNumber();		script.GetToken();  // slot ,
				script.GetNumber();		script.GetToken();	// ik3 ,
				script.GetNumber();		script.GetToken();	// job ,
				script.GetNumber();		script.GetToken();	// unique min, 
				script.GetNumber();		script.GetToken();	// unique max,
				script.GetNumber();		script.GetToken();	// total num )

				//LPVENDOR_ITEM pVendorItem = new VENDOR_ITEM;
				//pVendorItem->m_nItemkind3  = nItemKind3;
				//pVendorItem->m_nItemJob	= nItemJob;
				//pVendorItem->m_nUniqueMin = nUniqueMin;
				//pVendorItem->m_nUniqueMax = nUniqueMax;
				//pVendorItem->m_nTotalNum  = nTotalNum;
				//lpCharacter.m_venderItemAry[ nSlot ].Add( pVendorItem );
			}
			else if( script.Token == "AddVenderItem2" || script.Token == "AddVendorItem2")
			{
				script.GetToken(); // (
				script.GetNumber(); script.GetToken(); // slot ,
				script.GetNumber(); script.GetToken(); // id )

				//LPVENDOR_ITEM pVendorItem = new VENDOR_ITEM;
				//pVendorItem->m_dwItemId = dwId;
				//lpCharacter.m_venderItemAry2[ nSlot ].Add( pVendorItem );
			}
			else if( script.Token == "SetVenderType" )
			{
				script.GetToken();
				const int nVenderType = script.GetNumber();
				lpCharacter.m_nVenderType = nVenderType;
				script.GetToken();
			}
			else if( script.Token == "SetBuffSkill" )
			{
				NPC_BUFF_SKILL NPCBuffSkill{};
				script.GetToken(); // (
				NPCBuffSkill.dwSkillID = script.GetNumber(); script.GetToken(); //,
				NPCBuffSkill.dwSkillLV = script.GetNumber(); script.GetToken(); //,
				NPCBuffSkill.nMinPlayerLV = script.GetNumber(); script.GetToken(); //,
				NPCBuffSkill.nMaxPlayerLV = script.GetNumber(); script.GetToken(); //,
				NPCBuffSkill.dwSkillTime = script.GetNumber();
				script.GetToken(); // )
				lpCharacter.m_vecNPCBuffSkill.push_back( NPCBuffSkill );
			}
			else if( script.Token == "SetLang" )
			{
				script.GetToken();		// (
				script.GetNumber();		// lang
				
				script.GetToken();		// , or )
				if( script.Token == "," )
					script.GetNumber(); // sublang
				else
					script.GetToken();	// )
				//lpCharacter.m_vecdwLanguage.push_back( MAKELONG( wSubLang, wLang ) );
			}
			else if( script.Token == "SetOutput" )
			{
				script.GetToken(); // (
				script.GetToken();
				if( script.Token == "FALSE" )
					lpCharacter.bOutput = false;
				script.GetToken(); // )
			}
			else if (script.Token == "AddTeleport")
			{
				script.GetToken(); // (
				types::vec3d<float> vPos;
				vPos.x = script.GetFloat();
				script.GetToken(); // ,
				vPos.z = script.GetFloat();
				script.GetToken(); // )
				lpCharacter.m_vecTeleportPos.push_back(vPos);
			}
			else if( script.Token == "AddShopItem" )
			{
				script.GetToken();
				script.GetNumber(); script.GetToken();	// slot ,
				script.GetNumber(); script.GetToken();	// id , 

				if( script.Token == "," )
				{
					if(DWORD dwItemCost = script.GetNumber(); dwItemCost && dwItemCost < 1 )
						dwItemCost = 1;

					//ItemProp* pItemProp = prj.GetItemProp( dwId );
					//if( pItemProp )
					//{
					//	if( lpCharacter.m_nVenderType != 0 )
					//		pItemProp->dwReferValue1 = dwItemCost;
					//	else
					//		pItemProp->dwCost = dwItemCost;
					//}
				}
				//LPVENDOR_ITEM pVendorItem = new VENDOR_ITEM;
				//pVendorItem->m_dwItemId = dwId;
				//lpCharacter.m_venderItemAry3[ nSlot ].Add( pVendorItem );
			}
			else if (script.Token == "AddItemEx")
			{
				script.GetToken();
				script.GetToken();
				script.GetToken();
				script.GetToken();
				script.GetToken();
				script.GetToken();
				script.GetToken();
			}
			else if (script.Token == "SetCurrency")
			{
				script.GetToken();
				script.GetToken();
				script.GetToken();
			}
		}
		m_mapCharacter.insert(std::make_pair(lpCharacter.m_szKey, std::move(lpCharacter)));
		script.GetToken(); // Mover name
		strName = script.Token;
	}
}



std::string GetLangApplet(ffParser::text::CScript& scanner)
{

	scanner.GetToken();	// {
	scanner.GetToken();
	std::string str = scanner.token;
	scanner.GetToken();	// }
	return str;
}

std::string GetLangCtrl(ffParser::text::CScript& scanner)
{
	scanner.GetToken();	// {
	scanner.GetToken();
	std::string str = scanner.token;
	scanner.GetToken();	// }

	return str;
}

void res::props::propData::loadApplet(ffTextParser::CScript&& scanner, propVer&& ipv)
{
	scanner.GetToken_NoDef();

	while (scanner.tok != ffTextParser::FINISHED)
	{
		WNDAPPLET pWndApplet;
		pWndApplet.strDefine = scanner.token;
		pWndApplet.dwWndId = ffTextParser::CScript::GetDefineNum(scanner.token);

		scanner.GetToken();
		pWndApplet.strTexture = scanner.token;

		scanner.GetToken();

		pWndApplet.bTile = scanner.GetNumber();
		pWndApplet.size.x = scanner.GetNumber();
		pWndApplet.size.y = scanner.GetNumber();
		pWndApplet.dwWndStyle = scanner.GetNumber();
		pWndApplet.d3dFormat = scanner.GetNumber();

		pWndApplet.strTitle = GetLangApplet(scanner);
		pWndApplet.strToolTip = GetLangApplet(scanner);

		scanner.GetToken(); // skip {
		DWORD dwWndType = scanner.GetNumber();
		while (*scanner.token != '}')
		{
			WNDCTRL pWndCtrl;
			pWndCtrl.dwWndType = dwWndType;

			scanner.GetToken_NoDef();
			pWndCtrl.strDefine = scanner.token;///Char;

			pWndCtrl.dwWndId = ffTextParser::CScript::GetDefineNum(scanner.token);
			scanner.GetToken();
			pWndCtrl.strTexture = scanner.token;//token;
			pWndCtrl.bTile = scanner.GetNumber();
			pWndCtrl.rect.left() = scanner.GetNumber();
			pWndCtrl.rect.top() = scanner.GetNumber();
			pWndCtrl.rect.right() = scanner.GetNumber();
			pWndCtrl.rect.bottom() = scanner.GetNumber();
			pWndCtrl.dwWndStyle = scanner.GetNumber();

			pWndCtrl.m_bVisible = scanner.GetNumber();
			pWndCtrl.m_bGroup = scanner.GetNumber();
			pWndCtrl.m_bDisabled = scanner.GetNumber();
			pWndCtrl.m_bTabStop = scanner.GetNumber();

			//propver
			if (ipv >= propVer::ver16)
			{
				scanner.GetNumber();
				scanner.GetNumber();
				scanner.GetNumber();
			}

			pWndCtrl.strTitle = GetLangCtrl(scanner);
			pWndCtrl.strToolTip = GetLangCtrl(scanner);

			pWndApplet.ptrCtrlArray.push_back(std::move(pWndCtrl));
			dwWndType = scanner.GetNumber();
		}
		appletList.insert(std::make_pair(pWndApplet.dwWndId, pWndApplet));
		scanner.GetToken_NoDef();
	}
}

void res::props::propData::loadWorldIncBase(ffTextParser::CScript&& scanner)
{
	int m_nSize = 0;
	int nBrace = 1;
	scanner.SetMark();

	int i = scanner.GetNumber();
	while (nBrace)
	{
		if (*scanner.token == '}' || scanner.tok == ffTextParser::FINISHED)
		{
			nBrace--;
			if (nBrace > 0)
			{
				scanner.SetMark();
				i = scanner.GetNumber();	// folder or id
				if (i > m_nSize)
					m_nSize = i;
				continue;
			}
			if (nBrace == 0)
				continue;
		}
		scanner.GetToken();

		if (scanner.Token == "SetTitle")
		{
			scanner.GetToken(); // (
			std::string str = GetLangScript(scanner);
		}
		else
		{
			scanner.GoMark();
			i = scanner.GetNumber(); // id
			if (i > m_nSize)
				m_nSize = i;

			scanner.GetToken();
			std::string fileName = scanner.token;
			worldList[scanner.token] = baseWorldStruct(scanner.token);

		}

		scanner.SetMark();
		i = scanner.GetNumber();
		if (i > m_nSize)
			m_nSize = i;
	}
}


bool ffProps::propData::loadWorldWldFile(const std::string& fileName, baseWorldStruct& baseWrld) const
{
	if (const auto [fst, snd] = rmm->findFile(fileName); snd)
	{
		ffTextParser::CScript script;
		script.Load(std::visit(mLoad::load{ fst->first, fileName }, fst->second));

		do
		{
			script.GetToken();
			if (script.Token == "size")
			{
				baseWrld.size.x = script.GetNumber();
				script.GetToken();
				baseWrld.size.y = script.GetNumber();
			}
			else if (script.Token == "ambient")
				script.GetHex();
			else if (script.Token == "revival")
			{
				script.GetNumber();
				script.GetToken();
			}
			else if (script.Token == "indoor")
				script.GetNumber();
			else if (script.Token == "fly")
				script.GetNumber();
			else if (script.Token == "bgm")
				script.GetNumber(); // music id?
			else if (script.Token == "pkmode")
				script.GetNumber();
			else if (script.Token == "diffuse")
				script.GetHex();
			else if (script.Token == "lightDir")
			{
				script.GetFloat();
				script.GetFloat();
				script.GetFloat();
			}
			else if (script.Token == "fogSetting")
			{
				script.GetFloat();
				script.GetFloat();
				script.GetFloat();
			}
			else if (script.Token == "MPU")
				script.GetNumber();
			else if (script.Token == "sky")
			{
				for (int i = 0; i < 3; ++i)
				{
					script.GetToken();
					if ("default" != script.Token)
						baseWrld.sky.emplace_back(script.token);
				}
			}
			else if (script.Token == "cloud")
			{
				for (int i = 0; i < 3; ++i)
				{
					script.GetToken();
					if ("default" != script.Token)
						baseWrld.clouds.emplace_back(script.token);
				}
			}
			else if (script.Token == "sun")
			{
				script.GetToken();
				if ("default" != script.Token)
					baseWrld.sun = script.token;
			}
			else if (script.Token == "moon")
			{
				script.GetToken();
				if ("default" != script.Token)
					baseWrld.moon = script.token;
			}
			else if (script.Token == "use24light")
				script.GetNumber();
			else if (script.Token == "seacloud")
			{
				script.GetToken();
				if ("defualt" != script.Token)
					baseWrld.seaClout = script.token;
			}
		} while (script.tok != ffTextParser::FINISHED);
		return true;
	}
	return false;
}


void ffProps::propData::loadWorldInsides()
{
	if (const auto [fst, snd] = rmm->findFile("world.inc"); snd)
	{
		ffParser::text::CScript cs;
		cs.Load(std::visit(mLoad::load{ fst->first, "world.inc" }, fst->second));
		loadWorldIncBase(std::move(cs));

		for (auto& [iterA, iterB] : worldList)
		{
			std::string tempString = isPackedInOne ? paths::world : "";
			tempString += iterA;
			if (isPackedInOne)
			{
				tempString += '\\';
				tempString += iterA;
			}

			if (std::string innerWorld = tempString += ".wld"; loadWorldWldFile(innerWorld, iterB))
			{
			}
		}
	}
}


std::set<std::string> ffProps::propData::genMdlTexFileNames(const std::set<std::string>& modelFiles) const
{
	std::set<std::string> innerList{};
	for (const auto& str : modelFiles)
	{
		if (const auto [fst, snd] = rmm->findFile(str); snd)
		{
			if (auto val = std::visit(mLoad::load{ fst->first, str }, fst->second); !val.empty())
			{
				std::istringstream strstrm(val, std::stringstream::binary);

				std::filesystem::path p = str;
				std::string extString = p.extension().string();

				std::cout << "Skimming: " << str << std::endl;

				std::set<std::string> results;
				if (extString == ".o3d")
				{
					results = seekTextureObj(strstrm, isPackedInOne);
					innerList.insert(results.begin(), results.end());
				}
				else if (extString == ".sfx")
				{
					results = scanSfxFiles(strstrm);
					innerList.insert(results.begin(), results.end());
				}
			}
		}
		else
		{
			std::cout << "The file: " << str << " doesnt exist" << std::endl;
		}
	}
	return innerList;
}

// todo: cleanup functions ffs
std::set<std::string> ffProps::propData::genModelFileNames() const
{
	std::set<std::string> vs[MAX_OBJTYPE];
	for (int i = 0; i < MAX_OBJTYPE; ++i)
	{
		// ReSharper disable once CppUseStructuredBinding
		for (const auto& val : mdlMng.m_aaModelElem[i])
		{
			std::string wew = isPackedInOne ? i == OT_SFX ? paths::sfx : paths::model : "";
			if (!val.m_szPart.empty())
			{
				wew += "part_";
				if (const auto pos = val.m_szPart.find('/'); pos != std::string::npos)
				{
					std::string innerStr = wew;
					innerStr += val.m_szPart.substr(0, pos) + ".o3d";
					wew += val.m_szPart.substr(pos + 1) + ".o3d";
					vs[i].insert(std::move(innerStr));
					vs[i].insert(std::move(wew));
				}
			}
			else
			{
				std::string cpy = wew;
				wew += std::string(g_szRoot[i]) + "_";
				if (val.m_dwModelType == MODELTYPE_SFX || OT_SFX == i)
				{
					cpy += "sfx_";
					cpy += val.m_szName; // sfx_obj
					wew += val.m_szName; // obj_obj
					// sfx.sfx as well?
					std::string temp2 = val.m_szName + ".sfx"; // todo: tolower on load rather than this

					wew += ".sfx";
					cpy += ".sfx";
					if (auto fileFind = rmm->findFile(wew); fileFind.second)
						vs[i].insert(wew);
					else if (fileFind = rmm->findFile(cpy); fileFind.second)
						vs[i].insert(cpy);
					else if (fileFind = rmm->findFile(temp2); fileFind.second)
						vs[i].insert(temp2);
				}
				else
				{
					wew += val.m_szName;

					if (i == OT_MOVER || i == OT_ITEM || i == OT_CTRL)
					{
						for (const auto& motionIter : val.m_apszMotion)
						{
							std::string tmp = wew;
							tmp += "_";
							tmp += motionIter;

							vs[i].insert(tmp + ".ani");
						}
						if (const auto [fst, snd] = rmm->findFile(wew + ".chr"); snd)
							vs[i].insert(wew + ".chr");
					}
					vs[i].insert(wew + ".o3d");
				}
			}
		}
	}
	std::set<std::string> setter;
	for (auto& v : vs)
	{
		setter.insert(v.begin(), v.end());
		v.clear();
	}
	return setter;
}


std::set<std::string> ffProps::propData::genWorldFileNames()
{
	std::set<std::string> temps;
	for (auto& val : worldList | std::views::values)
	{
		std::string tempString = isPackedInOne ? paths::world : "";
		tempString += val.fileName;
		if (isPackedInOne)
		{
			tempString += '\\';
			tempString += val.fileName;
		}

		temps.insert(tempString + ".wld");
		temps.insert(tempString + ".wld.cnt");
		temps.insert(tempString + ".txt.txt");
		temps.insert(tempString + ".txt");
		temps.insert(tempString + ".rgn");
		temps.insert(tempString + ".dyo");

		for (const auto& str : val.clouds)
			temps.insert(isPackedInOne ? paths::weather + str : str);
		for (const auto& str : val.sky)
			temps.insert(isPackedInOne ? paths::weather + str : str);
		if (!val.moon.empty())
			temps.insert(isPackedInOne ? paths::weather + val.moon : val.moon);
		if (!val.sun.empty())
			temps.insert(isPackedInOne ? paths::weather + val.sun : val.sun);
		if (!val.seaClout.empty())
			temps.insert(isPackedInOne ? paths::weather + val.seaClout : val.seaClout);

		for (int i = 0; i < val.size.x; ++i)
		{
			for (int j = 0; j < val.size.y; ++j)
			{
				std::string wewStr = tempString;
				if (j < 10)
					wewStr += "0";
				wewStr += std::to_string(i);
				if (i < 10)
					wewStr += "0";
				wewStr += "-";
				wewStr += std::to_string(j);
				temps.insert(wewStr + ".lnd");
				temps.insert(wewStr + ".dds");

			}
		}
	}
	return temps;
}


std::set<std::string> ffProps::propData::generateFileList()
{
	if (rmm == nullptr)
		return {};

	std::set<std::string> fileList = genModelFileNames();
	std::set<std::string> fileList2 = genMdlTexFileNames(fileList);
	fileList.insert(fileList2.begin(), fileList2.end());
	fileList2.clear();

	fileList2 = genWorldFileNames();
	fileList.insert(fileList2.begin(), fileList2.end());
	fileList2.clear();


	for (const auto& key : terrain | std::views::keys)
	{
		if (isPackedInOne)
		{
			fileList.insert(paths::world_Texture_Low + key);
			fileList.insert(paths::world_Texture_High + key);
			fileList.insert(paths::world_Texture_Mid + key);
		}
		else
			fileList.insert(key);

	}
	for (const auto& val : itemProp | std::views::values)
		fileList.insert(isPackedInOne ? std::string(paths::item) + val.szIcon : val.szIcon);	
	for (const auto& val : m_mapCharacter | std::views::values)
		fileList.insert(isPackedInOne ? paths::character + std::string(val.m_szChar) : val.m_szChar);

	for (const auto& val : appletList | std::views::values)
	{
		fileList.insert(isPackedInOne ? paths::theme_default + val.strTexture : val.strTexture);
		fileList.insert(isPackedInOne ? paths::theme_english + val.strTexture : val.strTexture);
		fileList.insert(isPackedInOne ? paths::theme_german + val.strTexture : val.strTexture);

		for (const auto& innerVal : val.ptrCtrlArray)
		{
			fileList.insert(isPackedInOne ? paths::theme_default + innerVal.strTexture : innerVal.strTexture);
			fileList.insert(isPackedInOne ? paths::theme_english + innerVal.strTexture : innerVal.strTexture);
			fileList.insert(isPackedInOne ? paths::theme_german + innerVal.strTexture : innerVal.strTexture);
		}
	}
	return fileList;
}