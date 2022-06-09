#pragma once
#include <map>
#include <string>
#include <vector>
#include "commonTypes.hpp"
#include "ffParser.h"
#include <set>

constexpr unsigned long MAX_OBJARRAY = 8;
constexpr unsigned long MAX_QUICKSLOT = 21;
constexpr unsigned long MAX_EQUIPMENT = 18;
constexpr unsigned long MAX_PROPMOVER = 14900;
constexpr unsigned long MAX_HONOR_TITLE = 150;
constexpr unsigned long MAX_VENDOR_REVISION	 = 20;
constexpr unsigned long MAX_AMP_STACK_COUNT = 5;
constexpr unsigned long MAX_QUEST_COND_KILL = 2;
constexpr unsigned long MAX_QUESTITEM = 16;
constexpr unsigned long MAX_DROPKIND = 80;
constexpr unsigned long MAX_BEGINEQUIP = 10;
constexpr unsigned long MAX_BEGINITEM  = 42; //20 
constexpr unsigned long MAX_JOBITEM    = 10; 
constexpr unsigned long MAX_BEGINPOS = 16;
constexpr unsigned long ITEM_KIND_MAX = 100;

constexpr unsigned long MAX_JOB = 40;
constexpr unsigned long MAX_MONSTER_LEVEL = 150;
constexpr unsigned long MAX_RANDOMOPTITEM = 256;

constexpr unsigned long long skinvertSize = (sizeof(types::vec3d<float>) * 2) + (sizeof(float) * 2) + sizeof(int) + (sizeof(float) * 2);
constexpr unsigned long long normalVertSize = (sizeof(types::vec3d<float>) * 2) + (sizeof(float) * 2);
constexpr unsigned long long matBlockSize = sizeof(int) * 34;
constexpr unsigned long long tmAniSize = sizeof(types::vec4d<float>) + sizeof(types::vec3d<float>);


#define MAX_RESPAWN			1536
#define MAX_MONSTER_PROP	1024
#define MAX_GUILDAPPELL		5
#define MAX_HUMAN_PARTS 	31
#define MAX_QUESTPROPITEM  4
#define MAX_QUESTCONDITEM  128
#define MAX_QUESTREMOVE    12
#define	MAX_RANDOMOPTITEMSTRING		32

#define	MAX_PIERCING_DSTPARAM	32
#define	MAX_PIERCING_MATERIAL	128
#define	MAX_SETITEM_STRING		64
#define	MAX_SETITEM_ELEM		8

#define	MAX_ITEMAVAIL	32
#define MAX_REALITEM			8			
#define MAX_EVENTREALITEM		4096
#define MAX_EXPJOBLEVEL			90

#define MAX_MOVER_MENU				450	

#define	MAX_GUILD_QUEST_TITLE_LEN		32
#define	MAX_GUILD_QUEST_STATE		16
#define	MAX_GUILD_QUEST		256
#define	MAX_GUILD_QUEST_DESC_LEN	260

#define MAX_PATH 256

#define OT_OBJ          0
#define OT_ANI          1
#define OT_CTRL         2
#define OT_SFX          3
#define OT_ITEM         4
#define OT_MOVER        5
#define OT_REGION       6
#define OT_SHIP			7
#define OT_PATH			8
#define MAX_OBJTYPE     9

#define MODELTYPE_NONE               0
#define MODELTYPE_MESH               1
#define	MODELTYPE_ANIMATED_MESH		 2
#define MODELTYPE_BILLBOARD          3 
#define MODELTYPE_SFX                4
#define MODELTYPE_ASE				 5

#define SEX_MALE             0
#define SEX_FEMALE           1
#define SEX_SEXLESS          2

#define MAX_STRUCTURE            20

#define	MAX_EXPLPPOINT			41
#define	MAX_EXPSKILL			21
#define	MAX_EXPCHARACTER		200
#define	MAX_FXPCHARACTER		200

#define MAX_PARTYLEVEL			10
#define MAX_ADDEXPPARTY			16
#define	MAX_PARTY_QUEST_TITLE_LEN		32
#define	MAX_PARTY_QUEST_STATE		16
#define	MAX_PARTY_QUEST		256
#define	MAX_PARTY_QUEST_DESC_LEN	260

#define DEF_CTRL_CHECK "ButtCheck.bmp"
#define DEF_CTRL_RADIO "ButtRadio.bmp"
#define DEF_CTRL_GROUP "WndEditTile200.tga"
#define DEF_CTRL_TAB1  "WndTabTile00.bmp"
#define DEF_CTRL_TAB2  "WndTabTile10.bmp"
#define DEF_WNDBASE    "WndTile00.tga"
#define DEF_CTRL_TEXT  "WndEditTile00.tga"

//#define MAX_ITEM_KIND3			251		// ItemKind3 ÃÑ °³¼ö

namespace res::props
{
	enum class propVer { ver15, ver16, ver19 };

	static const char* defaultDefines[] = { "ContinentDef.h", "define.h", "defineattribute.h", "defineevent.h", "definehonor.h", "defineitem.h", "defineitemgrade.h", 
		"defineitemkind.h", "defineitemtype.h", "definejob.h", "definelordskill.h", "definemapcomboboxdata.h", "defineneuz.h", "defineobj.h",  "definequest.h", 
		"defineskill.h", "definesound.h", "definetext.h", "defineworld.h", "lang.h", "resdata.h", "wndstyle.h"
	};

	static const char* defaultTxtDefs[] = { "character.txt.txt", "character-etc.txt.txt", "etc.txt.txt", "honorList.txt.txt", "lordskill.txt.txt", "MiniGame_Alphabet.txt.txt",
		"PatrolDestination.txt.txt", "propCtrl.txt.txt", "propItem.txt.txt", "propItemEtc.txt.txt", "propKarma.txt.txt", "propMapComboBoxData.txt.txt", "propMotion.txt.txt",
		"propMover.txt.txt", "propQuest.txt.txt", "propQuest-DungeonandPK.txt.txt", "PropQuest-DungeonandPKtxt.txt", "propQuest-RequestBox.txt.txt", "propQuest-Scenario.txt.txt",
		"propSkill.txt.txt", "propTroupeSkill.txt.txt", "QuestDestination.txt.txt", "resData.txt.txt", "textClient.txt.txt", "textEmotion.txt.txt", "world.txt.txt"
	};

	static const char* defaultInc[] = { "accessory.inc", "character.inc", "character-etc.inc", "collecting.inc", "constant.inc", "couple.inc", "diepenalty.inc", "election.inc",
		"etc.inc", "exptable.inc", "filter.inc", "invalidname.inc", "jobitem.inc", "layer.inc", "letter.inc", "letter2.inc", "lordevent.inc", "mdldyna.inc", "mdlobj.inc", "mount.inc",
		"pet.inc", "pksetting.inc", "propbadge.inc", "propdropevent.inc", "propenchant.inc", "propevent.inc", "propflyffpiece.inc", "propgiftbox.inc", "propguildquest.inc",
		"propitemetc.inc", "propjob.inc", "propmapcomboboxdata.inc", "propmoverex.inc", "proppackitem.inc", "proppartyquest.inc", "propquest.inc", "propquest-dungeonandpk.inc",
		"propquest-requestbox.inc", "propquest-requestbox2.inc", "propquest-scenario.inc", "randomeventmonster.inc", "randomoption.inc", "resdata.inc", "terrain.inc", "textclient.inc",
		"ticket.inc", "transformitem.inc", "worldtooltip.inc", "world.inc"
	};
	static const char* defaultCSV[] = { "propctrl.csv", "propitem.csv", "propmotion.csv", "propmover.csv", "propobj.csv", "propskill.csv", "propskilladd.csv", "proptroupeskill.csv" };
	static const char* defaultTxt[] = { "ai_script.txt", "anarchy.txt", "createmonster.txt", "EnvironmentEffect.txt", "except.txt", "Exchange_Script.txt", "GuildCombat.txt", 
		"honorList.txt", "ItemMergeRandomOption.txt", "lordskill.txt", "MiniGame_Alphabet.txt", "MiniGame_FiveSystem.txt", "MiniGame_KawiBawiBo.txt", "MiniGame_Reassemble.txt", 
		"PaidWorldSet.txt", "PaidWorldTicket.txt", "PiercingSize.txt", "propCtrl.txt", "propItem.txt", "propKarma.txt", "propLang.txt", "propMotion.txt", "propMover.txt",  
		"propSkill.txt", "propTroupeSkill.txt", "provide.txt", "removequest.txt", "s.txt", "Spec_Item.txt", "spevent.txt", "StartPartyQuestInfo.txt", "teleportscroll.txt", 
		"textEmotion.txt",  "TreasureCombine.txt", "TreasureItem.txt", "Ultimate_GemAbility.txt", "Ultimate_UltimateWeapon.txt"
	};

	static const char* wndtiles[] = { "wndedittile00.tga", "wndedittile01.tga", "wndedittile02.tga", "wndedittile03.tga", "wndedittile04.tga", "wndedittile05.tga", "wndedittile06.tga", 
		"wndedittile07.tga", "wndedittile08.tga", "wndedittile09.tga", "wndedittile10.tga", "WndEditTile200.tga", "WndEditTile201.tga", "WndEditTile202.tga", "WndEditTile203.tga",
		"WndEditTile204.tga", "WndEditTile205.tga", "WndEditTile206.tga", "WndEditTile207.tga", "WndEditTile208.tga", "WndTile00.tga", "WndTile01.tga", "WndTile02.tga", "WndTile03.tga",
		"WndTile04.tga", "WndTile05.tga", "WndTile06.tga", "WndTile07.tga", "WndTile08.tga", "WndTile09.tga", "WndTile10.tga", "WndTile11.tga", "WndTile200.tga", "WndTile201.tga", 
		"WndTile202.tga", "WndTile203.tga", "WndTile204.tga", "WndTile205.tga", "WndTile206.tga", "WndTile207.tga", "WndTile208.tga", "WndTile209.tga", "WndTile210.tga", "WndTile211.tga",
		"WndTile300.tga", "WndTile301.tga", "WndTile302.tga", "WndTile303.tga", "WndTile304.tga", "WndTile305.tga", "WndTile306.tga", "WndTile307.tga", "WndTile308.tga", "WndTile309.tga",
		"WndTile310.tga", "WndTile311.tga"
	};

	namespace levelFlyffFile
	{
		static const char* mainResource[] = { "Achievements.inc", "advteleport.txt", "Autoconsumablefilter.inc", "Crafting.inc", "Dailyreward.txt", "globalbuffs.inc",
				"jobchangerewards.inc", "Luckyjackpot.inc", "Monsterhunt.inc", "TriggerQuests.inc", "charinfo.inc", "wiki_item.inc", "glowchange.inc"
		};

		static const char* mainResourceDefines[] = { "defineachievementstypes.h" };
		static const char* mainResourceTxt[] = { "globalbuffs.txt.txt", "propbadge.txt.txt" };

		static const char* themeResource[] = { "back_slotitem.bmp", "achievbg.tga", "shopitem.tga", "teleportperso.tga", "shopbuy.tga", "shopfriend.tga", "wndchgelemitem.tga",
			"check01.tga", "check02.tga", "crossedswords.png", "easy.tga", "medium.tga", "hard.tga", "dmg.png", "health.png", "level.png", "sablier.png", "farmpausebutton.tga",
			"farmplaybutton.tga", "safteygauge.bmp", "Wndinventorytool_element.tga", "wndinventorytoolclose_element.tga", "wndinventorytool_dem.tga", "wndinventorytool_cosmetic.tga",
			"wndinventorytoolclose_cosmetic.tga", "wndinventorytool_macro.tga", "wndinventorytoolclose_macro.tga", "slotknight.bmp", "slotblad.bmp", "slotjes.bmp", "slotran.bmp",
			"slotringm.bmp", "slotbillfor.bmp", "slotpsy.bmp", "slotelem.bmp", "slotlord.bmp", "40.bmp", "slotstormb.bmp", "41.bmp", "slotforcem.bmp", "45.bmp", "slotflor.bmp",
			"44.bmp", "slotelel.bmp", "47.bmp", "slotment.bmp", "46.bmp", "slotwindl.bmp", "slot42.bmp", "slotcracks.bmp", "43.bmp", "back_slotitem1.bmp", "back_slotitem2.bmp",
			"back_slotitem3.bmp", "back_slotitem4.bmp", "luckyjackpotcadre.bmp", "bannerlotery2.tga", "bannerlotery3.tga", "bannerlotery4.tga", "bannerlotery1.tga", "wndnotuse.tga",
			"monsterhuntbonus.png", "testborder.tga", "imgnavarrow.bmp", "buttmap04.tga", "buttmap01.tga", "buttmap02.tga", "buttmap05.tga", "navdunfog.tga", "buttdestination.bmp",
			"wdguildwar_mask.dds", "alphacircle.tga",
			//possibly somewhere else
			"jobmercenary.tga", "jobacrobat.tga", "jobassist.tga", "jobmagician.tga", "jobknight.tga", "jobblade.tga", "jobjester.tga", "jobranger.tga", "jobringmaster.tga",
			"jobbillposter.tga", "jobpsychikeeper.tga", "jobelementor.tga", "jobknightmaster.tga", "jobblademaster.tga", "jobjestermaster.tga", "jobrangermaster.tga",
			"jobringmastermaster.tga", "jobpsychikeepermaster.tga", "jobelementormaster.tga", "jobknighthero.tga", "jobbladehero.tga", "jobjesterhero.tga", "jobrangerhero.tga",
			"jobringmasterhero.tga", "jobbillposterhero.tga", "jobpsychikeeperhero.tga", "jobelementorhero.tga", "joblordtemplerhero.tga", "jobstormbladehero.tga",
			"jobwindlurkerhero.tga", "jobcrackshooterhero.tga", "jobfloristhero.tga", "jobforcemasterhero.tga", "jobmentalisthero.tga", "jobelementorlordhero.tga", "charinfo40.png",
			"charinfo41.png", "charinfo42.png", "charinfo43.png", "charinfo44.png", "charinfo45.png", "charinfo46.png", "charinfo47.png", "jobnpc.tga", "itemgrade5.dds",

			// more stuff
			"gaumagchargebg.tga", "gaumagchargehead.tga", "gaumagchargebody.tga", "gaumagchargebody2.tga", "gaumagchargetail.tga", "gaupartyhpbg.tga", "gaupartyhp.tga",
			"gaupartyhp_dbf.tga", "gauaccelbg.tga", "gauaccelinner.tga", "gauangelexpbg.tga", "gauangelexp.tga", "default.tgm", "imgmaparrow.bmp", "imgmarrowparty.bmp",
			"buttteleport.bmp", "buttwndmin.tga", "buttwndmax.tga", "lvup.bmp", "gauemptynormal.bmp", "lvdn.bmp", "lvup2.bmp", "lvdn2.bmp", "texguildcombatwinner.bmp",
			"texguildcombatbest.bmp", "wndcolocountdesign.tga", "rangreen02.tga", "wndinventorytool.tga", "wndinventorytoolclose.tga", "butttreeopen.tga", "butttreeclose.tga",
			"buttcscrbar.bmp", "buttvscrpup.bmp", "buttvscrpdown.bmp", "buttvscrpbar.bmp", "buttvscrup.tga", "buttvscrdown.tga", "exchangearrow.tga",

			//element something?
			"feu.tga", "eau.tga", "electrique.tga", "vent.tga", "terre.tga", "neutre.tga", "buttquicklistdn.tga", "selectcadre03.png", "selectcadre01.png",
			"wndfield.bmp", "wndaboutflyff.tga", "wndgauge.tga", "inventorybageff.tga", "wndcommonbanknotuse.tga", "wndnewcharacter01.tga", "butstateok.tga", "butstatecancel.tga",
			"buttcharapply.tga", "buttcharreset.tga", "selectcadre02.png", "wndcharacter2_1.tga", "wndcharacter2.tga", "buttcharplus.bmp", "wndcharacter2_2.tga",
			"imgskillvagrant.tga", "back_hero_kntdrawing.tga", "back_hero_blddefence.tga", "back_hero_blddefence.tga", "back_hero_jstsilence.tga", "back_hero_raghawkeye.tga",
			"back_hero_rigreturn", "back_hero_bildisenchant.tga", "back_hero_psystone.tga", "back_hero_elecursemind.tga", "back_me.tga", "back_night.tga", "slotmer.bmp",
			"skilltreelord.tga", "back_blade.tga", "skilltreestormb.tga", "back_as.tga", "back_bill.tga", "slotas.bmp", "skilltreeforcm.tga", "back_ring.tga", "skilltreeflor.tga",
			"back_ma.tga", "back_ele.tga", "slotmag.bmp", "skilltreelel.tga", "back_psy.tga", "skilltreewindl.tga", "back_rag.tga", "skilltreecracks.tga", "wndpostmark.tga",
			"yellowbutten.tga", "buttok2.bmp", "wnddisableblue.bmp", "wnddisableblack.bmp", "wnddisablered.bmp", "gauemptysmall.bmp", "gaufillsmall.bmp", "questuipapergreen.tga",
			"questuipaperred.tga", "questuipapergray.tga", "questuipaperyellow.tga", "wndtrash.png", "wndtrash2.png", "gaufillnormal.bmp", "buttnormal02.tga", "bg_coupleskill.tga",
			"safetysuccess.bmp", "safetyfailure.bmp", "cologauge01.bmp", "buttlockerinstall.bmp", "buttlockerdismantle.bmp", "wndmapblind.dds", "wndvolumebar.tga",
			"buttspin.tga", "partyleader.png", "petlevelbg.tga", "petstatusbg.tga", "buttchance.bmp", "buttyes.tga", "buttno.tga", "buffpetstatusbg .tga",
			"looklchange01.tga", "wndpvp2.tga", "checkred.bmp", "checkorange.bmp", "checkyellow.bmp", "checkgreen.bmp", "checkblue.bmp", "checkindigo.bmp",
			"checkviolet.bmp", "buttstop.bmp", "buttstart.bmp", "hotkey.inc", "wndtaskmenu.tga", "wndbuttmacrochat.tga", "wndchatedit00_1.tga", "wndchatedit1.tga", "wndchatedit02.tga",
			"wndvendorarrowex.tga", "mapnpcpositionmark.bmp", "hyperlinkusermark.bmp", "teleportmark01.tga", "teleportmark02.tga",

			"alert_character.bmp",

		};

		static const char* themeResTexinc[] = { "navposition.inc", "navigator.inc", "icon_questemoticon.inc", "countfight.inc", "texmapbutton.inc", "texmapmonster_darkon12.inc",
			"texmapmonster_darkon3.inc", "texmapmonster_ricis.inc", "texmapmonster_saint.inc", "texmapmonster_flaris.inc", "texmapmonster_harmonin.inc",
			"texmapmonster_estia.inc", "texmaprainbow_npc.inc", "theme_gauflight.inc", "theme_fontdigital1.inc",

		};

		static const char* themeResTele[] = { "filler" };
		static const char* iconResource[] = { "Icon_teleporterTask.png", "levelpoint.png", "votepoint.png", "icon_originpointsbind.png", "timeicon.png", "monstericon.png",
			"vagi_rank.png", "merc_rank.png", "acro_rank.png", "assist_rank.png", "mage_rank.png", "knight_rank.png", "rm_rank.png", "bp_rank.png", "psy_rank.png", "ranger_rank.png",
			"jester_rank.png", "icon_pvp.dds", "icon_ultimate.dds", "skill_trogiftbox02.dds", "icon_expert1.dds", "icon_expert2.dds", "icon_expert3.dds", "icon_expert4.dds",
			"icon_expert5.dds", "icon_expert6.dds", "icon_hero.dds",  "icon_forbid.dds", "icon_navigator.dds", "icon_status.dds", "icon_applet.dds", "icon_housing.dds",
			"icon_character.dds", "icon_inventory.dds", "icon_citemmail.dds", "icon_skill.dds", "icon_motion.dds", "icon_trade.dds", "icon_quest.dds", "icon_troupe.dds",
			"icon_citemtime.dds", "icon_chat.dds", "icon_messenger.dds", "icon_optsound.dds", "icon_helperhelp.dds", "icon_helpertip.dds", "icon_helperfaq.dds", "icon_logout.dds",
			"icon_quit.dds", "qoodo.dds", "icon_login.dds", "icon_selectserver.dds", "icon_createchar.dds", "icon_couple.dds", "icon_system02.png", "item_coffre_inva.png",
			"icon_system07.png", "icon_system01.png", "icon_system05.png", "icon_system08.png", "icon_system04.png", "icon_macrochat.dds", "icon_system06.png", "icon_system03.png",
			"icone_miseajour.tga", "icone_changementclasse.tga", "cristal.png", "icon_baruna01.dds", "icon_mastermark1.dds", "icon_mastermark2.dds", "icon_mastermark3.dds",
			"icon_mastermark4.dds", "icon_mastermark5.dds", "icon_mastermark6.dds", "icon_heromark.dds", "icon_lock.dds", "icon_folder.dds", "icon_actionskill.dds",
		};



		static const char* clientResource[] = { "summonangelquest.inc", "itemawakening.inc", "itemblessingcancel.inc", "smeltmixjewel.inc", "smeltextraction.inc",
			"smeltjewel.inc", "smeltchangeuniqueweapon.inc", "changeuniqueweaponinfo.inc", "smeltchangeultimateweapon.inc", "changeultimateweaponinfo.inc", "heroskillup.inc",
			"removeattribute.inc", "smeltremovepiercing.inc", "smeltremovejewel.inc", "changeattribute.inc", "coupleskillinfo.inc", "treehelp.inc", "faq.inc", "tip.inc",
			"lordinfo.inc", "lordrpinfo.inc", "lordcandidate.inc", "reqlvdown.inc", "minigamefindworddesc.inc", "minigamedicedesc.inc", "minigamepuzzledesc.inc",
			"petawakcancel.inc", "petmiracle.inc", "guildcombatredchilguild.inc", "secretroomdesc.inc", "secretroomchangetaxrate.inc", "secretroomchecktaxrate.inc",
			"secretroomboard.inc", "tip.txt.txt", "treehelp.txt.txt", "instanthelp.txt.txt", "patchclient.txt.txt", "help.txt.txt", "guide.txt.txt", "tutorial.txt.txt",
			"faq.txt.txt"
		};

		static const char* weatherResource[] = { "moon.dds", "sundisk.bmp", "flare_3.tga", "lenzflare_001.tga", "lenzflare_002.tga", "lenzflare_003.tga", "lenzflare_004.tga",
			"lenzflare_005.tga", "lenzflare_006.tga", "lenzflare_007.tga", "lenzflare_008.tga", "skybox01_low.dds", "skybox02_low.dds", "skybox03_low.dds",
			"cloud01_low.dds", "cloud02_low.dds", "cloud03_low.dds", "cloud03.dds", "cloud02.dds", "cloud01.dds", "skybox01.dds", "skybox02.dds", "skybox03.dds"

		};

		static const char* iconTexInc[] = { "icon_target.inc", "icon_flighttargetB.inc", "icon_flighttargetarrow.inc", "icon_iconmessenger.inc", "icon_monelemantkind.inc", "icon_playerdata.inc",
			"icon_iconskilllevel.inc"
		};
		static const char* tex2dSFX[] = { "dmgeffect.inc", "dmghealeffect.inc" };

		//icon\\frames\\
		//


		//unknown
		//"farmingfilters.txt"
		
		//icon_imgani_%.2d.dds
		// theme\default\cartetresor
		// 
		// 
		// 36
		inline [[nodiscard]] std::string GetCadreBleu(const int loopcnt) { return loopcnt < 10 ? "cadrebleu_0" + std::to_string(loopcnt) : "Cardrebleu_" + std::to_string(loopcnt); }
		//27
		inline [[nodiscard]] std::string GetCloakLogo(const int loopcnt) { return loopcnt < 10 ? "icon_cloakslogo0" + std::to_string(loopcnt) : "icon_cloakslogo" + std::to_string(loopcnt); }
		//11
		inline [[nodiscard]] std::string GetHeadMesh(const int loopcnt, const int sex, bool IsHair = false) {
			if (!IsHair)
				return sex == 0 ? (loopcnt < 10 ? "part_malehead0" + std::to_string(loopcnt) : "part_malehead" + std::to_string(loopcnt)) 
					: (loopcnt < 10 ? "part_femalehead0" + std::to_string(loopcnt) : "part_femalehead" + std::to_string(loopcnt))
					; 
			return sex == 0 ? (loopcnt < 10 ? "part_malehair0" + std::to_string(loopcnt) : "part_malehair" + std::to_string(loopcnt))
				: (loopcnt < 10 ? "part_femalehair0" + std::to_string(loopcnt) : "part_femalehair" + std::to_string(loopcnt))
			;
		}

	}


	enum {
		FILE_FILTER = 0, FILE_INVALID = 1, FILE_NOTICE = 2, FILE_GUILDCOMBAT_TEXT_1 = 3, FILE_GUILDCOMBAT_TEXT_2 = 4, FILE_GUILDCOMBAT_TEXT_3 = 5,
		FILE_GUILDCOMBAT_TEXT_4 = 6, FILE_GUILDCOMBAT_TEXT_5 = 7, FILE_GUILDCOMBAT_TEXT_6 = 8, FILE_ALLOWED_LETTER = 9, FILE_GUILDCOMBAT_1TO1_TEXT_1 = 10,
		FILE_GUILDCOMBAT_1TO1_TEXT_2 = 11, FILE_GUILDCOMBAT_1TO1_TEXT_3 = 12, FILE_GUILDCOMBAT_1TO1_TEXT_4 = 13, FILE_GUILDCOMBAT_1TO1_TEXT_5 = 14,
		FILE_ALLOWED_LETTER2 = 15
	};
	enum IP_TYPE { IP_FLAG_NONE = 0x00, IP_FLAG_BINDS = 0x01, IP_FLAG_UNDESTRUCTABLE = 0x02, IP_FLAG_EQUIP_BIND = 0x04 };



	enum SFXPARTTYPE : int { SFXPARTNONE, SFXPARTTYPE_BILL, SFXPARTTYPE_PARTICLE, SFXPARTTYPE_MESH, SFXPARTTYPE_CUSTOMMESH };

	enum SFXPARTBILLTYPE : int {
		SFXPARTBILLTYPE_BILL = 1,
		SFXPARTBILLTYPE_BOTTOM = 2,
		SFXPARTBILLTYPE_POLE = 3,
		SFXPARTBILLTYPE_NORMAL = 4,
	};
	enum SFXPARTALPHATYPE : int {
		SFXPARTALPHATYPE_BLEND = 1,
		SFXPARTALPHATYPE_GLOW = 2,
	};

	enum class SFXLOADVER : int { oldLoad, load1, load2, load3 };



	struct tagColorText
	{
		unsigned char dwColor;
		char* lpszData;
	};

	struct ObjProp
	{
		unsigned long dwID;
		char szName[64]{};	
		unsigned long dwType;
		unsigned long dwAI;
		unsigned long dwHP;
		ObjProp()
		{
			*szName = '\0';
			dwID = dwType = dwAI = dwHP = 0;
		}
	};

	struct CtrlProp : ObjProp
	{
		unsigned long dwCtrlKind1;
		unsigned long dwCtrlKind2;
		unsigned long dwCtrlKind3;
		unsigned long dwSfxCtrl;
		unsigned long dwSndDamage;
		CtrlProp() : ObjProp(), dwCtrlKind1(0), dwCtrlKind2(0), dwCtrlKind3(0), dwSfxCtrl(0), dwSndDamage(0)
		{
		}
		//int IsGuildHousingObj() { return CK1_GUILD_HOUSE == dwCtrlKind1; }
		//int IsHousingObj() { return CK1_HOUSING == dwCtrlKind1; }
	};

	struct AddSkillProp
	{
		unsigned long dwID;				
		unsigned long dwName;				
		unsigned long dwSkillLvl;			
		unsigned long dwAbilityMin;		
		unsigned long dwAbilityMax;		
		unsigned long dwAttackSpeed;		
		unsigned long dwDmgShift;			
		unsigned long nProbability;		
		unsigned long dwTaunt;			
		unsigned long dwDestParam[2]{0};
		unsigned long nAdjParamVal[2]{0};
		unsigned long dwChgParamVal[2]{0};
		int	nDestData1[3]{0};
		unsigned long dwActiveSkill;		
		unsigned long dwActiveSkillRate;	
		unsigned long dwActiveSkillRatePVP;
		int nReqMp;				
		int nReqFp;				
		unsigned long dwCooldown;		
		unsigned long dwCastingTime;	
		unsigned long dwSkillRange;	
		unsigned long dwCircleTime;	
		unsigned long dwPainTime;     
		unsigned long dwSkillTime;	
		int nSkillCount;	
		unsigned long dwSkillExp;     
		unsigned long dwExp;			
		unsigned long dwComboSkillTime;
		unsigned long dwAbilityMinPVP;
		unsigned long dwAbilityMaxPVP;
		unsigned long nProbabilityPVP;

		AddSkillProp() : dwID(0), dwName(0), dwSkillLvl(0), dwAbilityMin(0), dwAbilityMax(0), dwAttackSpeed(0), dwDmgShift(0), nProbability(0), dwTaunt(0), dwActiveSkill(0),
						 dwActiveSkillRate(0), dwActiveSkillRatePVP(0), nReqMp(0), nReqFp(0), dwCooldown(0), dwCastingTime(0), dwSkillRange(0), dwCircleTime(0), dwPainTime(0),
						 dwSkillTime(0), nSkillCount(0), dwSkillExp(0), dwExp(0), dwComboSkillTime(0), dwAbilityMinPVP(0), dwAbilityMaxPVP(0), nProbabilityPVP(0)
		{
		}
	};

	struct PartySkillProp
	{
		unsigned long dwID;	
		unsigned long dwName;	
		unsigned long dwSkillLvl;
		unsigned long dwAbilityMin;	
		unsigned long dwAtkAbilityMax;	
		unsigned long dwAttackSpeed;
		unsigned long dwDmgShift;
		unsigned long dwGroggy;	
		unsigned long dwTaunt;
		unsigned long dwDestParam[2]{ 0 };
		unsigned long nAdjParamVal[2]{ 0 };
		unsigned long dwChgParamVal[2]{ 0 };
		unsigned long dwReqMp;
		unsigned long dwReqFp;
		unsigned long dwSkillReady;		
		unsigned long dwCircleTime;	
		unsigned long dwSkillTime;
		unsigned long dwExp;
		unsigned long dwComboSkillTime;	
		PartySkillProp() : dwID(0), dwName(0), dwSkillLvl(0), dwAbilityMin(0), dwAtkAbilityMax(0), dwAttackSpeed(0), dwDmgShift(0), dwGroggy(0), dwTaunt(0), dwReqMp(0),
						   dwReqFp(0), dwSkillReady(0), dwCircleTime(0), dwSkillTime(0), dwExp(0), dwComboSkillTime(0)
		{
		}
	};


	struct MOUNT_ANIMATION 
	{
		unsigned char mType;
		unsigned long dwPlayerAni;
		types::vec3d<float> vOffSet;
		types::vec3d<float> vRotation;
		unsigned long dwBindBone;

		std::map<int, std::string> m_aMonsterAnimation;
		float fGroundSpeed;
		float fFlightSpeed;

		MOUNT_ANIMATION() : mType(0), dwPlayerAni(0), dwBindBone(0), fGroundSpeed(0.0f), fFlightSpeed(0.0f)
		{
		}
	};

	struct ItemProp : CtrlProp
	{
		unsigned long dwMotion;	
		unsigned long dwNum;
		unsigned long dwPackMax;
		unsigned long dwItemKind1;
		unsigned long dwItemKind2;
		unsigned long dwItemKind3;
		unsigned long dwItemJob;
		int bPermanence;	
		unsigned long dwUseable;		
		unsigned long dwItemSex;		
		unsigned long dwCost;			
		unsigned long dwEndurance;	
		int nLog;			
		int nAbrasion;		
		int nMaxRepair;		
		unsigned long dwHanded;		
		unsigned long dwFlag;			
		unsigned long dwParts;		
		unsigned long dwPartsub;		
		unsigned long bPartsFile;		
		unsigned long dwExclusive;	
		unsigned long dwBasePartsIgnore;
		unsigned long dwItemLV;		
		unsigned long dwItemRare;
		unsigned long dwShopAble;
		int	nShellQuantity;	
		unsigned long dwActiveSkillLv;
		unsigned long dwFuelRe;		
		unsigned long dwAFuelReMax;	
		unsigned long dwSpellType;	
		unsigned long dwLinkKindBullet;
		unsigned long dwLinkKind;		
		unsigned long dwAbilityMin;	
		unsigned long dwAbilityMax;	
		int bCharged;
		int eItemType; // sai
		short wItemEatk;
		unsigned long dwParry;		
		unsigned long dwblockRating;
		int nAddSkillMin;
		int nAddSkillMax;
		unsigned long dwAtkStyle;	
		unsigned long dwWeaponType;		
		unsigned long dwItemAtkOrder1;	
		unsigned long dwItemAtkOrder2;	
		unsigned long dwItemAtkOrder3;	
		unsigned long dwItemAtkOrder4;	
		unsigned long tmContinuousPain;	
		unsigned long dwRecoil;			
		unsigned long dwLoadingTime;	
		long nAdjHitRate;
		float fAttackSpeed;		
		unsigned long dwDmgShift;
		unsigned long dwAttackRange;
		int nProbability;

		unsigned long dwDestParam[6];	
		long nAdjParamVal[6];
		unsigned long dwChgParamVal[6];
		int nDestData1[6];

		unsigned long dwActiveSkill;
		unsigned long dwActiveSkillRate;
		unsigned long dwReqMp;
		unsigned long dwReqFp;
		unsigned long dwReqDisLV;
		unsigned long dwReSkill1;
		unsigned long dwReSkillLevel1;
		unsigned long dwReSkill2;
		unsigned long dwReSkillLevel2;
		unsigned long dwSkillReadyType;
		unsigned long dwSkillReady;		
		unsigned long _dwSkillRange;	
		unsigned long dwSfxElemental;
		unsigned long dwSfxObj;			
		unsigned long dwSfxObj2;		
		unsigned long dwSfxObj3;		
		unsigned long dwSfxObj4;		
		unsigned long dwSfxObj5;		
		unsigned long dwUseMotion;		
		unsigned long dwCircleTime;		
		unsigned long dwSkillTime;		
		unsigned long dwExeTarget;
		unsigned long dwUseChance;
		unsigned long dwSpellRegion;	
		unsigned long dwReferStat1;
		unsigned long dwReferStat2;
		unsigned long dwReferTarget1;
		unsigned long dwReferTarget2;
		unsigned long dwReferValue1;
		unsigned long dwReferValue2;
		unsigned long dwSkillType;		
		int nItemResistElecricity;
		int nItemResistDark;
		int nItemResistFire;
		int nItemResistWind;
		int nItemResistWater;
		int nItemResistEarth;
		long nEvildoing;			
		unsigned long dwExpertLV;
		unsigned long dwExpertMax;	
		unsigned long dwSubDefine;
		unsigned long dwExp;		
		unsigned long dwComboStyle;
		float fFlightSpeed;		
		float fFlightLRAngle;		
		float fFlightTBAngle;		
		unsigned long dwFlightLimit;
		unsigned long dwFFuelReMax;	
		unsigned long dwLimitLevel1;
		int		nReflect;			
		unsigned long dwSndAttack1;	
		unsigned long dwSndAttack2;	
		unsigned long dwQuestId;
		char szTextFileName[64];	

		char szIcon[64];			
		char szCommand[256];		
		int nVer;

		unsigned long dwBuffTickType;
		int nMinLimitLevel;
		int nMaxLimitLevel;
		int nItemGroup;
		int nUseLimitGroup;
		int nMaxDuplication;
		int nEffectValue;
		int nTargetMinEnchant;
		int nTargetMaxEnchant;
		int bResetBind;
		int nBindCondition;
		int nResetBindCondition;
		unsigned long dwHitActiveSkillId;
		unsigned long dwHitActiveSkillLv;
		unsigned long dwHitActiveSkillProb;
		unsigned long dwHitActiveSkillTarget;
		unsigned long dwDamageActiveSkillId;
		unsigned long dwDamageActiveSkillLv;
		unsigned long dwDamageActiveSkillProb;
		unsigned long dwDamageActiveSkillTarget;
		unsigned long dwEquipActiveSkillId;
		unsigned long dwEquipActiveSkillLv;
		unsigned long dwSmelting;
		unsigned long dwAttsmelting;
		unsigned long dwGemsmelting;
		unsigned long dwPierce;
		unsigned long dwUprouse;
		int bAbsoluteTime;

		unsigned long dwItemGrade;
		int bCanTrade;
		unsigned long dwMainCategory;
		unsigned long dwSubCategory;
		int bCanHaveServerTransform;
		int bCanSavePotion;
		int bCanLooksChange;
		int blsLooksChangeMaterial;
		int bCanUseActionSlot;
		unsigned long dwEquipItemKeepSkill;
		unsigned long dwMonsterGrade;


		[[nodiscard]] unsigned long GetCoolTime() const { return dwSkillReady; }

		ItemProp() : dwMotion(0), dwNum(0), dwPackMax(0), dwItemKind1(0), dwItemKind2(0), dwItemKind3(0), dwItemJob(0), bPermanence(0), dwUseable(0), dwItemSex(0), dwCost(0),
		    dwEndurance(0), nLog(0), nAbrasion(0), nMaxRepair(0), dwHanded(0), dwFlag(0), dwParts(0), dwPartsub(0), bPartsFile(0), dwExclusive(0), dwBasePartsIgnore(0),
			dwItemLV(0), dwItemRare(0), dwShopAble(0), nShellQuantity(0), dwActiveSkillLv(0), dwFuelRe(0), dwAFuelReMax(0), dwSpellType(0), dwLinkKindBullet(0),
			dwLinkKind(0), dwAbilityMin(0), dwAbilityMax(0), bCharged(0), eItemType(0), wItemEatk(0), dwParry(0), dwblockRating(0), nAddSkillMin(0), nAddSkillMax(0),
			dwAtkStyle(0), dwWeaponType(0), dwItemAtkOrder1(0), dwItemAtkOrder2(0), dwItemAtkOrder3(0), dwItemAtkOrder4(0), tmContinuousPain(0), dwRecoil(0), dwLoadingTime(0),
			nAdjHitRate(0), fAttackSpeed(0), dwDmgShift(0), dwAttackRange(0), nProbability(0), dwDestParam{}, nAdjParamVal{}, dwChgParamVal{}, nDestData1{}, dwActiveSkill(0),
			dwActiveSkillRate(0), dwReqMp(0), dwReqFp(0), dwReqDisLV(0), dwReSkill1(0), dwReSkillLevel1(0), dwReSkill2(0), dwReSkillLevel2(0), dwSkillReadyType(0), 
			dwSkillReady(0), _dwSkillRange(0), dwSfxElemental(0), dwSfxObj(0), dwSfxObj2(0), dwSfxObj3(0), dwSfxObj4(0), dwSfxObj5(0), dwUseMotion(0), dwCircleTime(0), 
			dwSkillTime(0), dwExeTarget(0), dwUseChance(0), dwSpellRegion(0), dwReferStat1(0), dwReferStat2(0), dwReferTarget1(0), dwReferTarget2(0), dwReferValue1(0), 
			dwReferValue2(0), dwSkillType(0), nItemResistElecricity(0), nItemResistDark(0), nItemResistFire(0), nItemResistWind(0), nItemResistWater(0), nItemResistEarth(0), 
			nEvildoing(0), dwExpertLV(0), dwExpertMax(0), dwSubDefine(0), dwExp(0), dwComboStyle(0), fFlightSpeed(0), fFlightLRAngle(0), fFlightTBAngle(0), dwFlightLimit(0),
			dwFFuelReMax(0), dwLimitLevel1(0), nReflect(0), dwSndAttack1(0), dwSndAttack2(0), dwQuestId(0), szTextFileName{}, szIcon{}, szCommand{}, nVer(0), dwBuffTickType(0),
			nMinLimitLevel(0), nMaxLimitLevel(0), nItemGroup(0), nUseLimitGroup(0), nMaxDuplication(0), nEffectValue(0), nTargetMinEnchant(0), nTargetMaxEnchant(0), bResetBind(0),
			nBindCondition(0), nResetBindCondition(0), dwHitActiveSkillId(0), dwHitActiveSkillLv(0), dwHitActiveSkillProb(0), dwHitActiveSkillTarget(0), dwDamageActiveSkillId(0),
			dwDamageActiveSkillLv(0), dwDamageActiveSkillProb(0), dwDamageActiveSkillTarget(0), dwEquipActiveSkillId(0), dwEquipActiveSkillLv(0), dwSmelting(0), dwAttsmelting(0),
			dwGemsmelting(0), dwPierce(0), dwUprouse(0), bAbsoluteTime(0)
		{
		}

		/*[[nodiscard]] bool IsUltimate() const { return dwReferStat1 == WEAPON_ULTIMATE; }
		[[nodiscard]] bool IsBaruna() const { return dwReferStat1 >= BARUNA_D && dwReferStat1 <= BARUNA_S; }
		[[nodiscard]] bool IsExperienceScroll() const { return dwItemKind3 == IK3_EXP_RATE; }
		[[nodiscard]] bool IsVisPet() const { return dwItemKind3 == IK3_PET && dwReferStat1 == PET_VIS; }
		[[nodiscard]] bool IsVis() const { return dwItemKind3 == IK3_VIS; }
		[[nodiscard]] bool IsVisKey() const { return II_SYS_SYS_VIS_KEY01 == dwID; }
		[[nodiscard]] bool IsPickupToBuff() const { return II_SYS_SYS_SCR_PET_MAGIC == dwID; } 
		[[nodiscard]] bool IsDebuff() const { return nEvildoing != 1 && dwReferTarget1 != RT_ATTACK && dwReferTarget2 != RT_ATTACK; }
		[[nodiscard]] bool IsBuff() const { return nEvildoing == 1; }
		[[nodiscard]] bool IsHeal() { return dwReferTarget1 == RT_HEAL; }
		[[nodiscard]] bool IsAlwaysAggro() const { return dwID == 400; }
		[[nodiscard]] bool isXZSkill() const 
		{
			switch (dwID)
			{
				case SI_MAG_MAG_BLINKPOOL: 
					return true;
				default: 
					return false;
			}
		}
		*/
	};

	enum JOB_PROP_TYPE { JOB_PROP_SWD, JOB_PROP_AXE, JOB_PROP_STAFF, JOB_PROP_STICK, JOB_PROP_KNUCKLE, JOB_PROP_WAND, JOB_PROP_BLOCKING, JOB_PROP_YOYO, JOB_PROP_CRITICAL, };
	struct JobProp
	{
		float fAttackSpeed;		
		float fFactorMaxHP;		
		float fFactorMaxMP;		
		float fFactorMaxFP;		
		float fFactorDef;		
		float fFactorHPRecovery;
		float fFactorMPRecovery;
		float fFactorFPRecovery;
		float fMeleeSWD;		
		float fMeleeAXE;		
		float fMeleeSTAFF; 		
		float fMeleeSTICK;		
		float fMeleeKNUCKLE;	
		float fMagicWAND;		
		float fBlocking;		
		float fMeleeYOYO;
		float fCritical;
	};

	struct RANDOM_ITEM
	{
		unsigned char m_lpQuestFlagArray[ITEM_KIND_MAX / 8 + 1];

		unsigned char m_nTotalNum;	
		unsigned char m_nUniqueMax;	
		unsigned char m_nUniqueMin;	
		unsigned short m_nGoldMax;	
		unsigned short m_nAmmoMax;	
		unsigned short m_nDartMax;	
		unsigned short m_nGoldMin;	
		unsigned short m_nAmmoMin;	
		unsigned short m_nDartMin;	
		RANDOM_ITEM() : m_lpQuestFlagArray{}, m_nTotalNum(0), m_nUniqueMax(0), m_nUniqueMin(0), m_nGoldMax(0),
		                   m_nAmmoMax(0), m_nDartMax(0),
		                   m_nGoldMin(0), m_nAmmoMin(0), m_nDartMin(0)
		{
		}
		void SetItemKind(int nItemKind, bool bBool);
		int IsItemKind(int nItemKind);
	};

	enum DROPTYPE { DROPTYPE_NORMAL, DROPTYPE_SEED, };
	struct DROPITEM
	{
		DROPTYPE dtType;
		unsigned long dwIndex;
		unsigned long dwProbability;
		unsigned long dwLevel;
		unsigned long dwNumber;
		unsigned long dwNumber2;
	};

	struct DROPKIND
	{
		unsigned long dwIK3;
		short nMinUniq;
		short nMaxUniq;
	};

	struct QUESTITEM
	{
		unsigned long dwQuest;
		unsigned long dwState;
		unsigned long dwIndex;
		unsigned long dwProbability;
		unsigned long dwNumber;
	};

	struct EVENTITEM
	{
		unsigned long dwIndex;
		unsigned long dwPrabability;
	};

	class CDropItemGenerator final
	{
		std::vector<DROPITEM> m_dropItems;

		public:
			unsigned long m_dwMax;

			CDropItemGenerator() : m_dwMax(0) { }

			~CDropItemGenerator() { Clear(); }
			void AddTail(const DROPITEM& rDropItem, const char* s);
			void Clear() { m_dropItems.clear(); }
			[[nodiscard]] unsigned long long GetSize() const { return m_dropItems.size(); }
			DROPITEM* GetAt(int nIndex, bool bUniqueMode, float fProbability = 0.0f);
	};

	class CDropKindGenerator final
	{
		int m_nSize;
		DROPKIND m_aDropKind[MAX_DROPKIND];

		public:
			CDropKindGenerator() : m_nSize(0), m_aDropKind{} { }
			~CDropKindGenerator() = default;
			
			void AddTail(const DROPKIND& rDropKind);
			[[nodiscard]] int GetSize() const { return m_nSize; }
			DROPKIND* GetAt(int nIndex);
	};

	class CQuestItemGenerator
	{
		unsigned long m_uSize;
		QUESTITEM m_pQuestItem[MAX_QUESTITEM];

		public:
			CQuestItemGenerator() : m_uSize(0), m_pQuestItem{} { }
			~CQuestItemGenerator() = default;

			void AddTail(const QUESTITEM& rQuestItem);
			[[nodiscard]] unsigned long GetSize() const { return m_uSize; }
			QUESTITEM* GetAt(int nIndex);
	};

	struct MonsterTransform
	{
		float fHPRate;
		unsigned long dwMonsterId;
		MonsterTransform() : fHPRate(0.0f), dwMonsterId(ffParser::NULL_ID) {}
	};

	struct MoverProp : CtrlProp
	{
		unsigned long dwStr;
		unsigned long dwSta;
		unsigned long dwDex;
		unsigned long dwInt;
		unsigned long dwHR;
		unsigned long dwER;
		unsigned long dwRace;
		unsigned long dwBelligerence;
		unsigned long dwGender;
		unsigned long dwLevel;
		unsigned long dwFlightLevel;
		unsigned long dwSize;
		unsigned long dwClass;
		int	bIfParts;
		int nChaotic;
		unsigned long dwUseable;
		unsigned long dwActionRadius;
		unsigned long dwAtkMin;
		unsigned long dwAtkMax;
		unsigned long dwAtk1;
		unsigned long dwAtk2;
		unsigned long dwAtk3;
		unsigned long dwAtk4;
		float fFrame;
		unsigned long dwOrthograde;
		unsigned long dwThrustRate;

		unsigned long dwChestRate;
		unsigned long dwHeadRate;
		unsigned long dwArmRate;
		unsigned long dwLegRate;

		unsigned long dwAttackSpeed;
		unsigned long dwReAttackDelay;
		unsigned long dwAddHp;
		unsigned long dwAddMp;
		unsigned long dwNaturalArmor;
		int nAbrasion;
		int nHardness;
		unsigned long dwAdjAtkDelay;

		int eElementType;
		short wElementAtk;

		unsigned long dwHideLevel;
		float fSpeed;
		unsigned long dwShelter;
		unsigned long dwFlying;
		unsigned long dwJumpIng;
		unsigned long dwAirJump;
		unsigned long bTaming;
		unsigned long dwResisMgic;

		int nResistElecricity;
		int nResistDark;
		int nResistFire;
		int nResistWind;
		int nResistWater;
		int nResistEarth;

		unsigned long dwCash;
		unsigned long dwSourceMaterial;
		unsigned long dwMaterialAmount;
		unsigned long dwCohesion;
		unsigned long dwHoldingTime;
		unsigned long dwCorrectionValue;
		long long nExpValue;
		int	nFxpValue;
		unsigned long nBodyState;
		unsigned long dwAddAbility;
		unsigned long bKillable;

		unsigned long dwVirtItem[3]{};
		unsigned long bVirtType[3]{};

		unsigned long dwSndAtk1;
		unsigned long dwSndAtk2;
		unsigned long dwSndDie1;
		unsigned long dwSndDie2;
		unsigned long dwSndDmg1;
		unsigned long dwSndDmg2;
		unsigned long dwSndDmg3;
		unsigned long dwSndIdle1;
		unsigned long dwSndIdle2;

		short m_nEvasionHP;
		short m_nEvasionSec;
		short m_nRunawayHP;
		short m_nCallHelperMax;
		short m_nCallHP;
		short m_nCallHelperIdx[5]{};
		short m_nCallHelperNum[5]{};
		short m_bCallHelperParty[5]{};

		short m_dwAttackMoveDelay;
		short m_dwRunawayDelay;
		short m_bPartyAttack;
		short m_nHelperNum;

		int m_nScanJob;
		int m_nHPCond;
		int m_nLvCond;
		int m_nRecvCondMe;
		int m_nRecvCondHow;
		int m_nRecvCondMP;
		unsigned char m_bMeleeAttack;
		unsigned char m_bRecvCondWho;
		unsigned char m_bRecvCond;
		unsigned char m_bHelpWho;
		unsigned char m_bRangeAttack[MAX_JOB]{};
		int m_nSummProb;
		int m_nSummNum;
		int m_nSummID;
		int m_nHelpRangeMul;
		unsigned long m_tmUnitHelp;
		int m_nBerserkHP;
		float m_fBerserkDmgMul;
		int m_nLoot;
		int m_nLootProb;
		unsigned long m_dwScanQuestId;
		unsigned long m_dwScanItemIdx;
		int	m_nScanChao;

		float m_fHitPoint_Rate;
		float m_fAttackPower_Rate;
		float m_fDefence_Rate;
		float m_fExp_Rate;
		float m_fItemDrop_Rate;
		float m_fPenya_Rate;
		int	m_bRate;

		short m_nAttackItemNear;
		short m_nAttackItemFar;
		short m_nAttackItem1;
		short m_nAttackItem2;
		short m_nAttackItem3;
		short m_nAttackItem4;
		short m_nAttackItemSec;
		short m_nMagicReflection;
		short m_nImmortality;
		int m_bBlow;
		short m_nChangeTargetRand;

		short m_nAttackFirstRange;
		RANDOM_ITEM m_randomItem;
		CDropItemGenerator m_DropItemGenerator;
		CQuestItemGenerator m_QuestItemGenerator;
		CDropKindGenerator m_DropKindGenerator;
		MonsterTransform m_MonsterTransform;

		MoverProp() : dwStr(0), dwSta(0), dwDex(0), dwInt(0), dwHR(0), dwER(0), dwRace(0), dwBelligerence(0), dwGender(0), dwLevel(0), dwFlightLevel(0), dwSize(0), dwClass(0),
			bIfParts(0), nChaotic(0), dwUseable(0), dwActionRadius(0), dwAtkMin(0), dwAtkMax(0), dwAtk1(0), dwAtk2(0), dwAtk3(0), dwAtk4(0), fFrame(1.0F), dwOrthograde(0),
			dwThrustRate(0), dwChestRate(0), dwHeadRate(0), dwArmRate(0), dwLegRate(0), dwAttackSpeed(0), dwReAttackDelay(0), dwAddHp(0), dwAddMp(0), dwNaturalArmor(0), 
			nAbrasion(0), nHardness(0), dwAdjAtkDelay(0), eElementType(0), wElementAtk(0), dwHideLevel(0), fSpeed(0.0F), dwShelter(0), dwFlying(0), dwJumpIng(0), dwAirJump(0), 
			bTaming(0), dwResisMgic(0), nResistElecricity(0), nResistDark(0), nResistFire(0), nResistWind(0), nResistWater(0), nResistEarth(0), dwCash(0),
			dwSourceMaterial(0), dwMaterialAmount(0), dwCohesion(0), dwHoldingTime(0), dwCorrectionValue(0), nExpValue(0), nFxpValue(0), nBodyState(0), dwAddAbility(0), bKillable(0),
			dwSndAtk1(0), dwSndAtk2(0), dwSndDie1(0), dwSndDie2(0), dwSndDmg1(0), dwSndDmg2(0), dwSndDmg3(0), dwSndIdle1(0), dwSndIdle2(0), m_nEvasionHP(0),
			m_nEvasionSec(0), m_nRunawayHP(0), m_nCallHelperMax(0), m_nCallHP(0), m_dwAttackMoveDelay(0), m_dwRunawayDelay(0), m_bPartyAttack(0), m_nHelperNum(0), 
			m_nScanJob(0), m_nHPCond(0), m_nLvCond(0), m_nRecvCondMe(0), m_nRecvCondHow(0), m_nRecvCondMP(0), m_bMeleeAttack(0), m_bRecvCondWho(0),
			m_bRecvCond(0), m_bHelpWho(0), m_nSummProb(0), m_nSummNum(0), m_nSummID(0), m_nHelpRangeMul(0), m_tmUnitHelp(0), 
			m_nBerserkHP(0), m_fBerserkDmgMul(0), m_nLoot(0), m_nLootProb(0), m_dwScanQuestId(0), m_dwScanItemIdx(0), m_nScanChao(0), m_fHitPoint_Rate(0), m_fAttackPower_Rate(0), 
			m_fDefence_Rate(0), m_fExp_Rate(0), m_fItemDrop_Rate(0), m_fPenya_Rate(0.0F), m_bRate(0), m_nAttackItemNear(0), m_nAttackItemFar(0), m_nAttackItem1(0), m_nAttackItem2(0),
			m_nAttackItem3(0), m_nAttackItem4(0), m_nAttackItemSec(0), m_nMagicReflection(0), m_nImmortality(0), m_bBlow(0), m_nChangeTargetRand(0), m_nAttackFirstRange(0)
		{
			memset(dwVirtItem, 0, sizeof(dwVirtItem));
			memset(bVirtType, 0, sizeof(bVirtType));
			memset(m_nCallHelperIdx, 0, sizeof m_nCallHelperIdx);
			memset(m_nCallHelperNum, 0, sizeof m_nCallHelperNum);
			memset(m_bCallHelperParty, 0, sizeof(m_bCallHelperParty));
			memset(m_bRangeAttack, 0, sizeof(m_bRangeAttack));
		}
	};

	struct JOBITEM
	{
		unsigned long equipSlots[31/*MAX_HUMAN_PARTS*/][2];

	};


	struct BEGINPOSARR
	{
		unsigned long uSize;
		types::vec3d<float> avPos[MAX_BEGINPOS];
		BEGINPOSARR() : uSize(0), avPos{} { }
	};

	typedef struct tagEXPPARTY
	{
		unsigned long Exp;
		unsigned long Point;
	}
	EXPPARTY, * LPEXPPARTY;

	typedef struct tagRENEWEVENT
	{
		int	nLevel;
		unsigned long dwItemId;
		char strItemName[64];
		float fPercent;
	}
	RENEWEVENT, * LPRENEWEVENT;

	typedef struct tagRENEWEVENTITEM
	{
		char strTime[32];
		int nLevel;
		int	nRealitem[MAX_REALITEM];
		int nRealitemCount[MAX_REALITEM];
		int nSendRealitemCount[MAX_REALITEM];
		int nPercent[MAX_REALITEM];
		int nNextIndex;
	}
	RENEWEVENTITEM, * LPRENEWEVENTITEM;

	typedef struct tagExpCharacter
	{
		long long nExp1;
		long long nExp2;
		unsigned long dwLPPoint;
		long long nLimitExp;
	}
	EXPCHARACTER, * LPEXPCHARACTER;

	struct TreasureItem
	{
		unsigned long dwKeyID;
		unsigned long dwItemID;
		int nChance;
		int nMaxamount;
		int nRepeat;
		int nLimitamount;
		int bNotice;
	};
	struct TreasureBox
	{
		unsigned long dwBoxID;
		unsigned long dwKeyID[3];
	};





	extern std::string GetLangFileName(int nLang, int nType);
	struct QuestState 
	{
		char m_szDesc[512];
	};

	struct QuestGoalData
	{
		QuestGoalData();

		unsigned long m_dwGoalIndex;
		unsigned long worldId = 0;
		float m_fGoalPositionX;
		float m_fGoalPositionZ;
		unsigned long m_dwGoalTextID;
	};
	struct characterQuestGoalData : QuestGoalData
	{
		bool mustRequest = false;
	};

	struct QuestPropItem
	{
		char m_nSex;
		char m_nType;
		int m_nJobOrItem;
		int m_nItemIdx;
		int m_nItemNum;
		int m_nAbilityOption;
		unsigned char m_byFlag;
		QuestGoalData m_ItemGoalData;
		QuestPropItem() : m_nSex(0), m_nType(0), m_nJobOrItem(0), m_nItemIdx(0), m_nItemNum(0), m_nAbilityOption(0), m_byFlag(0) { }
	};

	struct QuestProp
	{
		unsigned short m_wId{};
		char m_szTitle[64]{};
		char m_szNpcName[32]{};
		unsigned short	m_nHeadQuest{};
		char m_nQuestType{};
		bool m_bNoRemove{};

		int m_nParam[4]{};
		char m_nBeginCondPreviousQuestType{};
		unsigned short m_anBeginCondPreviousQuest[6]{};
		unsigned short m_anBeginCondExclusiveQuest[6]{};
		char m_nBeginCondJob[MAX_JOB]{};
		char m_nBeginCondJobNum{};
		unsigned char m_nBeginCondLevelMax{};
		unsigned char m_nBeginCondLevelMin{};
		char m_nBeginCondParty{};
		char m_nBeginCondPartyNumComp{};
		char m_nBeginCondPartyNum{};
		char m_nBeginCondPartyLeader{};
		char m_nBeginCondGuild{};
		char m_nBeginCondGuildNumComp{};
		unsigned short m_nBeginCondGuildNum{};
		char m_nBeginCondGuildLeader{};
		QuestPropItem* m_paBeginCondItem{};
		char m_nBeginCondItemNum{};
		char m_nBeginCondSex{};
		int m_nBeginCondSkillIdx{};
		char m_nBeginCondSkillLvl{};
		QuestPropItem* m_paBeginCondNotItem{};
		char m_nBeginCondNotItemNum{};
		int m_nBeginCondPKValue{};
		int m_nBeginCondDisguiseMoverIndex{};
		int m_nBeginSetAddItemIdx[4]{};
		char m_nBeginSetAddItemNum[4]{};
		int m_nBeginSetAddGold{};
		int m_nBeginSetDisguiseMoverIndex{};
		int m_nBeginCondPetLevel{};
		int m_nBeginCondPetExp{};
		int m_nBeginCondTutorialState{};
		int m_nBeginCondTSP{};
		int m_nEndCondLimitTime{};
		QuestPropItem* m_paEndCondItem{};
		char m_nEndCondItemNum{};
		int m_nEndCondKillNPCIdx[MAX_QUEST_COND_KILL]{};
		int m_nEndCondKillNPCNum[MAX_QUEST_COND_KILL]{};
		QuestGoalData m_KillNPCGoalData[MAX_QUEST_COND_KILL];
		unsigned long m_dwEndCondPatrolWorld{};
		unsigned long m_dwPatrolDestinationID{};
		QuestGoalData m_PatrolWorldGoalData;
		types::CRect m_rectEndCondPatrol; 
		char m_szEndCondCharacter[64]{}; 
		characterQuestGoalData m_MeetCharacterGoalData;
		char* m_lpszEndCondMultiCharacter{};
		int m_nEndCondSkillIdx{};
		char m_nEndCondSkillLvl{};
		QuestPropItem* m_paEndCondOneItem{};
		char m_nEndCondOneItemNum{};
		int m_nEndCondGold{};
		unsigned char m_nEndCondLevelMin{};
		unsigned char m_nEndCondLevelMax{};
		int m_nEndCondExpPercentMin{};
		int m_nEndCondExpPercentMax{};

		int m_nEndCondPetLevel{};
		int m_nEndCondPetExp{};

		int m_nEndCondDisguiseMoverIndex{};
		char m_nEndCondParty{};
		char m_nEndCondPartyNumComp{};
		unsigned short 	m_nEndCondPartyNum{};
		char m_nEndCondPartyLeader{};
		char m_nEndCondGuild{};
		char m_nEndCondGuildNumComp{};
		unsigned short	m_nEndCondGuildNum{};
		char m_nEndCondGuildLeader{};
		unsigned char m_nEndCondState{};
		unsigned char m_nEndCondCompleteQuestOper{};
		unsigned short m_nEndCondCompleteQuest[6]{};
		char m_szEndCondDlgCharKey[64]{};
		QuestGoalData m_DialogCharacterGoalData;
		char m_szEndCondDlgAddKey[64]{};
		char m_szPatrolZoneName[64]{};
		int m_nEndCondTSP{};
		int m_nDlgRewardItemIdx[4]{};
		int m_nDlgRewardItemNum[4]{};
		int m_nEndRemoveItemIdx[8]{};
		int m_nEndRemoveItemNum[8]{};
		int m_nEndRemoveGold{};
		int m_anEndRemoveQuest[MAX_QUESTREMOVE]{};
		int m_nEndRemoveTSP{};
		QuestPropItem* m_paEndRewardItem{};
		int m_nEndRewardItemNum{};
		int m_nEndRewardGoldMin{};
		int m_nEndRewardGoldMax{};
		int m_nEndRewardExpMin{};
		int m_nEndRewardExpMax{};
		int m_nEndRewardPKValueMin{};
		int m_nEndRewardPKValueMax{};
		int m_nEndRewardTeleport{};
		types::vec3d<float> m_nEndRewardTeleportPos;
		int	m_bEndRewardPetLevelup{};
		int m_nEndRewardSkillPoint{};
		bool m_bEndRewardHide{};
		bool m_bRepeat{};
		int m_nEndRewardTSP{};
		
		//char* m_apQuestDialog[32];
		QuestState* m_questState[16]{};
	};

	struct FILTER
	{
		char m_szSrc[64];
		char m_szDst[64];
	};

	struct JOB
	{
		char szName[32];
		char szEName[32];
		unsigned long dwJobBase;
		unsigned long dwJobType;
	};

	struct STRUCTURE
	{
		char szName[32];
		char szEName[32];
	};

	struct GUILD_APPELL
	{
		char szName[32];
		char szEName[32];
	};

	struct FXPCHARACTER
	{
		unsigned long dwFxp;
		unsigned long dwFxp2;
	};

	struct ATKSTYLE
	{
		unsigned long dwParam1;
		unsigned long dwParam2;
	};

	struct ADDEXPPARTY
	{
		unsigned long Exp;
		unsigned long Level;
	};

	struct MotionProp
	{
		unsigned long dwID;
		unsigned long dwAction;
		unsigned long dwMotion;
		unsigned long dwPlay;		
		unsigned long dwRequireLv;
		unsigned long dwRequireExp;
		char szName[32];	
		char szRoot[32];	
		char szLink[128];
		char szIconName[32];
		char szDesc[128];
	};

	struct VENDOR_ITEM
	{
		unsigned long m_dwItemId;
		int m_nItemkind3;
		int m_nItemJob;
		int m_nUniqueMin;
		int m_nUniqueMax;
		int m_nTotalNum;
		int m_nMaterialCount;
	};

	struct NPC_BUFF_SKILL
	{
		unsigned long dwSkillID;
		unsigned long dwSkillLV;
		int nMinPlayerLV;
		int nMaxPlayerLV;
		unsigned long dwSkillTime;
	};

	struct CHARACTER
	{
		char m_szKey[64];
		std::string m_strName;
		char m_szChar[64];
		char m_szDialog[64];
		char m_szDlgQuest[64];	
		int m_nStructure;
		int m_nEquipNum;
		unsigned long m_dwMoverIdx;
		unsigned long m_dwMusicId;
		unsigned long m_adwEquip[MAX_HUMAN_PARTS];
		unsigned long m_dwHairMesh;
		unsigned long m_dwHairColor;
		unsigned long m_dwHeadMesh;
		RANDOM_ITEM	m_randomItem;
		int m_abMoverMenu[MAX_MOVER_MENU];
		int m_abMoverMenu2[MAX_MOVER_MENU];
		std::string m_abMoverMenuText[MAX_MOVER_MENU];
		std::string m_venderSlot[4];

		std::vector<VENDOR_ITEM> m_venderItemAry[4];

		std::vector<unsigned short> m_awSrcQuest;
		std::vector<unsigned short> m_awDstQuest;
		std::vector<unsigned int> m_anSrcQuestItem;
		std::vector<unsigned int> m_anDstQuestItem;
		void Clear();

		unsigned long m_nVenderType;
		std::vector<VENDOR_ITEM> m_venderItemAry2[4];
		std::vector<NPC_BUFF_SKILL> m_vecNPCBuffSkill;
		std::vector<unsigned long> m_vecdwLanguage;

		int bOutput;
		unsigned long m_dwWorldId;
		types::vec3d<float> m_vPos;
		std::vector<types::vec3d<float>> m_vecTeleportPos;

		std::vector<VENDOR_ITEM> m_venderItemAry3[4];
		//int m_bTeleporter;
		//std::map<unsigned long, TeleportStruct*> m_mapTeleportMenuPos;

	};

	struct MONSTER_RESPAWN
	{
		char szMonsterName[32]{};
		int nRespawnIndex;
		types::vec3d<float> vPos;
		int nQuantity;
		int nAggressive;
		int nRange;
		int nTime;

		MONSTER_RESPAWN() : nRespawnIndex(0), nQuantity(0), nAggressive(0), nRange(0), nTime(0) {
			szMonsterName[0] = '\0';
		}
	};

	struct MONSTER_PROP
	{
		char szMonsterName[32]{};
		int nHitPoint;
		int nAttackPower;
		int nDefence;
		int nExp;
		int nItemDrop;
		int nPenya;
		MONSTER_PROP() : nHitPoint(0), nAttackPower(0), nDefence(0), nExp(0), nItemDrop(0), nPenya(0)
		{
			szMonsterName[0] = '\0';
		}
	};

	struct DIE_PENALTY
	{
		int nLevel;
		int nValue;
		DIE_PENALTY() : nLevel(0), nValue(0) { }
	};

	struct CHAO_PROPENSITY
	{
		unsigned long dwPropensityMin;
		unsigned long dwPropensityMax;
		int nEquipDorpMin; 
		int nEquipDorpMax;
		int nInvenDorpMin;
		int nInvenDorpMax;
		int nShop;	
		int nBank;	
		int nFly;
		int nGuardAttack;
		int nVendor;
		CHAO_PROPENSITY(): dwPropensityMin(0), dwPropensityMax(0), nEquipDorpMin(0), nEquipDorpMax(0), nInvenDorpMin(0), nInvenDorpMax(0), nShop(1), nBank(1), nFly(1),
			nGuardAttack(1), nVendor(1)
		{
		}
	};

	struct PK_SETTING
	{
		unsigned long dwGeneralColor;
		unsigned long dwReadyColor;	
		unsigned long dwChaoColor;	
		int nGeneralAttack;		
		int nReadyAttack;		
		int nLimitLevel;		
		int nDiePropensityMin;	
		int nDiePropensityMax;	
		std::map<int, CHAO_PROPENSITY> mapPropensityPenalty;
		std::map<int, unsigned long> mapLevelExp;			
		PK_SETTING(): dwGeneralColor(0), dwReadyColor(0), dwChaoColor(0), nGeneralAttack(0), nReadyAttack(0), nLimitLevel(0), nDiePropensityMin(0), nDiePropensityMax(0)
		{
		}
	};

	struct AMP_STORE_INFO
	{
		unsigned long dwIDSecond;
		unsigned long dwIDThird;
		unsigned long dwIDFourth;
		unsigned long dwIDFifth;

		AMP_STORE_INFO() : dwIDSecond(0), dwIDThird(0), dwIDFourth(0), dwIDFifth(0) {}
	};

	struct internalGiftBoxItem
	{
		unsigned long adwItem;
		unsigned long adwProbability;
		int anNum;
		int	anSpan;
		int anAbilityOption;
		unsigned char anFlag;
	};

	struct internalPackBoxItem
	{
		unsigned long adwItem;
		int anNum;
		int anAbilityOption;
		unsigned char anFlag;
	};

	union boxItems
	{
		internalGiftBoxItem gb;
		internalPackBoxItem pb;
	};

	struct GIFTBOX
	{
		unsigned long dwGiftbox;
		unsigned long nSum;

		std::vector<boxItems> itemList;
		GIFTBOX() : dwGiftbox(0), nSum(0) { }
		[[nodiscard]] size_t size() const { return itemList.size(); }
	};

	struct GIFTBOXRESULT
	{
		unsigned long dwItem;
		int nNum;
		int nSpan;
		int	nAbilityOption;
		unsigned char nFlag;

		GIFTBOXRESULT() : dwItem(0), nNum(0), nSpan(0), nAbilityOption(0), nFlag(0) { }
	};


	class CGiftboxMan
	{
		std::map<unsigned long, GIFTBOX> itemList;

		public:
			CGiftboxMan();
			~CGiftboxMan();

			CGiftboxMan(CGiftboxMan const&) = delete;
			CGiftboxMan& operator =(CGiftboxMan const&) = delete;
			CGiftboxMan(CGiftboxMan&&) = delete;
			CGiftboxMan& operator=(CGiftboxMan&&) = delete;

			static CGiftboxMan* GetInstance();
			std::vector<boxItems>* getInternalItemList(unsigned long mainId);

			bool isGiftbox(unsigned long itemId);
			GIFTBOX* getGiftbox(unsigned long itemId);

			std::pair<std::map<unsigned long, GIFTBOX>::iterator, bool> insert(unsigned long id, GIFTBOX gb);

//#ifdef __WORLDSERVER
//		bool open(unsigned long itemId, PGIFTBOXRESULT pGiftboxResult);
//#endif
	};

	struct PACKITEMELEM
	{
		unsigned long dwPackItem;
		unsigned long span;
		std::vector<boxItems> itemList;
		PACKITEMELEM() : dwPackItem(0), span(0) {}
		[[nodiscard]] size_t size() const { return itemList.size(); }
	};

	class CPackItem
	{
		std::map<unsigned long, PACKITEMELEM> itemList;

		public:
		CPackItem();
		~CPackItem() = default;

		CPackItem(CPackItem const&) = delete;
		CPackItem& operator =(CPackItem const&) = delete;
		CPackItem(CPackItem&&) = delete;
		CPackItem& operator=(CPackItem&&) = delete;

		std::map<unsigned long, PACKITEMELEM>& getItemList() { return itemList; }
		static CPackItem* GetInstance();
		PACKITEMELEM Open(unsigned long dwPackItem);

		bool isPackItem(unsigned long itemId);
		PACKITEMELEM* getPackItem(unsigned long itemId);

	};

	struct ITEMAVAIL
	{
		int nSize;
		int anDstParam[MAX_ITEMAVAIL]{};
		int anAdjParam[MAX_ITEMAVAIL]{};
		ITEMAVAIL() : nSize(0) { }
	};

	struct NEWSETITEMAVAIL : ITEMAVAIL
	{
		int anEquiped[MAX_ITEMAVAIL]{};
	};

	class CSetItem final
	{
		public:
			int m_nId;
			char m_pszString[MAX_SETITEM_STRING];
			int m_nElemSize;
			int	m_anParts[MAX_SETITEM_ELEM];
			unsigned long m_adwItemId[MAX_SETITEM_ELEM];
			NEWSETITEMAVAIL	m_avail;

			CSetItem(int nId, const char* pszString);
			~CSetItem() = default;

			int	AddSetItemElem(unsigned long dwItemId, int nParts);
			int	AddItemAvail(int nDstParam, int nAdjParam, int nEquiped);
			void SortItemAvail();

			void GetItemAvail(ITEMAVAIL* pItemAvail, int nEquiped, int bAll);
			char* GetString() { return m_pszString; }
	};

	class CSetItemFinder final
	{
		std::map<unsigned long, CSetItem*> m_mapItemId;
		std::map<int, CSetItem*> m_mapSetId;

		public:
			CSetItemFinder() = default;
			~CSetItemFinder() { Free(); }

			void AddSetItem(CSetItem* pSetItem);
			CSetItem* GetSetItem(int nSetItemId);
			CSetItem* GetSetItemByItemId(unsigned long dwItemId);
			void Free();
			static CSetItemFinder* GetInstance();
	};

	struct SETITEMAVAIL
	{
		int nHitRate;
		int nBlock;
		int	nMaxHitPointRate;
		int nAddMagic;
		int nAdded;
	};

	struct PIERCINGAVAIL
	{
		unsigned long dwItemId;
		int nSize;
		int anDstParam[MAX_PIERCING_DSTPARAM];
		int anAdjParam[MAX_PIERCING_DSTPARAM];
	};

	class CPiercingAvail
	{
		int m_nSize;
		std::map<unsigned long, int> m_mapIdx;
		PIERCINGAVAIL m_pPiercingAvail[MAX_PIERCING_MATERIAL];

		public:
			CPiercingAvail();
			~CPiercingAvail();
			int	AddPiercingAvail(unsigned long dwItemId, int nDstParam, int nAdjParam);
			PIERCINGAVAIL* GetPiercingAvail(unsigned long dwItemId);
			static CPiercingAvail* GetInstance();
	};



	struct RANDOMOPTITEM
	{
		int	nId;
		int nLevel;
		char pszString[MAX_RANDOMOPTITEMSTRING]{};
		unsigned long dwProbability;
		ITEMAVAIL ia;
		RANDOMOPTITEM() : nId(0), nLevel(0), dwProbability(0)
		{
			*pszString = '\0';
		}
	};

	class CRandomOptItemGen
	{
		int m_nSize;
		RANDOMOPTITEM	m_aRandomOptItem[MAX_RANDOMOPTITEM];
		std::map<int, int>	m_mapid;

		int m_anIndex[MAX_MONSTER_LEVEL];
		public:
			CRandomOptItemGen();
			virtual	~CRandomOptItemGen() { Free(); }

			int	AddRandomOption(RANDOMOPTITEM* pRandomOptItem);
			RANDOMOPTITEM* GetRandomOptItem(int nId);
			const char* GetRandomOptItemString(int nId);
			int GenRandomOptItem(int nLevel, float fPenalty, ItemProp* pItemProp, unsigned long dwClass);
			void Arrange();
			void Free();

			static	CRandomOptItemGen* GetInstance();
	};

	struct GUILDQUESTPROP
	{
		char szTitle[MAX_GUILD_QUEST_TITLE_LEN]{};
		int nLevel;
		char szDesc[MAX_GUILD_QUEST_STATE][MAX_GUILD_QUEST_DESC_LEN]{};
		unsigned long dwWormon;
		unsigned long dwWorldId;
		types::vec3d<float> vPos;
		int	x1;
		int y1;
		int x2;
		int y2;

		GUILDQUESTPROP() : nLevel(0), dwWormon(0), dwWorldId(0), x1(0), y1(0), x2(0), y2(0) { }
	};

	struct GUILDQUEST
	{
		int nId;
		int nState;
		unsigned long idGuild;

		GUILDQUEST() : nId(-1), nState(0), idGuild(0) { }
	};

	struct WORMON
	{
		DWORD	dwWormon{};
		types::vec3d<float>	vPos{};
	};


	struct	PARTYQUESTPROP
	{
		char	szTitle[MAX_PARTY_QUEST_TITLE_LEN]{};
		int		nLevel;
		char	szDesc[MAX_PARTY_QUEST_STATE][MAX_PARTY_QUEST_DESC_LEN]{};
		DWORD	dwWorldId;
		char    szWorldKey[32]{};
		std::vector<WORMON> vecWormon;
		int	x1;
		int y1;
		int x2;
		int y2;

		PARTYQUESTPROP() : nLevel(0), dwWorldId(0), x1(0), y1(0), x2(0), y2(0)
		{
			vecWormon.clear();
		}
	};

	struct	PARTYQUEST
	{
		int nId;
		int nState;
		unsigned long idParty;

		PARTYQUEST() : nId(-1), nState(0), idParty(0)
		{
		}
	};


	struct WNDCTRL
	{
		unsigned long dwWndId;
		unsigned long dwWndType;
		unsigned long dwWndStyle;
		int bTile;

		std::string strDefine;
		std::string strTitle;
		std::string strToolTip;
		std::string strTexture;
		types::CRect rect;
		int m_bVisible;
		int m_bDisabled;
		int m_bTabStop;
		int m_bGroup;
	};

	struct WNDAPPLET
	{
		unsigned long dwWndId;
		unsigned long dwWndStyle;
		int bTile;

		std::string strTitle;
		std::string strToolTip;
		unsigned long d3dFormat;
		std::string strDefine;
		std::string strTexture;

		std::vector<WNDCTRL> ptrCtrlArray;
		types::vec2d<int> size;
	};


	struct MODELELEM
	{
		unsigned long m_dwType;
		unsigned long m_dwIndex;
		std::string m_szName;
		int m_nMax;
		std::vector<std::string> m_apszMotion;
		unsigned long m_dwModelType;
		std::string m_szPart;
		float m_fScale;
		int m_nTextureEx;

		unsigned char m_bMark;
		unsigned char m_bFly : 1;
		unsigned char m_dwDistant : 2;
		unsigned char m_bPick : 1;
		unsigned char m_bUsed : 1;
		unsigned char m_bTrans : 1;
		unsigned char m_bShadow : 1;
		unsigned char m_bReserved : 1;
		unsigned char m_bRenderFlag : 1;

	};

	static char g_szRoot[MAX_OBJTYPE][32] = { "obj", "ani", "ctrl", "sfx", "item", "mvr", "region", "obj", "path" };
	class modelMng
	{
		public:
			std::vector<MODELELEM> m_aaModelElem[MAX_OBJTYPE];

			modelMng() = default;
			~modelMng() = default;

			modelMng(modelMng const&) = delete;
			modelMng& operator =(modelMng const&) = delete;
			modelMng(modelMng&&) = delete;
			modelMng& operator=(modelMng&&) = delete;

			MODELELEM* GetModelElem(unsigned long dwType, unsigned long dwIndex);
			std::string MakeBoneName(unsigned long dwType, unsigned long dwIndex);
			void LoadScript(ffTextParser::CScript&& script);
	};


	struct TERRAIN
	{
		char m_szTextureFileName[128];
		char m_szSoundFileName[128];
		bool m_bBlock;
		unsigned long m_dwId;
	};


	typedef std::map<unsigned long, std::string> ULONG2STRING;
	typedef std::map<std::string, unsigned long> STRING2ULONG;
	typedef std::map<unsigned long, AMP_STORE_INFO*> MAPAMPSTORE;


	struct baseWorldStruct
	{
		std::string fileName;
		types::vec2d<int> size;
		std::vector<std::string> sky;
		std::vector<std::string> clouds;
		std::string sun;
		std::string moon;
		std::string seaClout;
		//todo: cnt struct

		baseWorldStruct() = default;
		explicit baseWorldStruct(std::string fn) : fileName(std::move(fn)) {}
	};




}

namespace ffProps = res::props;